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

// Super simple alternative to std::bitset with a guaranteed storage layout.
template <size_t N, typename WordType = u32>
class BitSet {
public:
  constexpr void Set(size_t idx) { GetWord(idx) |= 1 << (idx % NumBitsPerWord); }
  constexpr void Clear(size_t idx) { GetWord(idx) &= ~(1 << (idx % NumBitsPerWord)); }
  constexpr bool IsSet(size_t idx) const {
    return (GetWord(idx) & (1 << (idx % NumBitsPerWord))) != 0;
  }

  constexpr bool TestAndClear(FlagType v) {
    if (!IsSet(v))
      return false;
    Clear(v);
    return true;
  }

private:
  static_assert(std::is_arithmetic_v<WordType>);
  // Yes, not technically correct, but this will work on any sane machine, including the 3DS...
  static constexpr size_t NumBitsPerWord = sizeof(WordType) * 8;
  static constexpr size_t NumWords = (N + NumBitsPerWord - 1) / NumBitsPerWord;
  constexpr Word& GetWord(size_t idx) { return m_storage[idx / NumBitsPerWord]; }
  std::array<Word, NumWords> m_storage{};
};

}  // namespace rst
