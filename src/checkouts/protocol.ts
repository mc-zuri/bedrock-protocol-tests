import { compileProtocol } from './mcdata.ts'

const dirs = process.argv.slice(2)
for (const dir of dirs.length ? dirs : ['latest']) {
  const version = compileProtocol(dir)
  console.log(`bedrock/${dir}/proto.yml -> bedrock/${version}/protocol.json`)
}
