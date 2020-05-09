#pragma once

#include <array>
#include <numeric>
#include <type_traits>

#include "common/debug.h"

namespace rst {

// Helper class that makes it easy to manipulate bit flags in a typesafe way.
template <typename FlagType, typename = typename std::enable_if_t<std::is_enum_v<FlagType>, void>>
class Flags {
public:
  constexpr auto& operator=(FlagType v) {
    flags = std::underlying_type_t<FlagType>(v);
    return *this;
  }
  constexpr void Set(FlagType v) { flags |= std::underlying_type_t<FlagType>(v); }
  constexpr void Clear(FlagType v) { flags &= ~std::underlying_type_t<FlagType>(v); }

  constexpr void Set(FlagType v, bool condition) {
    if (condition)
      Set(v);
    else
      Clear(v);
  }

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

  std::underlying_type_t<FlagType> flags;
};

// Super simple alternative to std::bitset with a guaranteed storage layout.
template <size_t N, typename WordType = u32, typename IndexType = WordType>
class BitSet {
public:
  constexpr void Set(IndexType idx) { GetWord(idx) |= 1 << (size_t(idx) % NumBitsPerWord); }
  constexpr void Clear(IndexType idx) { GetWord(idx) &= ~(1 << (size_t(idx) % NumBitsPerWord)); }
  constexpr bool IsSet(IndexType idx) const {
    return (GetWord(idx) & (1 << (size_t(idx) % NumBitsPerWord))) != 0;
  }

  constexpr bool TestAndClear(IndexType idx) {
    if (!IsSet(idx))
      return false;
    Clear(idx);
    return true;
  }

  constexpr WordType GetStorage(size_t idx) const { return m_storage[idx]; }

  constexpr size_t Count() const {
    return std::accumulate(m_storage.begin(), m_storage.end(), 0,
                           [](auto word) { return __builtin_popcount(word); });
  }

private:
  static_assert(std::is_arithmetic_v<WordType>);
  // Yes, not technically correct, but this will work on any sane machine, including the 3DS...
  static constexpr size_t NumBitsPerWord = sizeof(WordType) * 8;
  static constexpr size_t NumWords = (N + NumBitsPerWord - 1) / NumBitsPerWord;
  constexpr WordType& GetWord(IndexType idx) { return m_storage[size_t(idx) / NumBitsPerWord]; }
  constexpr const WordType& GetWord(IndexType idx) const {
    return m_storage[size_t(idx) / NumBitsPerWord];
  }

  std::array<WordType, NumWords> m_storage{};
};

}  // namespace rst
