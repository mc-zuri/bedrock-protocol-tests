import type { PacketMeta, PacketRecord } from '../shared/bdt.ts'
import type { CompiledProtocol } from './protocol.ts'

export type Status =
  | 'ok'
  | 'unknown_id'
  | 'read_error'
  | 'trailing_bytes'
  | 'write_error'
  | 'mismatch'
  // invalid UTF-8 cannot survive a JS string: a limit of JS, not a protocol bug
  | 'lossy_string'
  // the game wrote a bool as a byte other than 0/1, which re-encodes as 01
  | 'noncanonical_bool'

export const PASSING: ReadonlySet<Status> = new Set<Status>(['ok', 'lossy_string', 'noncanonical_bool'])

export interface Outcome {
  status: Status
  error?: string
  consumed?: number
  reencoded?: Buffer
  diffAt?: number
  params?: unknown
}

export function checkPacket (proto: CompiledProtocol, names: Map<number, string>, r: PacketRecord): Outcome {
  if (!names.has(r.id)) return { status: 'unknown_id', error: `packet id ${r.id} is not in mcpe_packet` }
  let parsed: { data: any, metadata: { size: number } }
  try {
    parsed = proto.parsePacketBuffer('mcpe_packet', r.buffer)
  } catch (e) {
    return { status: 'read_error', error: errorText(e) }
  }
  const params = parsed.data?.params
  const consumed = parsed.metadata.size
  if (consumed !== r.buffer.length) {
    return {
      status: 'trailing_bytes',
      consumed,
      params,
      error: `read ${consumed} of ${r.buffer.length} bytes (${r.buffer.length - consumed} left over)`
    }
  }
  let reencoded: Buffer
  try {
    reencoded = proto.createPacketBuffer('mcpe_packet', parsed.data)
  } catch (e) {
    return { status: 'write_error', consumed, params, error: errorText(e) }
  }
  if (!reencoded.equals(r.buffer)) {
    const diffAt = firstDifference(reencoded, r.buffer)
    if (onlyBoolNormalization(r.buffer, reencoded)) {
      return {
        status: 'noncanonical_bool',
        consumed,
        params,
        reencoded,
        diffAt,
        error: 'a bool byte other than 0/1 re-encodes as 01'
      }
    }
    if (hasReplacementChar(params)) {
      return {
        status: 'lossy_string',
        consumed,
        params,
        reencoded,
        diffAt,
        error: 'a string is not valid UTF-8; JS decodes it to U+FFFD, so it cannot re-encode byte for byte'
      }
    }
    return {
      status: 'mismatch',
      consumed,
      params,
      reencoded,
      diffAt,
      error: reencoded.length === r.buffer.length
        ? `re-encoded bytes differ (same length ${reencoded.length})`
        : `re-encoded ${reencoded.length} bytes, game wrote ${r.buffer.length}`
    }
  }
  return { status: 'ok', consumed }
}

Error.stackTraceLimit = 100

// protodef compiles one eval'd function per type, so the stack frames name the type chain that failed
function errorText (e: unknown): string {
  if (!(e instanceof Error)) return String(e)
  let msg = e.message.split('\n')[0].replace(/^(Read|Write|SizeOf) error for undefined : /, '')
  if (msg === 'undefined' || msg === '') msg = e.name === 'PartialReadError' ? 'unexpected end of buffer' : e.name
  const chain: string[] = []
  for (const line of (e.stack ?? '').split('\n')) {
    const m = /at (?:Object\.)?(?:reader|writer|sizeOf)? ?\[as ([\w$]+)\]|at (?:Object\.)?([\w$]+) \(eval at compile/.exec(line)
    const name = m?.[1] ?? m?.[2]
    if (name && name !== 'eval' && chain[0] !== name) chain.unshift(name)
  }
  if (chain[0] === 'mcpe_packet') chain.shift()
  return chain.length ? `${msg} [in ${chain.join(' > ')}]` : msg
}

function onlyBoolNormalization (game: Buffer, reencoded: Buffer): boolean {
  if (game.length !== reencoded.length) return false
  for (let i = 0; i < game.length; i++) {
    if (game[i] !== reencoded[i] && !(reencoded[i] === 1 && game[i] > 1)) return false
  }
  return true
}

function hasReplacementChar (v: unknown): boolean {
  if (typeof v === 'string') return v.includes('�')
  if (v && typeof v === 'object') return Object.values(v).some(hasReplacementChar)
  return false
}

function firstDifference (a: Buffer, b: Buffer): number {
  const n = Math.min(a.length, b.length)
  for (let i = 0; i < n; i++) if (a[i] !== b[i]) return i
  return n
}

export function caseLabel (meta: PacketMeta | null): string {
  return meta?.p ?? '?'
}

export function toJSONSafe (value: unknown, maxLength = 4000): unknown {
  const text = JSON.stringify(value, (_k, v) => {
    if (typeof v === 'bigint') return v.toString()
    if (v && typeof v === 'object' && v.type === 'Buffer' && Array.isArray(v.data)) return 'hex:' + Buffer.from(v.data).toString('hex')
    return v
  })
  if (text === undefined) return undefined
  return text.length > maxLength ? text.slice(0, maxLength) + '…' : JSON.parse(text)
}
