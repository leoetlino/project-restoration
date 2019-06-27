#pragma once

#include <array>
#include <string_view>

namespace rst::util {

/// Prints a debug message using svcOutputDebugString.
void Print(std::string_view string);
/// Prints a debug message using svcOutputDebugString.
__attribute__((format(printf, 1, 2))) void Print(const char* format, ...);

/// Returns the in-game time as a null-terminated HH:MM string.
std::array<char, 6> TimeToString();

}  // namespace rst::util
