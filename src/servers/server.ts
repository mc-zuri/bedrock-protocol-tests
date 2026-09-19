import { spawn, spawnSync, type ChildProcess } from 'node:child_process'
import { existsSync, mkdirSync, readFileSync, rmSync, writeFileSync } from 'node:fs'
import { join } from 'node:path'
import { createRequire } from 'node:module'
import { SERVER_PORT_BASE, SERVERS_DIR, versions, type Build } from '../shared/config.ts'

const require = createRequire(import.meta.url)
const START_TIMEOUT_MS = 200_000

export const serverDir = (b: Build) => join(SERVERS_DIR, b.mcDataVersion)
export const serverExe = (b: Build) => join(serverDir(b), 'bedrock_server.exe')

export function properties (b: Build): Record<string, string | number | boolean> {
  const index = versions.findIndex(v => v.mcDataVersion === b.mcDataVersion)
  return {
    'level-name': 'world',
    gamemode: 'survival',
    difficulty: 'easy',
    'allow-cheats': true,
    'online-mode': false,
    'allow-list': false,
    'enable-lan-visibility': false,
    'default-player-permission-level': 'operator',
    'client-side-chunk-generation-enabled': false,
    'player-movement-action-direction-threshold': 0,
    'player-position-acceptance-threshold': 1000,
    'player-rewind-min-correction-delay-ticks': 10000,
    'player-rewind-history-size-ticks': 40,
    // 1.26.50 defaults to nethernet (WebRTC), which bedrock-protocol does not speak
    transport: 'raknet',
    'server-port': SERVER_PORT_BASE + 2 * index,
    'server-portv6': SERVER_PORT_BASE + 2 * index + 1
  }
}

export async function install (b: Build): Promise<boolean> {
  if (existsSync(serverExe(b))) return false
  const dir = serverDir(b)
  mkdirSync(dir, { recursive: true })
  const lock = join(dir, `.bds-download-${b.serverVersion}.lock`)
  writeFileSync(lock, `downloading at ${new Date().toISOString()}`)
  try {
    const { downloadServer } = require('minecraft-bedrock-server')
    await downloadServer(b.serverVersion, { root: dir, path: dir, platform: process.platform === 'win32' ? 'win32' : 'linux' })
  } finally {
    rmSync(lock, { force: true })
  }
  if (!existsSync(serverExe(b))) throw new Error(`${b.serverVersion}: downloaded, but there is no ${serverExe(b)}`)
  return true
}

export function configure (b: Build): boolean {
  const file = join(serverDir(b), 'server.properties')
  if (!existsSync(file)) throw new Error(`no ${file}: is the server installed?`)
  const before = readFileSync(file, 'utf8')
  let content = before
  for (const [key, value] of Object.entries(properties(b))) {
    const line = `${key}=${value}`
    const re = new RegExp(`^${key}=[^\\r\\n]*`, 'm') // keeps the file's line endings
    content = re.test(content) ? content.replace(re, line) : content.replace(/\r?\n?$/, `\n${line}\n`)
  }
  if (content === before) return false
  writeFileSync(file, content)
  return true
}

export interface RunningServer {
  port: number
  stop (): Promise<void>
}

export async function start (b: Build): Promise<RunningServer> {
  if (!existsSync(serverExe(b))) throw new Error(`no ${serverExe(b)} (run pnpm servers)`)
  const port = Number(properties(b)['server-port'])
  const child = spawn(serverExe(b), [], { cwd: serverDir(b), stdio: ['pipe', 'pipe', 'pipe'] })
  child.stdin?.on('error', () => {})
  let tail = ''
  await new Promise<void>((resolve, reject) => {
    const timer = setTimeout(() => {
      kill(child)
      reject(new Error(`did not start within ${START_TIMEOUT_MS / 1000} s`))
    }, START_TIMEOUT_MS)
    child.stdout?.on('data', (data: Buffer) => {
      tail = (tail + data.toString()).slice(-4096)
      if (tail.includes('Server started')) {
        clearTimeout(timer)
        setTimeout(resolve, 500)
      }
    })
    child.once('error', e => {
      clearTimeout(timer)
      reject(e)
    })
    child.once('exit', code => {
      clearTimeout(timer)
      reject(new Error(`exited with code ${code} before it started:\n${tail}`))
    })
  })
  child.stdout?.resume()
  child.stderr?.resume()
  return {
    port,
    stop: () => new Promise<void>(resolve => {
      if (child.exitCode !== null) return resolve()
      if (child.stdin?.writable) child.stdin.write('stop\n')
      const timer = setTimeout(() => {
        kill(child)
        resolve()
      }, 5000)
      child.once('exit', () => {
        clearTimeout(timer)
        resolve()
      })
    })
  }
}

function kill (child: ChildProcess): void {
  if (!child.pid) return
  if (process.platform === 'win32') spawnSync('taskkill', ['/pid', String(child.pid), '/f', '/t'], { stdio: 'ignore' })
  else child.kill('SIGKILL')
}
