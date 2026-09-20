// pnpm relay [version]: starts the build's server and a bedrock-protocol relay in front of it, so a real client can
// join. Every packet either side sends is validated as it passes and appended to the build's captured-packets.bin.
import { createHash } from 'node:crypto'
import { closeSync, existsSync, mkdirSync, openSync, writeFileSync, writeSync } from 'node:fs'
import { createRequire } from 'node:module'
import { dirname } from 'node:path'
import { createInterface } from 'node:readline'
import { styleText } from 'node:util'
import { encodePackets, encodeRecord, readPackets } from '../shared/bdt.ts'
import { capturedFile, RELAY_PORT, versions } from '../shared/config.ts'
import { serverExe, start } from '../servers/server.ts'
import { dumpShieldItemId, hasProtocol, loadProtocol, shieldItemId } from '../validator/protocol.ts'
import { checkPacket, PASSING } from '../validator/validate.ts'
import { packetId } from './capture.ts'

const require = createRequire(import.meta.url)
const { Relay } = require('bedrock-protocol')

// the real client's login carries its Xbox identity and a token that can join servers as that player: it is
// validated like everything else, but never written to disk
const NOT_STORED = new Set(['login', 'sub_client_login'])
const STATUS_EVERY_MS = 5000

const version = process.argv[2] ?? versions.at(-1)!.mcDataVersion
const build = versions.find(b => b.mcDataVersion === version)
if (!build) throw new Error(`${version} is not in versions.json`)
if (!existsSync(serverExe(build))) throw new Error(`no ${serverExe(build)} (run pnpm servers)`)
// minecraft-data may name the protocol after the server build instead (1.26.51 for build 1.26.51.1)
const protocolVersion = [version, build.serverVersion.split('.').slice(0, 3).join('.')].find(hasProtocol)
if (!protocolVersion) throw new Error(`minecraft-data has no bedrock protocol for ${version}`)

const file = capturedFile(version)
if (!existsSync(file)) {
  mkdirSync(dirname(file), { recursive: true })
  writeFileSync(file, encodePackets(build.serverVersion, []))
}
const before = readPackets(file)
const hash = (buffer: Buffer) => createHash('sha1').update(buffer).digest('base64')
// a packet whose bytes are already in the file would only validate the same way again
const stored = new Set(before.map(r => hash(r.buffer)))

const { proto, packetNames } = loadProtocol(protocolVersion)
const shieldId = dumpShieldItemId(proto, packetNames, before) ?? shieldItemId(protocolVersion)
if (shieldId !== undefined) proto.setVariable('ShieldItemID', shieldId)

const out = openSync(file, 'a')
const seen = { packets: 0, ok: 0, appended: 0, duplicates: 0, kinds: new Set<number>() }
const failures = new Map<string, { from: string, name: string, text: string, count: number }>()

function record (from: 'client' | 'server', raw: Buffer): void {
  const buffer = Buffer.from(raw)
  const id = packetId(buffer)
  const name = packetNames.get(id) ?? `id_${id}`
  seen.packets++
  seen.kinds.add(id)

  const outcome = checkPacket(proto, packetNames, { index: seen.packets, id, buffer, meta: null })
  if (PASSING.has(outcome.status)) {
    seen.ok++
  } else {
    const key = `${from}|${id}|${outcome.status}|${outcome.error ?? ''}`
    const known = failures.get(key)
    if (known) {
      known.count++
    } else {
      const text = `${outcome.status}: ${outcome.error ?? ''}`
      failures.set(key, { from, name, text, count: 1 })
      console.log(`  ${styleText('red', 'FAIL')} ${from} ${name} ${text}`)
    }
  }

  if (NOT_STORED.has(name)) return
  const digest = hash(buffer)
  if (stored.has(digest)) {
    seen.duplicates++
    return
  }
  stored.add(digest)
  writeSync(out, encodeRecord(buffer, { p: name, from }))
  seen.appended++
}

const server = await start(build)
const relay = new Relay({
  version,
  host: '0.0.0.0',
  port: RELAY_PORT,
  offline: true,
  // an undecodable packet is the most interesting one to capture: keep the session going
  omitParseErrors: true,
  motd: { motd: `bedrock-protocol-tests ${version}`, levelName: 'capturing packets' },
  destination: { host: '127.0.0.1', port: server.port, offline: true }
})

relay.on('connect', (player: any) => {
  console.log(`  ${player.connection.address} connected`)
  // both hand over one decrypted, decompressed packet as the other side wrote it, before the relay decodes it
  const readPacket = player.readPacket.bind(player)
  player.readPacket = (buffer: Buffer) => {
    record('client', buffer)
    try {
      return readPacket(buffer)
    } catch (e) {
      console.log(`  relay could not pass on client ${packetNames.get(packetId(buffer)) ?? packetId(buffer)}: ${e instanceof Error ? e.message.split('\n')[0] : e}`)
    }
  }
  const readUpstream = player.readUpstream.bind(player)
  player.readUpstream = (buffer: Buffer) => {
    record('server', buffer)
    return readUpstream(buffer)
  }
  player.on('close', () => console.log(`  ${player.connection.address} disconnected`))
})

const status = () => `${seen.packets} packets of ${seen.kinds.size} kinds, ${seen.ok} ok, ${seen.packets - seen.ok} failed; ${seen.appended} appended, ${seen.duplicates} already there`
let reported = 0
const ticker = setInterval(() => {
  if (seen.packets === reported) return
  reported = seen.packets
  console.log(`  ${status()}`)
}, STATUS_EVERY_MS)

let stopping = false
async function stop (code: number): Promise<void> {
  if (stopping) return
  stopping = true
  clearInterval(ticker)
  try {
    relay.close('relay stopped')
  } catch {}
  await server.stop()
  closeSync(out)
  console.log(`\n${version}: ${status()}`)
  for (const f of [...failures.values()].sort((a, b) => b.count - a.count)) console.log(`  ${String(f.count).padStart(6)}x ${f.from} ${f.name} ${f.text}`)
  console.log(`${file}: ${before.length} -> ${before.length + seen.appended} packets` + (failures.size ? ` (pnpm validate -V ${version} -v for the report)` : ''))
  process.exit(code)
}
process.on('SIGINT', () => { void stop(0) })
process.on('SIGTERM', () => { void stop(0) })
createInterface({ input: process.stdin }).on('line', line => {
  if (line.trim() === 'stop') void stop(0)
})
// the server is a child process: never leave it running behind a crash
process.on('uncaughtException', e => {
  console.error(e)
  void stop(1)
})

await relay.listen()
console.log(`${version}: server on 127.0.0.1:${server.port}, relay on port ${RELAY_PORT}
  join 127.0.0.1:${RELAY_PORT} with a ${version} client (Play > Servers > Add Server); Ctrl+C or "stop" ends it
  appending to ${file} (${before.length} packets)`)
