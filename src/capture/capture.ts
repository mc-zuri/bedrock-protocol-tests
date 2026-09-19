import { createClient, type Version } from 'bedrock-protocol'
import { createRequire } from 'node:module'

const require = createRequire(import.meta.url)

const TIMEOUT_MS = 120_000
// old builds never report the spawn to bedrock-protocol
const SPAWN_WAIT_MS = 30_000
const LINGER_MS = 5_000

const REQUIRED: Record<number, string> = {
  11: 'start_game',
  29: 'update_attributes',
  52: 'crafting_data',
  119: 'available_entity_identifiers',
  122: 'biome_definition_list'
}
const ITEM_REGISTRY = 162

// from 1.26.43 the server silently refuses bedrock-protocol's default DeviceOS 7 (Win10); 8 (Win32) works everywhere
const skinData = { DeviceOS: 8 }

export function packetId (raw: Buffer): number {
  let value = 0
  for (let i = 0, shift = 0; i < raw.length && i < 5; i++, shift += 7) {
    value |= (raw[i] & 0x7f) << shift
    if (!(raw[i] & 0x80)) break
  }
  return value & 0x3ff
}

function protocolPacketNames (version: string): Record<string, string> {
  try {
    return require('minecraft-data')(`bedrock_${version}`).protocol.types.mcpe_packet[1][0].type[1].mappings
  } catch {
    return {}
  }
}

export interface Captured {
  packets: Map<string, Buffer>
  decodeFailures: string[]
}

export function capture (version: string, withItemRegistry: boolean, port: number): Promise<Captured> {
  const required = new Set(Object.keys(REQUIRED).map(Number))
  if (withItemRegistry) required.add(ITEM_REGISTRY)

  return new Promise((resolve, reject) => {
    const client = createClient({ host: '127.0.0.1', port, version: version as Version, username: 'bpt', offline: true, skipPing: true, skinData } as any)
    const raw = new Map<number, Buffer>()
    const names = new Map<number, string>()
    const decodeFailures = new Set<string>()
    const keep = (buffer: Buffer) => {
      const id = packetId(buffer)
      if (!raw.has(id)) raw.set(id, Buffer.from(buffer))
      required.delete(id)
      if (required.size === 0) requiredDone()
      return id
    }

    // not in bedrock-protocol's typings; raw bytes are kept before decoding so undecodable packets are captured too
    const conn = client as unknown as {
      readPacket (raw: Buffer): void
      serializer: { createPacketBuffer (packet: { name: string, params: unknown }): Buffer }
    }
    const readPacket = conn.readPacket.bind(client)
    let reading = -1 // 'packet' is emitted synchronously inside readPacket
    conn.readPacket = (buffer: Buffer) => {
      reading = keep(buffer)
      try {
        return readPacket(buffer)
      } finally {
        reading = -1
      }
    }
    const createPacketBuffer = conn.serializer.createPacketBuffer.bind(conn.serializer)
    conn.serializer.createPacketBuffer = packet => {
      const buffer = createPacketBuffer(packet)
      const id = keep(buffer)
      if (!names.has(id)) names.set(id, packet.name)
      return buffer
    }
    client.on('packet', ({ data: { name } }: any) => {
      if (reading >= 0 && !names.has(reading)) names.set(reading, name)
    })
    client.on('error', (err: unknown) => {
      const e = err instanceof Error ? err : new Error(String(err))
      const where = /at Object\.(packet_\w+)/.exec(e.stack ?? '')?.[1] ?? 'unknown'
      decodeFailures.add(`${where}: ${e.message.split('\n')[0]}`)
    })

    let settled = false
    const close = () => {
      // close() removes every listener, but queued packets can still emit 'error', which would crash the process
      client.close()
      client.on('error', () => {})
    }
    const timer = setTimeout(() => {
      settled = true
      close()
      reject(new Error(`timed out; never received: ${[...required].map(id => REQUIRED[id] ?? `id ${id}`).join(', ')}`))
    }, TIMEOUT_MS)
    const finish = () => {
      if (settled) return
      settled = true
      clearTimeout(timer)
      close()
      const known = protocolPacketNames(version)
      const packets = new Map<string, Buffer>()
      for (const [id, buffer] of raw) packets.set(names.get(id) ?? known[id] ?? `id_${id}`, buffer)
      resolve({ packets, decodeFailures: [...decodeFailures] })
    }

    let spawned = false
    let lingering = false
    let done = false
    const linger = () => {
      if (lingering) return
      lingering = true
      setTimeout(finish, LINGER_MS)
    }
    function requiredDone () {
      if (done) return
      done = true
      if (spawned) linger()
      else setTimeout(linger, SPAWN_WAIT_MS)
    }
    client.on('spawn', () => {
      spawned = true
      if (done) linger()
    })
  })
}
