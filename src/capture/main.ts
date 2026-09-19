import { existsSync, mkdirSync, renameSync, writeFileSync } from 'node:fs'
import { dirname, join } from 'node:path'
import { encodePackets } from '../shared/bdt.ts'
import { capturedFile, compareVersions, JOBS, OUT_DIR, pool, versions } from '../shared/config.ts'
import { serverExe, start, type RunningServer } from '../servers/server.ts'
import { capture } from './capture.ts'

const only = process.argv.slice(2)
const wanted = versions.filter(b => !only.length || only.includes(b.mcDataVersion))
const todo = wanted.filter(b => !existsSync(capturedFile(b.mcDataVersion)))
console.log(`captures: ${join(OUT_DIR, '<version>', 'captured-packets.bin')} (${todo.length} to capture, ${wanted.length - todo.length} already there, ${JOBS} servers at a time)`)

const failed: string[] = []
await pool(todo, JOBS, async b => {
  const v = b.mcDataVersion
  if (!existsSync(serverExe(b))) {
    failed.push(v)
    console.error(`  ${v}: no server (run pnpm servers)`)
    return
  }
  let server: RunningServer | undefined
  const t0 = performance.now()
  const took = () => `${((performance.now() - t0) / 1000).toFixed(1)} s`
  try {
    server = await start(b)
    const { packets, decodeFailures } = await capture(v, compareVersions(v, '1.21.60') >= 0, server.port)
    const file = capturedFile(v)
    mkdirSync(dirname(file), { recursive: true })
    const records = [...packets].sort(([a], [b]) => a.localeCompare(b)).map(([name, buffer]) => ({ buffer, meta: { p: name } }))
    writeFileSync(`${file}.tmp`, encodePackets(b.serverVersion, records))
    renameSync(`${file}.tmp`, file)
    console.log(`  ${v}: ${packets.size} packets (${took()})`)
    for (const f of decodeFailures) console.log(`  ${v}: bedrock-protocol could not decode ${f}`)
  } catch (e) {
    failed.push(v)
    console.error(`  ${v}: FAILED: ${e instanceof Error ? e.message : e} (${took()})`)
  } finally {
    await server?.stop()
  }
})

console.log(`captured ${todo.length - failed.length}/${todo.length}`)
if (failed.length) {
  console.error(`failed: ${failed.sort(compareVersions).join(', ')}`)
  process.exitCode = 1
}
