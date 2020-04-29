#pragma once

#include <cmath>
#include <cstdint>

#include <Eigen/Dense>

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

using Matrix23 = Eigen::Matrix<float, 2, 3, Eigen::RowMajor | Eigen::DontAlign>;
using Matrix34 = Eigen::Matrix<float, 3, 4, Eigen::RowMajor | Eigen::DontAlign>;
using Vec2 = Eigen::Matrix<float, 2, 1, Eigen::DontAlign>;
using Vec4 = Eigen::Matrix<float, 4, 1, Eigen::DontAlign>;

template <typename T = float>
struct TVec3 {
  T x;
  T y;
  T z;

  constexpr bool operator==(const TVec3& rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
  }
  constexpr bool operator!=(const TVec3& rhs) const { return !(*this == rhs); }

  T Distance(const TVec3& other) const {
    const T delta_x = x - other.x;
    const T delta_y = y - other.y;
    const T delta_z = z - other.z;
    return std::sqrt(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
  }
};
using Vec3 = TVec3<float>;
using Vec3s = TVec3<s16>;

// Put hook functions in .init as they are always kept by the linker.
#define RST_HOOK [[gnu::section(".init")]] [[gnu::used]]
