#include "common/context.h"
#include "common/debug.h"
#include "common/types.h"
#include "common/utils.h"
#include "game/context.h"
#include "game/sound.h"
#include "rst/fixes.h"
#include "rst/fixes/time.h"
#include "rst/link.h"

namespace rst {

namespace {

void Init(Context& context) {
  link::Init();

  util::Print("Project Restoration initialised (" __DATE__ " " __TIME__ ")");
  game::sound::PlayEffect(0x10007DD);
  context.has_initialised = true;
}

// Important: do NOT assume the player actor exists here.
// This is called as soon as a game state is initialised,
// not necessarily when the global context is initialised.
void UpdateContext(game::GlobalContext* gctx) {
  Context& context = GetContext();
  context.gctx = gctx;

  if (!context.has_initialised && gctx->type == game::GameStateType::FirstGame)
    Init(context);
}

}  // anonymous namespace

void Calc(game::GlobalContext* gctx) {
  UpdateContext(gctx);

  if (gctx->type != game::GameStateType::Play)
    return;

  link::HandleFastTransform();
  link::HandleFastOcarina();
  link::HandleFastArrowSwitch();
  FixTime();
  FixTwinmold();
  FixIceArrows();
  FixDekuMovingPlatforms();
  FixBombers();

#if 0
  std::string actors_str = "actors:";
  for (size_t i = 0; i < gctx->actors.lists.size(); ++i) {
    game::ActorList& list = gctx->actors.lists[i];
    actors_str += util::StringFromFormat("\ntype %02zu: ", i);
    for (auto* actor = list.first; actor; actor = actor->next) {
      actors_str += util::StringFromFormat("%04x ", u16(actor->id));
    }
  }
  util::Print(actors_str);
#endif
}

void UiScheduleTriggerHook() {
  auto* gctx = GetContext().gctx;
  if (!gctx || gctx->type != game::GameStateType::Play)
    return;

  if (gctx->pad_state.input.new_buttons.IsSet(game::pad::Button::Start))
    game::OpenUiScreen(game::UiScreen::Items);
}

}  // namespace rst

extern "C" {
RST_HOOK void rst_Calc(game::GlobalContext* gctx) {
  rst::Calc(gctx);
}

RST_HOOK void rst_UiScheduleTriggerHook() {
  rst::UiScheduleTriggerHook();
}

extern char* fake_heap_start;
extern char* fake_heap_end;
extern void (*__init_array_start[])(void) __attribute__((weak));
extern void (*__init_array_end[])(void) __attribute__((weak));

RST_HOOK void _start(void) {
  // Just in case something needs to be dynamically allocated...
  static char s_fake_heap[0x80000];
  fake_heap_start = &s_fake_heap[0];
  fake_heap_end = &s_fake_heap[sizeof(s_fake_heap)];

  for (size_t i = 0; i < size_t(__init_array_end - __init_array_start); i++)
    __init_array_start[i]();
}
}
