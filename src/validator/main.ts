import { existsSync, writeFileSync } from 'node:fs'
import { join } from 'node:path'
import { parseArgs, styleText } from 'node:util'
import { readPackets, type PacketRecord } from '../shared/bdt.ts'
import { capturedFile, compareVersions, generatedFile, OUT_DIR, reportFile, versions as allVersions } from '../shared/config.ts'
import { dumpShieldItemId, hasProtocol, loadProtocol, shieldItemId, type CompiledProtocol } from './protocol.ts'
import { ReportBuilder, type SourceReport } from './report.ts'
import { checkPacket, PASSING, type Status } from './validate.ts'

const { values: opts, positionals } = parseArgs({
  allowPositionals: true,
  options: {
    version: { type: 'string', short: 'V', multiple: true },
    verbose: { type: 'boolean', short: 'v' },
    strict: { type: 'boolean' },
    help: { type: 'boolean', short: 'h' }
  }
})

if (opts.help) {
  console.log(`Usage: pnpm validate [packets...] [options]

  packets              only these packets (name or id), e.g. pnpm validate start_game 52
  -V, --version <v>    only this version (repeatable; default: every version with packets)
  -v, --verbose        print every failure group with its cases
      --strict         exit 1 if anything fails

  Packets:  ${join(OUT_DIR, '<version>', 'generated-packets.bin')}
            ${join(OUT_DIR, '<version>', 'captured-packets.bin')}
  Reports:  ${join(OUT_DIR, '<version>', 'report.json')}`)
  process.exit(0)
}

const builds = allVersions.filter(b => !opts.version || opts.version.includes(b.mcDataVersion))
const packetFilter = positionals.map(p => p.toLowerCase())

const summary: { version: string, line: string }[] = []
let anyFailure = false

for (const { mcDataVersion: version, serverVersion } of builds) {
  const generated = existsSync(generatedFile(version)) ? readPackets(generatedFile(version)) : undefined
  const captures = existsSync(capturedFile(version)) ? readPackets(capturedFile(version)) : undefined
  if (!generated && !captures) continue
  // minecraft-data may name the protocol after the server build instead (1.26.51 for build 1.26.51.1)
  const protocolVersion = [version, serverVersion.split('.').slice(0, 3).join('.')].find(hasProtocol)
  if (!protocolVersion) {
    anyFailure = true
    console.log(`\n${version}: minecraft-data has no bedrock protocol for it`)
    summary.push({ version, line: styleText('red', 'no protocol in minecraft-data') })
    continue
  }

  const t0 = performance.now()
  const { proto, packetNames } = loadProtocol(protocolVersion)
  // the protocol reads a shield's extra blocking tick only when ShieldItemID matches, normally set from start_game
  const shieldId = dumpShieldItemId(proto, packetNames, [...captures ?? [], ...generated ?? []]) ?? shieldItemId(protocolVersion)
  if (shieldId !== undefined) proto.setVariable('ShieldItemID', shieldId)
  const selected = (id: number) => !packetFilter.length || packetFilter.includes(String(id)) || packetFilter.includes(packetNames.get(id) ?? '')

  console.log(`\n${version}`)
  const sources: Record<string, SourceReport> = {}
  if (generated) sources.generated = check(proto, packetNames, generated, selected)
  if (captures) sources.captured = check(proto, packetNames, captures, selected)

  const seen = new Set(Object.values(sources).flatMap(s => Object.values(s.packets).map(p => p.id)))
  const coverage = {
    protocolPacketsNotExercised: [...packetNames].filter(([id]) => selected(id) && !seen.has(id)).map(([id, n]) => `${id} ${n}`),
    idsNotInProtocol: [...seen].filter(id => !packetNames.has(id))
  }

  for (const [kind, rep] of Object.entries(sources)) {
    printSource(kind, rep)
    if (rep.checked !== rep.ok) anyFailure = true
  }
  if (coverage.protocolPacketsNotExercised.length) {
    console.log(`  not exercised (${coverage.protocolPacketsNotExercised.length}): ${coverage.protocolPacketsNotExercised.join(', ')}`)
  }
  summary.push({ version, line: Object.entries(sources).map(([k, r]) => `${k} ${r.ok}/${r.checked}${failed(r)}`).join(', ') })

  const file = reportFile(version)
  writeFileSync(file, JSON.stringify({ version, generatedAt: new Date().toISOString(), filter: packetFilter, sources, coverage }, null, 2))
  console.log(`  report: ${file}  (${((performance.now() - t0) / 1000).toFixed(1)} s)`)
}

if (!summary.length) {
  console.log(`nothing to validate: no packets in ${OUT_DIR} (pnpm capture, pnpm export)`)
} else {
  console.log('\nsummary (ok/checked)')
  for (const { version, line } of summary.sort((a, b) => compareVersions(a.version, b.version))) console.log(`  ${version.padEnd(10)} ${line}`)
}
process.exit(opts.strict && anyFailure ? 1 : 0)

function check (proto: CompiledProtocol, names: Map<number, string>, records: PacketRecord[], selected: (id: number) => boolean): SourceReport {
  const b = new ReportBuilder(records.length)
  for (const r of records) {
    if (!selected(r.id)) continue
    b.add(r, names.get(r.id), checkPacket(proto, names, r))
  }
  return b.finish()
}

function printSource (kind: string, rep: SourceReport): void {
  const bad = Object.entries(rep.byStatus).filter(([s]) => !PASSING.has(s as Status)).map(([s, n]) => `${s} ${n}`)
  const lenientOk = Object.entries(rep.byStatus).filter(([s]) => s !== 'ok' && PASSING.has(s as Status)).map(([s, n]) => `${n} ${s}`)
  console.log(`  ${kind}: ${rep.ok}/${rep.checked} ok` +
    (lenientOk.length ? ` (incl. ${lenientOk.join(', ')})` : '') +
    (bad.length ? `, ${bad.join(', ')}` : ''))
  const failing = Object.values(rep.packets).filter(p => p.ok !== p.total)
  if (!failing.length) return
  console.log(`    ${'id'.padStart(4)}  ${'packet'.padEnd(40)} ${'ok/total'.padStart(12)}  first failure`)
  for (const p of failing) {
    const f = p.failures[0]
    console.log(`    ${String(p.id).padStart(4)}  ${p.name.padEnd(40)} ${`${p.ok}/${p.total}`.padStart(12)}  ${f.status}: ${truncate(f.error, 90)}`)
    if (!opts.verbose) continue
    for (const g of p.failures) {
      console.log(`          ${g.count}x ${g.status}: ${g.error}`)
      for (const c of g.cases.slice(0, 8)) console.log(`              ${c}`)
      if (g.cases.length > 8) console.log(`              … ${g.cases.length - 8} more`)
      const s = g.samples[0]
      if (s) {
        console.log(`              game:       ${truncate(s.hex, 160)}`)
        if (s.reencodedHex) console.log(`              re-encoded: ${truncate(s.reencodedHex, 160)}  (differs at byte ${s.diffAt})`)
      }
    }
  }
}

function truncate (s: string, n: number): string {
  return s.length > n ? s.slice(0, n - 1) + '…' : s
}

function failed (rep: SourceReport): string {
  const n = rep.checked - rep.ok
  return n ? ' ' + styleText('red', `(${n} failed)`) : ''
}
