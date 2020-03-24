// File formats for the UI system.

#pragma once

#include "common/span.h"
#include "common/types.h"

namespace game::ui {

enum class WidgetValueType : u8 {
  TranslateX = 0,
  TranslateY = 1,
  TranslateZ = 2,
  ScaleX = 3,
  ScaleY = 4,
  ScaleZ = 5,
  RotateX = 6,
  RotateY = 7,
  RotateZ = 8,
  Visible = 9,
  Field24_X = 10,
  Field24_Y = 11,
  Field2C_X = 12,
  Field2C_Y = 13,
  Field34 = 14,
  ColorR = 15,
  ColorG = 16,
  ColorB = 17,
  ColorA = 18,
  Unk = 19,
};

enum class AnimEntryType : u8 {
  Interpolate = 0,
  Set = 1,
  Add = 2,
  AddPositive = 3,
};

enum class AnimKeyframeType : u8 {
  Nop = 0,
  Lerp = 1,
  Type2 = 2,
  Type2R = 3,
  SetToZero = 4,
};

struct ResAnimKeyframe {
  AnimKeyframeType GetType() const { return AnimKeyframeType(flags & 0xf); }
  float GetValue(WidgetValueType value_type) const;
  /// Note: do not use this for widget value type Visible (9) or Unk (19).
  void SetFloat(float value);

  u32 frame;
  u16 flags;
  u32 raw_value;
};
static_assert(sizeof(ResAnimKeyframe) == 0xc);

struct ResAnimEntry {
  AnimEntryType GetType() const { return AnimEntryType(flags & 3); }

  /// Only valid for type 0!
  tcb::span<ResAnimKeyframe> GetKeyframes() {
    return {reinterpret_cast<ResAnimKeyframe*>(this + 1), num_keyframes};
  }

  /// Only valid for type 1, 2 and 3!
  tcb::span<float> GetFrameValues() {
    return {reinterpret_cast<float*>(this + 1), max_frame_idx + 1};
  }

  /// Index of the widget this animation entry applies to.
  u16 widget_idx;
  /// Type of the widget value to animate.
  WidgetValueType value_type;
  /// Number of keyframes (for type 0 / interpolation).
  u16 num_keyframes;
  /// Flags (& 3 to get the type)
  u16 flags;
  /// Maximum frame index (for type 1, 2 and 3).
  u32 max_frame_idx;
};
static_assert(sizeof(ResAnimEntry) == 0xc);

}  // namespace game::ui
