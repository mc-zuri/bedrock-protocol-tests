// varint(len) version, then records: 'L' int64(0) varint(len) JSON meta for the next record,
// 'C' int64(0) int32le(len) varint(packet id) + body. Also written by src/native/export/Dump.cpp.
import { readFileSync } from 'node:fs'

export interface PacketMeta {
  p?: string
  [k: string]: unknown
}

export interface PacketRecord {
  index: number
  id: number
  buffer: Buffer
  meta: PacketMeta | null
}

export function readVarint (buf: Buffer, offset: number): [value: number, next: number] {
  let value = 0
  let shift = 0
  for (;;) {
    if (offset >= buf.length) throw new RangeError(`varint runs past end of buffer at ${offset}`)
    const b = buf[offset++]
    value += (b & 0x7f) * 2 ** shift
    if ((b & 0x80) === 0) return [value, offset]
    shift += 7
    if (shift > 35) throw new RangeError(`varint too long at ${offset}`)
  }
}

function writeVarint (value: number): Buffer {
  const out: number[] = []
  do {
    let b = value % 128
    value = Math.floor(value / 128)
    if (value > 0) b |= 0x80
    out.push(b)
  } while (value > 0)
  return Buffer.from(out)
}

export function readPackets (file: string): PacketRecord[] {
  const buf = readFileSync(file)
  let [len, o] = readVarint(buf, 0)
  o += len
  const records: PacketRecord[] = []
  let meta: PacketMeta | null = null
  while (o < buf.length) {
    const tag = String.fromCharCode(buf[o])
    o += 1 + 8
    if (tag === 'L') {
      ;[len, o] = readVarint(buf, o)
      meta = JSON.parse(buf.toString('utf8', o, o + len)) as PacketMeta
      o += len
    } else if (tag === 'C') {
      len = buf.readInt32LE(o)
      o += 4
      const packet = buf.subarray(o, o + len)
      o += len
      records.push({ index: records.length, id: readVarint(packet, 0)[0], buffer: packet, meta })
      meta = null
    } else {
      throw new Error(`${file}: unknown record tag 0x${buf[o - 9].toString(16)} at offset ${o - 9}`)
    }
  }
  return records
}

export function encodePackets (version: string, records: { buffer: Buffer, meta: PacketMeta | null }[]): Buffer {
  const time = Buffer.alloc(8)
  const parts: Buffer[] = [writeVarint(Buffer.byteLength(version)), Buffer.from(version)]
  for (const r of records) {
    if (r.meta) {
      const json = Buffer.from(JSON.stringify(r.meta))
      parts.push(Buffer.from('L'), time, writeVarint(json.length), json)
    }
    const len = Buffer.alloc(4)
    len.writeInt32LE(r.buffer.length)
    parts.push(Buffer.from('C'), time, len, r.buffer)
  }
  return Buffer.concat(parts)
}
