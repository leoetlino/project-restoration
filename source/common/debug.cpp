#include "common/debug.h"

#include "game/common_data.h"

namespace rst::util {

extern "C" {
int svcOutputDebugString(const char* string, int length);
}

void Print(std::string_view string) {
  svcOutputDebugString(string.data(), string.size());
}

void Print(const char* format, ...) {
  char buffer[0x200];
  va_list arg;
  va_start(arg, format);
  const int written = std::vsnprintf(buffer, sizeof(buffer), format, arg);
  va_end(arg);
  if (written >= 0)
    Print(std::string_view{buffer, size_t(written)});
}

std::array<char, 6> TimeToString() {
  const u16 game_time = game::GetCommonData().save.time;

  const float day_percentage = float(game_time) / 0x10000;
  const int hours = 24 * day_percentage;
  const int minutes = 60 * ((24 * day_percentage) - hours);

  std::array<char, 6> output;
  std::snprintf(output.data(), output.size(), "%02d:%02d", hours, minutes);
  return output;
}
}
