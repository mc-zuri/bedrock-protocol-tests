#pragma once

#include <format>
#include <string>
#include <string_view>

namespace bpx::log {

void open(std::string const& path);
void line(std::string_view text);

template <class... Args>
void info(std::format_string<Args...> fmt, Args&&... args) {
    line(std::format(fmt, std::forward<Args>(args)...));
}

}
