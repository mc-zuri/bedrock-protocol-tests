import { createRequire } from 'node:module'
import type { PacketRecord } from '../shared/bdt.ts'

const require = createRequire(import.meta.url)

export interface CompiledProtocol {
  parsePacketBuffer (type: string, buffer: Buffer, offset?: number): { data: any, metadata: { size: number } }
  createPacketBuffer (type: string, packet: unknown): Buffer
  setVariable (name: string, value: unknown): void
}

function mcData (): any {
  return require('minecraft-data')
}

export function hasProtocol (version: string): boolean {
  try {
    return Boolean(mcData()(`bedrock_${version}`)?.protocol)
  } catch {
    return false
  }
}

export function dumpShieldItemId (proto: CompiledProtocol, packetNames: Map<number, string>, records: PacketRecord[]): number | undefined {
  for (const r of records) {
    const name = packetNames.get(r.id)
    if (name !== 'item_registry' && name !== 'start_game') continue
    try {
      const { data } = proto.parsePacketBuffer('mcpe_packet', r.buffer)
      const shield = data.params.itemstates?.find((i: { name: string }) => i.name === 'minecraft:shield')
      if (shield) return shield.runtime_id
    } catch {}
  }
  return undefined
}

export function shieldItemId (version: string): number | undefined {
  try {
    return mcData()(`bedrock_${version}`)?.itemsByName?.shield?.id
  } catch {
    return undefined
  }
}

export function loadProtocol (version: string): { proto: CompiledProtocol, packetNames: Map<number, string> } {
  const { createProtocol } = require('bedrock-protocol/src/transforms/serializer.js')
  const proto = createProtocol(version) as CompiledProtocol
  const mappings: Record<string, string> = mcData()(`bedrock_${version}`).protocol.types.mcpe_packet[1][0].type[1].mappings
  const packetNames = new Map(Object.entries(mappings).map(([id, name]) => [Number(id), name]))
  return { proto, packetNames }
}
