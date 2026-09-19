import { execFileSync } from 'node:child_process'
import { existsSync } from 'node:fs'
import { join } from 'node:path'
import { BEDROCK_PROTOCOL_DIR, NODE_MINECRAFT_DATA_DIR } from '../shared/config.ts'

const checkouts = [
  { url: 'https://github.com/PrismarineJS/node-minecraft-data', dir: NODE_MINECRAFT_DATA_DIR, submodules: true },
  { url: 'https://github.com/PrismarineJS/bedrock-protocol', dir: BEDROCK_PROTOCOL_DIR, submodules: false }
]

export function cloneCheckouts (): void {
  for (const { url, dir, submodules } of checkouts) {
    if (!existsSync(dir)) {
      console.log(`cloning ${url} into ${dir}`)
      execFileSync('git', ['clone', ...(submodules ? ['--recurse-submodules'] : []), url, dir], { stdio: 'inherit' })
    } else if (submodules && !existsSync(join(dir, 'minecraft-data', 'data'))) {
      // cloned without --recurse-submodules: its submodule is still empty
      execFileSync('git', ['submodule', 'update', '--init'], { cwd: dir, stdio: 'inherit' })
    }
  }
}
