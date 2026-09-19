#pragma once

#include <string>

// A probe value landing in a container size can make the game loop for billions of elements;
// a label armed too long ends the process, and the journal records it like a crash.
namespace bpx::watchdog {

void arm(std::string label);
void disarm();

}
