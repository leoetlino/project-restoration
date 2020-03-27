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
  template <typename T>
  T* GetState() const {
    return ctx ? static_cast<T*>(ctx->game_state) : nullptr;
  }

  Context* ctx;
  /// The currently opened screen.
  Screen* active_screen = nullptr;
  /// The screen that is soon to be opened and shown.
  Screen* new_screen = nullptr;
  bool open_finished = true;
  bool close_finished = true;
};
static_assert(sizeof(ScreenContext) == 0x14);

// A screen is the top-level component of the UI system.
// It is responsible for orchestrating layouts and handling transitions between game menus.
class Screen {
public:
  virtual ~Screen() = default;
  virtual void Init(ScreenContext&) {}
  virtual void m3() {}
  virtual void PrepareOpen(ScreenContext&) {}
  virtual void m4() {}
  /// Called while opening the screen.
  /// Return true when finished.
  virtual bool CalcOpen(ScreenContext&) { return true; }
  virtual void Open(ScreenContext&) {}
  virtual void Calc(ScreenContext&) {}
  /// Called after game state calc.
  virtual void Draw(ScreenContext&) {}
  virtual void PrepareClose(ScreenContext&) {}
  /// Called while closing the screen.
  /// Return true when finished (typically, after animations have completed).
  virtual bool CalcClose(ScreenContext&) { return true; }
  virtual void Close(ScreenContext&) {}
};
static_assert(sizeof(Screen) == 0x4);

}  // namespace game::ui
