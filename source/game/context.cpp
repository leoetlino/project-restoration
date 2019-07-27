#include "game/context.h"

#include "common/context.h"
#include "game/player.h"
#include "game/static_context.h"

namespace game {

act::Actor* GlobalContext::FindActorWithId(act::Id id, act::Type type) const {
  if (u8(type) >= actors.lists.size())
    return nullptr;

  act::Actor* actor = this->actors.lists[u8(type)].first;
  while (actor && actor->id != id)
    actor = actor->next;
  return actor;
}

act::Player* GlobalContext::GetPlayerActor() const {
  return static_cast<act::Player*>(this->actors.lists[u8(act::Type::Player)].first);
}

act::Actor* GlobalContext::SpawnActor(act::Actor* actor, act::Id id, u16 rx, u16 ry, u16 rz,
                                      u16 param, float pos_x, float pos_y, float pos_z) {
  auto spawn_actor_wrapper =
      rst::util::GetPointer<act::Actor*(ActorLists*, act::Actor*, GlobalContext*, act::Id, u16, u16,
                                        u16, u16 param, float x, float y, float z)>(0x22CAAC);
  return spawn_actor_wrapper(&actors, actor, this, id, rx, ry, rz, param, pos_x, pos_y, pos_z);
}

void GlobalContext::ShowMessage(u16 msgid, int unknown) {
  rst::util::GetPointer<void(GlobalContext*, int msgid, int)>(0x21BAFC)(this, msgid, unknown);
}

namespace {

class Screen;

// Temporary name. Structure is also likely to be very incomplete.
struct UiScreenContext {
  void* vtable;
  void* state;
  Screen* active_screen;
  Screen* new_screen;
  bool allow_open;
  bool allow_close;
};

UiScreenContext& GetUiScreenContext() {
  return *rst::util::GetPointer<UiScreenContext>(0x72F5BC);
}

Screen* GetScreenPointer(UiScreen screen) {
  switch (screen) {
  case UiScreen::Black:
    return *rst::util::GetPointer<Screen*>(0x656B00);
  case UiScreen::Main:
    return *rst::util::GetPointer<Screen*>(0x656B04);
  case UiScreen::Schedule:
    return *rst::util::GetPointer<Screen*>(0x656B08);
  case UiScreen::Quest:
    return *rst::util::GetPointer<Screen*>(0x656B0C);
  case UiScreen::Items:
    return *rst::util::GetPointer<Screen*>(0x656B10);
  case UiScreen::Map:
    return *rst::util::GetPointer<Screen*>(0x656B14);
  case UiScreen::Masks:
    return *rst::util::GetPointer<Screen*>(0x656B18);
  case UiScreen::Ocarina:
    return *rst::util::GetPointer<Screen*>(0x656B1C);
  }
  return nullptr;  // should be unreachable
}

}  // namespace

bool OpenUiScreen(UiScreen screen) {
  GlobalContext* gctx = rst::GetContext().gctx;
  UiScreenContext& ui_ctx = GetUiScreenContext();

  if (!gctx || gctx->type != GameStateType::Play)
    return false;

  ui_ctx.new_screen = GetScreenPointer(screen);
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

bool CheckCurrentUiScreen(UiScreen screen) {
  return GetUiScreenContext().active_screen == GetScreenPointer(screen);
}

}  // namespace game
