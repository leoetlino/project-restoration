#pragma once

#include <cstring>
#include <tuple>
#include <type_traits>

#include "common/types.h"

namespace rst::util {

namespace detail {
#if defined(RST_VER)
constexpr u32 Version = RST_VER;
#else
constexpr u32 Version = 0;
#endif
static_assert(Version == 0 || Version == 1, "Unknown version");

template <class... Ts, typename std::enable_if_t<(Version < sizeof...(Ts))>* = nullptr>
constexpr uintptr_t GetAddr(Ts... addresses) {
  return std::get<Version>(std::forward_as_tuple(addresses...));
}

template <class... Ts>
constexpr uintptr_t GetAddr(Ts...) {
  return 0;
}
}  // namespace detail

/// Returns a version-specific address from a list of addresses and casts it to Type*.
template <typename Type, class... Ts>
constexpr auto GetPointer(Ts... addresses) {
  static_assert(detail::Version < sizeof...(Ts), "Missing address!");
  return reinterpret_cast<Type*>(detail::GetAddr(addresses...));
}

/// Returns the offset in bytes of a member.
/// Unlike offsetof, this works for derived classes as well.
template <typename T1, typename T2>
inline size_t constexpr OffsetOf(T1 T2::*member) {
  constexpr T2 object{};
  return size_t(&(object.*member)) - size_t(&object);
}

template <typename Dest, typename T>
Dest BitCastPtr(const T* ptr, size_t offset = 0) {
  Dest dest;
  std::memcpy(&dest, reinterpret_cast<const u8*>(ptr) + offset, sizeof(dest));
  return dest;
}

}  // namespace rst::util
