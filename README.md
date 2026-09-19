# bedrock-protocol-tests

Checks that [bedrock-protocol](https://github.com/PrismarineJS/bedrock-protocol) with
[minecraft-data](https://github.com/PrismarineJS/minecraft-data)'s bedrock protocol can read and write every packet the
real game produces, for every Bedrock Dedicated Server build in `versions.json` (1.16.201 to 1.26.50). Both libraries are
local clones in `checkouts/`, so this is also the place to fix them.

The packets come from the servers themselves:

- **exported** (`src/native`): a DLL injected into each server builds every packet in every shape it can, writes it with
  the game's own `Packet::write`, and keeps it only if the game's own reader reads it back to the same bytes.
- **captured** (`src/capture`): bedrock-protocol joins each server and keeps the first packet of each kind, both
  directions (start_game, item_registry, crafting_data, biome_definition_list, ...). The exporter also reads these back into
  the game, as its `captured:` scenarios.

The validator parses each packet with bedrock-protocol, which must consume every byte, serializes it again, and compares
the result with the game's bytes.

## Requirements

- Windows (the servers are the Windows builds), git
- Node 24+ and pnpm 11
- [xmake](https://xmake.io) with Visual Studio 2022+ (C++ and MASM), for the exporter

## Setup

```sh
pnpm install            # also clones checkouts/node-minecraft-data and checkouts/bedrock-protocol if missing
cp .env.example .env    # optional: server location, output location, ports, parallel servers
```

## Run

| script | does |
|---|---|
| `pnpm servers` | downloads each missing server into `SERVERS_DIR/<version>/` (exactly the build `versions.json` names) and sets its `server.properties` (offline, RakNet, its own ports) |
| `pnpm capture` | joins each server that has no captures yet (`JOBS` at a time) and writes `OUT_DIR/<version>/captured-packets.bin` |
| `pnpm export` | builds `src/native` and exports each server's packets (`JOBS` at a time) to `OUT_DIR/<version>/generated-packets.bin` (needs the captures) |
| `pnpm validate` | validates everything in `OUT_DIR` and writes `OUT_DIR/<version>/report.json` |
| `pnpm validate start_game 52` | only these packets (name or id) |
| `pnpm all` | servers, capture, export, validate |
| `pnpm protocol` | after editing `proto.yml` / `types.yml` in minecraft-data: compiles them into `protocol.json` |

Each step can be limited to builds: `pnpm capture 1.21.0`, `pnpm export 1.21.0 1.26.40`, `pnpm validate -V 1.21.0`.
`pnpm validate --help` lists its options.

A capture is kept until its `captured-packets.bin` is deleted. `pnpm export` prints one line per build: what it exported,
or why not. A build that failed keeps its logs in `out/<version>/.export/` (`bpx.log` names each scenario the game did
not write or read back, `server.log`); a rerun resumes after a scenario that took the server down.

## Fixing the protocol

1. Run `pnpm validate <packet>` for a failing packet. The report lists each failure with the game's bytes, the re-encoded
   bytes, where they differ, and the decoded fields.
2. Fix `checkouts/node-minecraft-data/minecraft-data/data/bedrock/latest/proto.yml` (or `types.yml`), then run
   `pnpm protocol`. Or fix `checkouts/bedrock-protocol`.
3. Run `pnpm validate <packet>` again. Both checkouts are used as they are, with no reinstall.

## Layout

```
versions.json        the builds: minecraft-data version + exact server build
src/servers          download and configure the servers
src/capture          bedrock-protocol join capture
src/native           the exporter: agent/ (bpx_agent.dll, injected) and launcher/ (bpx_launch.exe); packets/defs has
                     one file per packet: its members per build and the values its scenarios use
src/validator        deserialize / serialize / compare
src/checkouts        clone the checkouts, generate data.js, compile the protocol
patches              minecraft-bedrock-server: download the exact build, never another one

out/<version>/            (OUT_DIR)
  captured-packets.bin    pnpm capture: the first packet of each kind of a join, as it went over the wire
  generated-packets.bin   pnpm export: every scenario of every packet the game wrote and read back
  report.json             pnpm validate

Both .bin files are the same format (src/shared/bdt.ts): a version string, then per packet an optional JSON meta
{"p": "<what it is>"} and the packet (varint id + body).
```
