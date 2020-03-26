#pragma once

#include "common/types.h"

namespace game {
class State;
}

namespace game::ui {

class Screen;

struct Context {
  u8 gap_0[0x16C];
  State* game_state;
  float time_delta;
  u8 field_174;
};
static_assert(sizeof(Context) == 0x178);

struct ScreenContext {
  virtual ~ScreenContext() = default;
  Context* ctx;
  /// The currently opened screen.
  Screen* active_screen = nullptr;
  /// The screen that is soon to be opened and shown.
  Screen* new_screen = nullptr;
  bool allow_open = true;
  bool allow_close = true;
};
static_assert(sizeof(ScreenContext) == 0x14);

// A screen is the top-level component of the UI system.
// It is responsible for orchestrating layouts and handling transitions between game menus.
class Screen {
public:
  virtual ~Screen() = default;
  virtual void m1();
  virtual void Init(ScreenContext& ctx);
  virtual void m3();
  virtual void PrepareOpen(ScreenContext& ctx);
  virtual bool ShouldOpen(ScreenContext& ctx);
  virtual void Open(ScreenContext& ctx);
  virtual void Calc(ScreenContext& ctx);
  /// Called after game state calc.
  virtual void Draw(ScreenContext& ctx);
  virtual void PrepareClose(ScreenContext& ctx);
  virtual bool ShouldClose(ScreenContext& ctx);
  virtual void Close(ScreenContext& ctx);
};
static_assert(sizeof(Screen) == 0x4);

}  // namespace game::ui
