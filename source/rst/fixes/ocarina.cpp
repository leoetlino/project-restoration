#include <bitset>

#include "common/context.h"
#include "common/debug.h"
#include "common/types.h"
#include "game/context.h"
#include "game/player.h"
#include "game/sound.h"
#include "game/ui/layouts/message_window.h"

namespace rst {

static void EndOcarinaSession(game::ui::MessageWindow* window) {
  auto* gctx = GetContext().gctx;

  constexpr int fade_durations[] = {20, 25, 25, 20, 20};
  const auto set_ocarina_fadeout = util::GetPointer<void(int zero, int duration)>(0x4FE0BC);
  set_ocarina_fadeout(0, fade_durations[u8(gctx->GetPlayerActor()->active_form)]);

  const auto set_ocarina_mode =
      util::GetPointer<void(game::ui::MessageWindow*, int mode)>(0x1D1A18);
  set_ocarina_mode(window, 1);

  // Disable BGM fadeout
  util::Write(gctx, 0x8422, 1);
  gctx->ocarina_state = game::OcarinaState::StoppedPlaying;
}

static bool IsElegyOfEmptinessAllowed() {
  constexpr game::SceneId allowed_scenes[] = {
      game::SceneId::StoneTower,       game::SceneId::StoneTowerInverted,
      game::SceneId::RoadToIkana,      game::SceneId::IkanaCastle,
      game::SceneId::IgosDuIkanaLair,  game::SceneId::IkanaCanyon,
      game::SceneId::StoneTowerTemple, game::SceneId::StoneTowerTempleInverted,
      game::SceneId::SecretShrine,     game::SceneId::BeneathTheWell,
      game::SceneId::GhostHut,
  };
  return util::Contains(allowed_scenes, GetContext().gctx->scene);
}

// Returns true to make the caller return, false to continue.
bool HandleOcarinaSong(game::ui::MessageWindow* self, game::OcarinaSong song) {
  // field_42C is MessageWindow's ocarina state
  // 0 inactive
  // 0x0D playing
  // 0x0E ?
  // 0x0F ?
  // 0x12 repeating (step 1; textbox appears)
  // 0x13 repeating (step 2; first note appears)
  // 0x14 repeating (step 3; main)
  // 0x15 repeating (step 4)
  // 0x16 repeating (step 5; you played text appeared)
  // 0x17 repeating (step 6; textbox disappears)
  // 0x18 repeating (step 7; end)

  static std::bitset<16> s_played_songs;

  const bool played_once = u16(song) < s_played_songs.size() && s_played_songs.test(u16(song));
  if (u16(song) < s_played_songs.size())
    s_played_songs.set(u16(song));

  if (song == game::OcarinaSong::SongOfSoaring) {
    if (!played_once)
      return false;

    EndOcarinaSession(self);
    auto* gctx = GetContext().gctx;
    game::sound::PlayEffect(game::sound::EffectId::NA_SE_SY_TRE_BOX_APPEAR);
    gctx->ocarina_song = song;
    gctx->ocarina_state = game::OcarinaState::PlayingAndReplayDone;
    self->song = u16(song);
    // This flag must always be false; otherwise the Song of Soaring handler will refuse
    // to show the map screen.
    util::Write<bool>(gctx, 0x83EC, false);
    const auto handle_songs = util::GetPointer<void(game::ui::MessageWindow*)>(0x1D78F0);
    handle_songs(self);
    return true;
  }

  if (song == game::OcarinaSong::ElegyOfEmptiness) {
    if (!played_once)
      return false;

    EndOcarinaSession(self);

    auto* gctx = GetContext().gctx;
    if (IsElegyOfEmptinessAllowed()) {
      game::sound::PlayEffect(game::sound::EffectId::NA_SE_SY_TRE_BOX_APPEAR);
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
