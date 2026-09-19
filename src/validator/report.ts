import type { PacketRecord } from '../shared/bdt.ts'
import { caseLabel, PASSING, toJSONSafe, type Outcome, type Status } from './validate.ts'

const MAX_SAMPLES = 3
const MAX_CASES = 25
const MAX_HEX_BYTES = 2048

export interface Sample {
  record: number
  case: string
  meta: unknown
  hex: string
  reencodedHex?: string
  diffAt?: number
  consumed?: number
  params?: unknown
}

export interface FailureGroup {
  status: Status
  error: string
  count: number
  cases: string[]
  samples: Sample[]
}

export interface PacketReport {
  id: number
  name: string
  total: number
  ok: number
  byStatus: Partial<Record<Status, number>>
  failures: FailureGroup[]
}

export interface SourceReport {
  records: number
  checked: number
  ok: number
  byStatus: Partial<Record<Status, number>>
  packets: Record<string, PacketReport>
}

export class ReportBuilder {
  private packets = new Map<number, PacketReport>()
  private groups = new Map<string, FailureGroup>()
  readonly report: SourceReport

  constructor (records: number) {
    this.report = { records, checked: 0, ok: 0, byStatus: {}, packets: {} }
  }

  add (r: PacketRecord, name: string | undefined, o: Outcome): void {
    const rep = this.report
    rep.checked++
    rep.byStatus[o.status] = (rep.byStatus[o.status] ?? 0) + 1
    let p = this.packets.get(r.id)
    if (!p) {
      p = { id: r.id, name: name ?? `#${r.id}`, total: 0, ok: 0, byStatus: {}, failures: [] }
      this.packets.set(r.id, p)
    }
    p.total++
    p.byStatus[o.status] = (p.byStatus[o.status] ?? 0) + 1
    if (PASSING.has(o.status)) {
      rep.ok++
      p.ok++
      return
    }
    const key = `${r.id}|${o.status}|${o.error ?? ''}`
    let g = this.groups.get(key)
    if (!g) {
      g = { status: o.status, error: o.error ?? '', count: 0, cases: [], samples: [] }
      this.groups.set(key, g)
      p.failures.push(g)
    }
    g.count++
    const label = caseLabel(r.meta)
    if (g.cases.length < MAX_CASES && !g.cases.includes(label)) g.cases.push(label)
    if (g.samples.length < MAX_SAMPLES) {
      g.samples.push({
        record: r.index,
        case: label,
        meta: r.meta,
        hex: hex(r.buffer),
        reencodedHex: o.reencoded ? hex(o.reencoded) : undefined,
        diffAt: o.diffAt,
        consumed: o.consumed,
        params: o.params === undefined ? undefined : toJSONSafe(o.params)
      })
    }
  }

  finish (): SourceReport {
    const sorted = [...this.packets.values()].sort((a, b) => a.id - b.id)
    for (const p of sorted) {
      p.failures.sort((a, b) => b.count - a.count)
      this.report.packets[p.name] = p
    }
    return this.report
  }
}

function hex (b: Buffer): string {
  return b.length > MAX_HEX_BYTES ? b.subarray(0, MAX_HEX_BYTES).toString('hex') + '…' : b.toString('hex')
}
