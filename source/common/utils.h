#pragma once

#include <algorithm>
#include <cstring>
#include <tuple>
#include <type_traits>

#include "common/types.h"

namespace rst::util {

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

/// Returns a version-specific address from a list of addresses and casts it to Type*.
template <typename Type, class... Ts>
constexpr auto GetPointer(Ts... addresses) {
  static_assert(Version < sizeof...(Ts), "Missing address!");
  return reinterpret_cast<Type*>(GetAddr(addresses...));
}

template <typename T>
static void InitIfNeeded(T* instance, bool* init_flag, void (*init_fn)(T*)) {
  if (*init_flag)
    return;
  *init_flag = true;
  init_fn(instance);
}

template <class T, class... A, class... B, class... C>
static T& GetInstance(std::tuple<A...> ptrs, std::tuple<B...> flags, std::tuple<C...> fns) {
  T* instance = std::apply([](auto&&... a) { return GetPointer<T>(a...); }, ptrs);
  InitIfNeeded(instance, std::apply([](auto&&... a) { return GetPointer<bool>(a...); }, flags),
               std::apply([](auto&&... a) { return GetPointer<void(T*)>(a...); }, fns));
  return *instance;
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

template <typename ValueType, typename T>
void Write(T* ptr, size_t offset, const ValueType& value) {
  std::memcpy(reinterpret_cast<u8*>(ptr) + offset, &value, sizeof(value));
}

template <typename First, typename... T>
constexpr inline bool IsAnyOf(const First& first, const T&... t) {
  return ((first == t) || ...);
}

template <class InputIt, class T>
bool Contains(InputIt first, InputIt last, const T& value) {
  return std::find(first, last, value) != last;
}

template <class C, class T>
bool Contains(const C& c, const T& value) {
  return Contains(std::begin(c), std::end(c), value);
}

}  // namespace rst::util
