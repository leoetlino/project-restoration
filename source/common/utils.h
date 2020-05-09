#pragma once

#include <algorithm>
#include <cstring>
#include <tuple>
#include <type_traits>

#include "common/types.h"

namespace rst::util {

constexpr uintptr_t GetAddr(uintptr_t addr) {
  return addr;
}

/// Returns a version-specific address from a list of addresses and casts it to Type*.
template <typename Type>
inline auto GetPointer(uintptr_t addr) {
  return reinterpret_cast<Type*>(GetAddr(addr));
}

template <typename T>
static void InitIfNeeded(T* instance, bool* init_flag, void (*init_fn)(T*)) {
  if (*init_flag)
    return;
  *init_flag = true;
  init_fn(instance);
}

template <class T>
static T& GetInstance(uintptr_t ptr, uintptr_t init_flag, uintptr_t init_fn) {
  T* instance = GetPointer<T>(ptr);
  InitIfNeeded(instance, GetPointer<bool>(init_flag), GetPointer<void(T*)>(init_fn));
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
