#pragma once

#include <array>
#include <type_traits>

#include "common/debug.h"

namespace rst {

// Helper class that makes it easy to manipulate bit flags in a typesafe way.
template <typename FlagType, typename = typename std::enable_if_t<std::is_enum_v<FlagType>, void>>
class Flags {
public:
  constexpr void Set(FlagType v) { flags |= std::underlying_type_t<FlagType>(v); }
  constexpr void Clear(FlagType v) { flags &= ~std::underlying_type_t<FlagType>(v); }

  constexpr bool IsSet(FlagType v) const {
    return (flags & std::underlying_type_t<FlagType>(v)) != 0;
  }

  constexpr bool TestAndClear(FlagType v) {
    if (!IsSet(v))
      return false;
    Clear(v);
    return true;
  }

  template <typename... Flags>
  constexpr bool AreAllSet(Flags... v) const {
    return (... && IsSet(v));
  }

  template <typename... Flags>
  constexpr bool IsOneSet(Flags... v) const {
    return (... || IsSet(v));
  }

  std::underlying_type_t<FlagType> flags = 0;
};

}  // namespace rst
