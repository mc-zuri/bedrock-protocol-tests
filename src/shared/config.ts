import { readFileSync } from 'node:fs'
import { dirname, isAbsolute, join, resolve } from 'node:path'
import { fileURLToPath } from 'node:url'

export const ROOT = resolve(dirname(fileURLToPath(import.meta.url)), '..', '..')

function dir (name: string, fallback: string): string {
  const value = process.env[name] || fallback
  return isAbsolute(value) ? resolve(value) : resolve(ROOT, value)
}

function int (name: string, fallback: number): number {
  const value = Number(process.env[name] || fallback)
  if (!Number.isInteger(value) || value < 1) throw new Error(`${name} must be a positive integer, not ${process.env[name]}`)
  return value
}

export const SERVERS_DIR = dir('SERVERS_DIR', 'servers')
export const OUT_DIR = dir('OUT_DIR', 'out')
export const capturedFile = (version: string) => join(OUT_DIR, version, 'captured-packets.bin')
export const generatedFile = (version: string) => join(OUT_DIR, version, 'generated-packets.bin')
export const reportFile = (version: string) => join(OUT_DIR, version, 'report.json')
export const SERVER_PORT_BASE = int('SERVER_PORT_BASE', 41032)
export const JOBS = int('JOBS', 4)
export const RELAY_PORT = int('RELAY_PORT', 19132)

export const CHECKOUTS_DIR = join(ROOT, 'checkouts')
export const BEDROCK_PROTOCOL_DIR = join(CHECKOUTS_DIR, 'bedrock-protocol')
export const NODE_MINECRAFT_DATA_DIR = join(CHECKOUTS_DIR, 'node-minecraft-data')
export const MINECRAFT_DATA_DIR = join(NODE_MINECRAFT_DATA_DIR, 'minecraft-data')
export const TOOLS_JS_DIR = join(MINECRAFT_DATA_DIR, 'tools', 'js')

export interface Build {
  mcDataVersion: string
  serverVersion: string
}

export const versions: Build[] = JSON.parse(readFileSync(join(ROOT, 'versions.json'), 'utf8'))

export function compareVersions (a: string, b: string): number {
  const pa = a.split('.').map(Number)
  const pb = b.split('.').map(Number)
  for (let i = 0; i < Math.max(pa.length, pb.length); i++) {
    const d = (pa[i] ?? 0) - (pb[i] ?? 0)
    if (d) return d
  }
  return 0
}

export async function pool<T> (items: T[], jobs: number, work: (item: T) => Promise<void>): Promise<void> {
  let next = 0
  const worker = async () => {
    while (next < items.length) await work(items[next++])
  }
  await Promise.all(Array.from({ length: Math.min(jobs, items.length) }, worker))
}
