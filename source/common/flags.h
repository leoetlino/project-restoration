#pragma once

#include <type_traits>

namespace rst {

// Helper class that makes it easy to manipulate bit flags in a typesafe way.
template <typename FlagType, typename = typename std::enable_if_t<std::is_enum_v<FlagType>, void>>
class Flags {
public:
  constexpr auto Set(FlagType v) {
    flags |= std::underlying_type_t<FlagType>(v);
    return *this;
  }
  constexpr auto Clear(FlagType v) {
    flags &= ~std::underlying_type_t<FlagType>(v);
    return *this;
  }

  constexpr bool IsSet(FlagType v) const {
    return (flags & std::underlying_type_t<FlagType>(v)) != 0;
  }

  constexpr bool AreAllSet(FlagType v) const { return IsSet(v); }
  template <typename... Rest>
  constexpr bool AreAllSet(FlagType v, Rest... rest) const {
    return IsSet(v) && AreAllSet(rest...);
  }

  constexpr bool IsOneSet(FlagType v) const { return IsSet(v); }
  template <typename... Rest>
  constexpr bool IsOneSet(FlagType v, Rest... rest) const {
    return IsSet(v) || IsOneSet(rest...);
  }

  std::underlying_type_t<FlagType> flags = 0;
};

}  // namespace rst
