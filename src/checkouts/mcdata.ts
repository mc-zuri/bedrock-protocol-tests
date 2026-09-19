import { execFileSync } from 'node:child_process'
import { existsSync, readFileSync } from 'node:fs'
import { createRequire } from 'node:module'
import { join } from 'node:path'
import { BEDROCK_PROTOCOL_DIR, NODE_MINECRAFT_DATA_DIR, ROOT, TOOLS_JS_DIR } from '../shared/config.ts'

const require = createRequire(import.meta.url)

export function checkCheckouts (): void {
  for (const [what, dir, file] of [
    ['bedrock-protocol', BEDROCK_PROTOCOL_DIR, 'src/transforms/serializer.js'],
    ['node-minecraft-data', NODE_MINECRAFT_DATA_DIR, 'minecraft-data/data/dataPaths.json']
  ] as const) {
    if (!existsSync(join(dir, file))) throw new Error(`no ${what} checkout at ${dir} (missing ${file}): run pnpm install`)
  }
  // linked checkouts get no node_modules: bedrock-protocol's dependencies must resolve from this package
  const ours = JSON.parse(readFileSync(join(ROOT, 'package.json'), 'utf8')).dependencies ?? {}
  const theirs = JSON.parse(readFileSync(join(BEDROCK_PROTOCOL_DIR, 'package.json'), 'utf8')).dependencies ?? {}
  const missing = Object.keys(theirs).filter(d => !(d in ours))
  if (missing.length) throw new Error(`bedrock-protocol needs ${missing.join(', ')}: add ${missing.length > 1 ? 'them' : 'it'} to package.json dependencies`)
}

export function generateDataJs (): void {
  execFileSync(process.execPath, [join(NODE_MINECRAFT_DATA_DIR, 'bin', 'generate_data.js')], { cwd: NODE_MINECRAFT_DATA_DIR, stdio: 'inherit' })
}

export function compileProtocol (dir = 'latest'): string {
  // compileProtocol.js chdirs into the proto dir
  const cwd = process.cwd()
  try {
    const { convert } = require(join(TOOLS_JS_DIR, 'compileProtocol.js')) as { convert (edition: string, ver: string): string }
    return convert('bedrock', dir)
  } finally {
    process.chdir(cwd)
  }
}
