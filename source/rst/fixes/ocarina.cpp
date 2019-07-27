#include "common/context.h"
#include "common/debug.h"
#include "common/types.h"
#include "game/context.h"
#include "game/sound.h"

namespace game::ui {
class MessageWindow;
};

namespace rst {

static void EndOcarinaSession(game::ui::MessageWindow* window) {
  const auto set_ocarina_fadeout = util::GetPointer<void(int zero, int duration)>(0x4FE0BC);
  set_ocarina_fadeout(0, 30);

  const auto set_ocarina_mode =
      util::GetPointer<void(game::ui::MessageWindow*, int mode)>(0x1D1A18);
  set_ocarina_mode(window, 1);

  auto* gctx = GetContext().gctx;
  // Disable BGM fadeout
  util::Write(gctx, 0x8422, 1);
  gctx->ocarina_state = game::OcarinaState::StoppedPlaying;
}

static bool IsElegyOfEmptinessAllowed() {
  constexpr u16 allowed_maps[] = {0x58, 0x59, 0x53, 0x1D, 0x56, 0x13, 0x16, 0x18, 0x60, 0x4B, 0x51};
  return util::Contains(allowed_maps, GetContext().gctx->map_maybe);
}

// Returns true to make the caller return, false to continue.
bool HandleOcarinaSong(game::ui::MessageWindow* self, game::OcarinaSong song) {
  // field_42C is MessageWindow's ocarina state
  // 0 inactive
  // 0xd playing
  // 0x12 repeating (step 1; textbox appears)
  // 0x13 repeating (step 2; first note appears)
  // 0x14 repeating (step 3; main)
  // 0x15 repeating (step 4)
  // 0x16 repeating (step 5; you played text appeared)
  // 0x17 repeating (step 6; textbox disappears)
  // 0x18 repeating (step 7; end)

  if (song == game::OcarinaSong::ElegyOfEmptiness) {
    static bool s_played_once = false;
    util::Print("%s: played the Elegy of Emptiness (once=%u)", __func__, s_played_once);
    if (!s_played_once) {
      s_played_once = true;
      return false;
    }

    EndOcarinaSession(self);

    auto* gctx = GetContext().gctx;
    if (IsElegyOfEmptinessAllowed()) {
      game::sound::PlayEffect(0x1000773);
      gctx->ocarina_song = game::OcarinaSong::ElegyOfEmptiness;
      gctx->ocarina_state = game::OcarinaState::PlayingAndReplayDone;
    } else {
      gctx->ShowMessage(0x1B95, 0);  // "Your notes echoed far..."
      gctx->ocarina_state = game::OcarinaState::StoppedPlaying;
      util::GetPointer<void(int)>(0x1D8C5C)(0);
    }

    return true;
  }

  return false;
}

}  // namespace rst

extern "C" {
RST_HOOK bool rst_HandleOcarinaSong(game::ui::MessageWindow* self, game::OcarinaSong song) {
  return rst::HandleOcarinaSong(self, song);
}
}
