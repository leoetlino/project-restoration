#pragma once

#include <cmath>
#include <cstdint>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;
using size_t = std::size_t;

static_assert(sizeof(u16) == sizeof(short));
static_assert(sizeof(u32) == sizeof(int));

struct Vec3 {
  float x;
  float y;
  float z;

  float Distance(const Vec3& other) const {
    const float delta_x = x - other.x;
    const float delta_y = y - other.y;
    const float delta_z = z - other.z;
    return std::sqrt(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
  }
};
