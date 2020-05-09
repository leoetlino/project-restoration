#include "rst/fixes/time.h"

#include <unordered_map>

#include "common/context.h"
#include "common/debug.h"
#include "game/common_data.h"
#include "game/context.h"
#include "game/static_context.h"

namespace rst {

static constexpr u16 HhmmToTime(int hours, int minutes, int seconds = 0) {
  return 0x10000 * (3600.0 * hours + 60.0 * minutes + seconds) / (60 * 60 * 24);
}

static bool ShouldSlowTime() {
  const game::GlobalContext& gctx = *GetContext().gctx;
  const game::CommonData& cdata = game::GetCommonData();

  // Require using the Inverted Song of Time
  if (cdata.save.extra_time_speed != -1)
    return false;

  // Don't slow time to 1/3 speed if the Old Lady is in North Clock Town.
  // The cutscene is already slow enough on 1/2 speed!
  if ((cdata.sub1.entrance & 0xff00) == 0xD600 &&
      gctx.FindActorWithId(game::act::Id::NpcOldLady, game::act::Type::Npc)) {
    return false;
  }

  // Romani Ranch, alien shooting game
  if ((cdata.sub1.entrance & 0xff00) == 0x6400 && cdata.save.day == 1 &&
      HhmmToTime(2, 0) <= cdata.save.time && cdata.save.time <= HhmmToTime(5, 15)) {
    return false;
  }

  // Kafei
  if (gctx.IsActorVisible(game::act::Id::NpcKafei, game::act::Type::Npc)) {
    return false;
  }

  return true;
}

struct NpcSchedulePosition {
  float last_value = 0.0;
  int ticks_since_last_update = 0;
};

static auto& GetNpcSchedulePositions() {
  static std::unordered_map<float*, NpcSchedulePosition> s_positions;
  return s_positions;
}

float MoveScheduledNpcHook(float* schedule_position, float speed) {
  auto& entry = GetNpcSchedulePositions()[schedule_position];
  if (entry.last_value < *schedule_position || *schedule_position == 0.0)
    entry.last_value = *schedule_position;
  entry.ticks_since_last_update = 0;

  const game::CommonData& cdata = game::GetCommonData();
  const game::StaticContext& sctx = game::GetStaticContext();

  // Ensure scheduled NPCs don't go too fast.
  //
  // Most NPC's walking speeds are only based on the time speed which are both integers,
  // so we need to apply the slow time multiplier manually.
  //
  // Using a PWM style trick for the time speed does not work well and causes
  // flickering NPCs as they switch between different walking speeds too quickly.
  const bool uses_custom_speed = (sctx.time_speed + cdata.save.extra_time_speed) != speed;
  constexpr float SlowTimeMultiplier = 1.0 / 3.0;
  return (!uses_custom_speed && ShouldSlowTime()) ? (sctx.time_speed * SlowTimeMultiplier) : speed;
}

void FixTime() {
  if (GetContext().gctx->ocarina_state == game::OcarinaState::GoingBackInTime) {
    GetContext().going_back_in_time = true;
  }

  for (auto it = GetNpcSchedulePositions().begin(); it != GetNpcSchedulePositions().end();) {
    auto& [ptr, entry] = *it;

    if (entry.ticks_since_last_update >= 60) {
      // If an entry has received no update for 60 ticks, stop keeping track of it.
      // The actor instance was probably freed.
      // Otherwise, assume that the pointer still points to actual NPC instance data
      // and hope we don't overwrite anything important.
      // Yes, this is absolutely horrible.
      util::Print("%s: forgetting %p", __func__, ptr);
      it = GetNpcSchedulePositions().erase(it);
      continue;
    }

    // For some reason, Nintendo/Grezzo truncates the schedule position variable
    // every time it is supposed to stop updating (e.g. when time is frozen
    // for the postman actor).
    //
    // This would normally not be an issue -- as the variable only stores integer values --
    // but causes NPCs to turn around and walk in the wrong direction every time a loading zone
    // is entered when the fixed Inverted Song of Time is active, as that requires using
    // non-integer speed values.
    //
    // The fix is to try to detect position truncation and restore the previous value.
    const bool was_truncated = *ptr != entry.last_value && *ptr == int(entry.last_value);
    if (was_truncated) {
      util::Print("%s: detected truncation for %p, restoring %f", __func__, ptr, entry.last_value);
      *ptr = entry.last_value;
    }

    ++entry.ticks_since_last_update;
    ++it;
  }
}

RST_HOOK void UpdateTimeHook() {
  game::CommonData& cdata = game::GetCommonData();

  // Restore the effectiveness of the Inverted Song of Time.
  //
  // In MM, the normal time speed is +3 and the ISoT sets the extra time speed to -2, resulting
  // in a +1 effective time speed (which means 1/3 time speed).
  //
  // In MM3D, the normal speed is +2 and the ISoT only sets the extra speed to -1, which still
  // gives the player a +1 effective speed, but only 1/2 time speed.
  //
  // A quick fix is to decrement the in-game time every 3 frames,
  // giving us the desired ratio of 1/3 = (1+1)/(2+2+2).

  if (ShouldSlowTime() && GetContext().gctx->frame_counter % 3 == 0) {
    cdata.save.time -= 1;
  }
  cdata.time_copy = cdata.save.time;
}

}  // namespace rst

extern "C" {
RST_HOOK float rst_MoveScheduledNpcHook(u32, float*, float* schedule_position, u32, u32, u32*,
                                        Vec3*, u32, float speed) {
  return rst::MoveScheduledNpcHook(schedule_position, speed);
}
}
