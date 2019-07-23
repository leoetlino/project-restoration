#pragma once

#include "common/flags.h"
#include "common/types.h"
#include "common/utils.h"

namespace game::pad {

enum class Button : u32 {
  A = 0x1,
  B = 0x2,
  Select = 0x4,
  Start = 0x8,
  Right = 0x10,
  Left = 0x20,
  Up = 0x40,
  Down = 0x80,
  R = 0x100,
  L = 0x200,
  X = 0x400,
  Y = 0x800,
  Debug = 0x1000,
  Gpio14 = 0x2000,
  ZL = 0x4000,
  ZR = 0x8000,
  CStickRight = 0x1000000,
  CStickLeft = 0x2000000,
  CStickUp = 0x4000000,
  CStickDown = 0x8000000,
  MainStickRight = 0x10000000,
  MainStickLeft = 0x20000000,
  MainStickUp = 0x40000000,
  MainStickDown = 0x80000000,
};

struct State {
  struct Input {
    s16 main_stick_x;
    s16 main_stick_y;
    s16 c_stick_x;
    s16 c_stick_y;
    rst::Flags<Button> buttons;
    rst::Flags<Button> new_buttons;
    rst::Flags<Button> released_buttons;
    u8 field_14;
    u8 field_15;
    u8 field_16;
    u8 field_17;
  };
  static_assert(sizeof(Input) == 0x18);

  struct AnalogInput {
    /// Horizontal axis. From -1.0 (left) to 1.0 (right).
    float x;
    /// Vertical axis. From -1.0 (bottom) to 1.0 (top).
    float y;
    /// Horizontal axis. From -60.0 (left) to 60.0 (right).
    float x_raw;
    /// Vertical axis. From -60.0 (bottom) to 60.0 (top).
    float y_raw;
    float x_raw_last;
    float y_raw_last;
  };
  static_assert(sizeof(AnalogInput) == 0x18);

  Input input;
  Input input_last;
  AnalogInput main_stick;
  AnalogInput c_stick;
  u32 field_60;
  /// Buttons, but the value switches between input.buttons and 0 every other frame...
  rst::Flags<Button> field_64;
  /// Buttons, but the value switches between input.buttons and 0 every other frame...
  /// 0 when field_64 is non-zero, and vice versa.
  rst::Flags<Button> field_68;
};
static_assert(sizeof(State) == 0x6c);

enum class TouchscreenButton : u8 {
  I = 1 << 0,
  II = 1 << 1,
  /// Note: does not support holding.
  PictographBox = 1 << 2,
  Ocarina = 1 << 3,
};

struct TouchscreenState {
  rst::Flags<TouchscreenButton> buttons;
  rst::Flags<TouchscreenButton> new_buttons;
};
static_assert(sizeof(TouchscreenState) == 2);

struct ControllerInfo {
  /// state is copied from ControllerMgr to GlobalContext, then to the Player actor
  State* state;
  TouchscreenState* touchscreen;
  /// 0.0-60.0
  float main_stick_strength;
  /// 0x0000 (top) to 0xffff (counterclockwise)
  u16 angle;
  u8 gap_E[22];
  // -1: None, 0: B, 1: Y, 2: X
  u32 touchscreen_btn_slot;
  u32 touchscreen_btn_pressed;
  u8 gap_2C[20];
  u32 field_40;
  u32 field_44;
};

#pragma pack(push, 1)
struct ControllerMgr {
  u8 gap_0[0x1000];
  State state;
  State state2; // unused?
  void* pad_readers;
  u32 field_DC;
  u32 field_E0;
  u32 field_E4;
  u32 field_E8;
  u8 field_EC;
  TouchscreenState touchscreen_state;
  u8 field_EF;
};
#pragma pack(pop)
static_assert(sizeof(ControllerMgr) == 0x10F0);
static_assert(rst::util::OffsetOf(&ControllerMgr::state) == 0x1000);
static_assert(rst::util::OffsetOf(&ControllerMgr::touchscreen_state) == 0x10ED);

ControllerMgr& GetControllerMgr();

}  // namespace game::pad
