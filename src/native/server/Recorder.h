#pragma once

#include "server/Server.h"

#include <cstdint>
#include <string>
#include <vector>

// Swaps a BinaryStream's vtable for a copy whose methods go through Thunks.asm, recording each
// writer's doc name (its r8 argument) and the body size at that point. One thread at a time.
namespace bpx::server::recorder {

bool attach(void* stream, std::string const* buffer, std::vector<WriteEvent>* events);

void detach(void* stream);

}
