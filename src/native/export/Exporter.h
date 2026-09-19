#pragma once

#include "server/Server.h"

#include <set>
#include <string>

namespace bpx::exporter {

struct Options {
    std::string outFile;       // replaced only once the export is complete
    std::string workDir;       // resume journal, log, result.txt
    std::string serverVersion; // exact build, e.g. 1.21.0.03
    std::set<std::string> only; // empty: all packets
    std::string version; // minecraft-data version, selects the captures
};

void run(server::Server& server, Options const& options);

}
