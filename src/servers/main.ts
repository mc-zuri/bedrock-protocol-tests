import { SERVERS_DIR, versions } from '../shared/config.ts'
import { configure, install } from './server.ts'

console.log(`servers: ${SERVERS_DIR}`)
const failed: string[] = []
let downloaded = 0
let configured = 0
for (const b of versions) {
  try {
    if (await install(b)) {
      downloaded++
      console.log(`  ${b.mcDataVersion}: downloaded ${b.serverVersion}`)
    }
    if (configure(b)) {
      configured++
      console.log(`  ${b.mcDataVersion}: server.properties updated`)
    }
  } catch (e) {
    failed.push(b.mcDataVersion)
    console.error(`  ${b.mcDataVersion}: ${e instanceof Error ? e.message : e}`)
  }
}
console.log(`${versions.length - failed.length}/${versions.length} servers ready (${downloaded} downloaded, ${configured} reconfigured)`)
if (failed.length) {
  console.error(`failed: ${failed.join(', ')}`)
  process.exitCode = 1
}
