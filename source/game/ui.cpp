#include "game/ui.h"

#include "common/context.h"
#include "common/utils.h"
#include "game/context.h"
#include "game/static_context.h"

namespace game::ui {

// Temporary name. Structure is also likely to be very incomplete.
struct ScreenContext {
  void* vtable;
  void* state;
  Screen* active_screen;
  Screen* new_screen;
  bool allow_open;
  bool allow_close;
};

ScreenContext& GetScreenContext() {
  return *rst::util::GetPointer<ScreenContext>(0x72F5BC);
}

Screen* GetScreen(ScreenType screen) {
  switch (screen) {
  case ScreenType::Black:
    return *rst::util::GetPointer<Screen*>(0x656B00);
  case ScreenType::Main:
    return *rst::util::GetPointer<Screen*>(0x656B04);
  case ScreenType::Schedule:
    return *rst::util::GetPointer<Screen*>(0x656B08);
  case ScreenType::Quest:
    return *rst::util::GetPointer<Screen*>(0x656B0C);
  case ScreenType::Items:
    return *rst::util::GetPointer<Screen*>(0x656B10);
  case ScreenType::Map:
    return *rst::util::GetPointer<Screen*>(0x656B14);
  case ScreenType::Masks:
    return *rst::util::GetPointer<Screen*>(0x656B18);
  case ScreenType::Ocarina:
    return *rst::util::GetPointer<Screen*>(0x656B1C);
  }
  return nullptr;  // should be unreachable
}

bool OpenScreen(ScreenType screen) {
  GlobalContext* gctx = rst::GetContext().gctx;
  ScreenContext& ui_ctx = GetScreenContext();

  if (!gctx || gctx->type != StateType::Play)
    return false;

  ui_ctx.new_screen = GetScreen(screen);
  gctx->ui_menu_state = UiMenuState::Opening;
  gctx->field_CCAC = false;

  if (GetStaticContext().field_D38) {
    GetStaticContext().field_D38 = 0;
    gctx->field_CAB0 = 0xA0A0A000;
  }
  if (!gctx->field_CAAD)
    gctx->field_CAB0 = 0;

  if (gctx->some_fn2 && !gctx->some_fn2(&gctx->some_ptr) && gctx->some_fn1)
    gctx->some_fn1(&gctx->some_ptr, 0);

  return true;
}

bool CheckCurrentScreen(ScreenType screen) {
  return GetScreenContext().active_screen == GetScreen(screen);
}

void UpdateLayout(Layout* layout) {
  rst::util::GetPointer<void(Layout*)>(0x161AE8)(layout);
}

}  // namespace game::ui
