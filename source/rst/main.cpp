#include "common/context.h"
#include "common/debug.h"
#include "common/types.h"
#include "common/utils.h"
#include "game/camera.h"
#include "game/common_data.h"
#include "game/context.h"
#include "game/pad.h"
#include "game/player.h"
#include "game/sound.h"
#include "game/states/state.h"
#include "game/ui.h"
#include "game/ui/screens/gear_screen.h"
#include "game/ui/screens/schedule_screen.h"
#include "rst/fixes.h"
#include "rst/fixes/boss.h"
#include "rst/fixes/time.h"
#include "rst/link.h"

namespace rst {

namespace {

void Init(Context& context) {
  link::Init();

  util::Print("Project Restoration initialised (" __DATE__ " " __TIME__ ")");
  game::sound::PlayEffect(game::sound::EffectId::NA_SE_SY_QUEST_CLEAR);
  context.has_initialised = true;
}

}  // anonymous namespace

[[maybe_unused]] static void PrintDebug(game::GlobalContext* gctx) {
  std::string actors_str = "actors:";
  for (size_t i = 0; i < gctx->actors.lists.size(); ++i) {
    game::ActorList& list = gctx->actors.lists[i];
    actors_str += util::StringFromFormat("\ntype %02zu: ", i);
    for (auto* actor = list.first; actor; actor = actor->next) {
      actors_str += util::StringFromFormat("%04x[%04x] ", u16(actor->id), actor->params);
    }
  }
  util::Print(actors_str);

  const auto* player = gctx->GetPlayerActor();
  if (player) {
    util::Print("%08lx %08lx %08lx %08lx %08x", player->flags1.flags, player->flags2.flags,
                player->flags3.flags, player->flags4, player->flags_94.flags);

    util::Print("state handler: %p - 92C: %u - 92D: %u", player->state_handler_fn, player->fn1_idx,
                player->fn2_idx);

    auto* camera = &gctx->main_camera + gctx->camera_idx;
    util::Print("camera_idx=%u target=%p%s state=%04x mode=%04x", gctx->camera_idx,
                camera->target_actor,
                (camera->target_actor && camera->target_actor == player) ? "(player)" : "",
                u16(camera->state), u16(camera->mode));
  }
}

static bool IsStartOrSelectPressed() {
  return GetContext().gctx && GetContext().gctx->pad_state.input.new_buttons.IsOneSet(
                                  game::pad::Button::Start, game::pad::Button::Select);
}

static void UiGearScreenUpdate() {
  if (!game::ui::CheckCurrentScreen(game::ui::ScreenType::Gear))
    return;
  auto* screen =
      static_cast<game::ui::GearScreen*>(game::ui::GetScreen(game::ui::ScreenType::Gear));
  if (IsStartOrSelectPressed()) {
    screen->pressed_btn_option = 1;
  }
}

static void UiOcarinaScreenUpdate() {
  if (!game::ui::CheckCurrentScreen(game::ui::ScreenType::Ocarina))
    return;
  auto* screen = game::ui::GetScreen(game::ui::ScreenType::Ocarina);

  if (IsStartOrSelectPressed()) {
    util::Write<bool>(screen, 0x41, true);  // is transitioning
    auto* layout_ocarina = util::BitCastPtr<game::ui::Layout*>(screen, 0x10);
    auto* layout_music_list = util::BitCastPtr<game::ui::Layout*>(screen, 0x14);
    const bool is_menu_ocarina = util::BitCastPtr<bool>(screen, 0x40);
    if (is_menu_ocarina)
      util::GetPointer<void(void*)>(0x5F54DC)(layout_ocarina);
    else
      util::GetPointer<void(void*)>(0x5F7DC8)(layout_music_list);
    game::sound::PlayEffect(game::sound::EffectId::NA_SE_SY_DECIDE);
  }
}

static void UiScheduleScreenUpdate() {
  if (!game::ui::CheckCurrentScreen(game::ui::ScreenType::Schedule))
    return;
  auto* screen =
      static_cast<game::ui::ScheduleScreen*>(game::ui::GetScreen(game::ui::ScreenType::Schedule));

  // Allow several touch buttons to be activated with physical button presses.
  // Conveniently enough, it looks like this is already partially implemented by Grezzo,
  // so all we have to do is copy over the pad button flags.
  const auto& input = rst::GetContext().gctx->pad_state.input;
  const auto copy_btn_flag = [&input, screen](game::pad::Button btn) {
    if (input.new_buttons.IsSet(btn))
      screen->btn_press_flags |= u32(btn);
  };
  copy_btn_flag(game::pad::Button::X);
  copy_btn_flag(game::pad::Button::Y);

  if (input.new_buttons.IsSet(game::pad::Button::Select) &&
      screen->btn_map_anim_player->GetAnim() == screen->disp_btn_map_anim) {
    auto& ctx = game::ui::GetScreenContext();
    auto& handler = screen->GetTouchHandler();
    for (auto* btn : {screen->btn_map_l, screen->base1_btn_map_l}) {
      handler.UpdateState(btn, ctx, 0, 0);
      handler.OnActivate(btn, false, ctx, 0, 0);
      handler.OnRelease(btn, false, false, ctx, 0, 0);
    }
  }
}

RST_HOOK void Calc(game::State* state) {
  Context& context = GetContext();
  context.gctx = nullptr;

  if (!context.has_initialised && state->type == game::StateType::FirstGame)
    Init(context);

  if (state->type != game::StateType::Play)
    return;

  context.gctx = static_cast<game::GlobalContext*>(state);
  game::CalcCamera();
  link::Calc();
  FixTime();
  FixIceArrows();
  FixDekuMovingPlatforms();
  FixBombers();
  FixHintStone();
  FixFreeCameraReset();

  UiGearScreenUpdate();
  UiOcarinaScreenUpdate();
  UiScheduleScreenUpdate();

  if (false)
    PrintDebug(context.gctx);
}

RST_HOOK void PreActorCalcHook() {
  FixOwlStatueActivationTrigger();
}

RST_HOOK void PostActorCalcHook() {
  FixBosses();
}

RST_HOOK void UiScheduleTriggerHook() {
  auto* gctx = GetContext().gctx;
  if (!gctx || gctx->type != game::StateType::Play)
    return;

  const bool zr = gctx->pad_state.input.buttons.IsSet(game::pad::Button::ZR);
  const bool start = gctx->pad_state.input.new_buttons.IsSet(game::pad::Button::Start);
  const bool select = gctx->pad_state.input.new_buttons.IsSet(game::pad::Button::Select);
  if (!zr && select)
    game::ui::OpenScreen(game::ui::ScreenType::Items);
  if (!zr && start && game::GetCommonData().save.inventory.collect_register & 0x40000)
    game::ui::OpenScreen(game::ui::ScreenType::Schedule);
  if (zr && start)
    game::ui::OpenScreen(game::ui::ScreenType::Quest);
  if (zr && select) {
    // Clear map screen type. (Needed because the screen could be in "soaring" mode.)
    util::Write<u8>(game::ui::GetScreen(game::ui::ScreenType::Map), 0x78E, 0);
    game::ui::OpenScreen(game::ui::ScreenType::Map);
    gctx->pad_state.input.buttons.Clear(game::pad::Button::Select);
    gctx->pad_state.input.new_buttons.Clear(game::pad::Button::Select);
  }
}

}  // namespace rst

extern "C" {
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
