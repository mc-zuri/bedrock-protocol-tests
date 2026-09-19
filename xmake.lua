add_rules("mode.release", "mode.debug")
set_defaultmode("release")
set_languages("c++20")
set_runtimes("MD") -- std types handed to the game are freed by its shared-CRT heap

target("bpx_agent")
    set_kind("shared")
    add_files("src/native/core/*.cpp", "src/native/memory/*.cpp", "src/native/bindings/*.cpp",
              "src/native/server/*.cpp", "src/native/server/*.asm", "src/native/schema/*.cpp", "src/native/packets/*.cpp",
              "src/native/packets/defs/*.cpp", "src/native/export/*.cpp", "src/native/agent/*.cpp")
    add_includedirs("src/native")
    set_exceptions("none") -- /EHa below: SEH-aware C++ exceptions (memory/Guard)
    add_cxflags("/EHa", "/utf-8", "/W4", "/bigobj", "/permissive-")
    add_defines("NOMINMAX", "WIN32_LEAN_AND_MEAN", "_CRT_SECURE_NO_WARNINGS")

target("bpx_launch")
    set_kind("binary")
    add_files("src/native/launcher/*.cpp")
    add_cxflags("/EHsc", "/utf-8", "/W4", "/permissive-")
    add_defines("NOMINMAX", "_CRT_SECURE_NO_WARNINGS")
