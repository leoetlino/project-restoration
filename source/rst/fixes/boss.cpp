#include "rst/fixes/boss.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <optional>
#include <type_traits>

#include "common/context.h"
#include "common/debug.h"
#include "common/types.h"
#include "game/actor.h"
#include "game/actors/boss_goht.h"
#include "game/actors/boss_gyorg.h"
#include "game/actors/boss_odolwa.h"
#include "game/actors/boss_twinmold.h"
#include "game/as.h"
#include "game/collision.h"
#include "game/context.h"
#include "game/player.h"
#include "game/random.h"
#include "game/sound.h"

namespace rst {

using OdolwaCalc = std::remove_pointer_t<decltype(game::act::BossOdolwa::odolwa_calc)>;
static auto const odolwa_stunned = (OdolwaCalc*)util::GetAddr(0x53E880);
static auto const odolwa_stunned_2 = (OdolwaCalc*)util::GetAddr(0x2FBC68);
static auto const odolwa_intro = (OdolwaCalc*)util::GetAddr(0x308484);
static auto const odolwa_frozen = (OdolwaCalc*)util::GetAddr(0x56655C);
static auto const odolwa_stunned_eye_exposed_taking_damage = (OdolwaCalc*)util::GetAddr(0x27AA2C);
static auto const odolwa_stunned_eye_exposed = (OdolwaCalc*)util::GetAddr(0x555EF0);
static auto const odolwa_weakened = (OdolwaCalc*)util::GetAddr(0x27E194);
static auto const odolwa_charging = (OdolwaCalc*)util::GetAddr(0x24C5A4);
static auto const odolwa_preparing = (OdolwaCalc*)util::GetAddr(0x24FE24);
static auto const odolwa_waiting = (OdolwaCalc*)util::GetAddr(0x5725E4);
static auto const odolwa_dying = (OdolwaCalc*)util::GetAddr(0x2A4FAC);
static auto const odolwa_jump_landed = (OdolwaCalc*)util::GetAddr(0x2A6FF8);

extern "C" RST_HOOK int rst_OdolwaGetWaitDuration(game::act::BossOdolwa* odolwa) {
  if (odolwa->odolwa_calc_prev == odolwa_intro)
    return 90;
  const bool is_phase_1 = odolwa->call_bug_counter == 0;
  const int delay = game::RandomFloat() <= 0.25f ? 15 : 0;
  return delay + (is_phase_1 ? 60 : 15);
}

extern "C" RST_HOOK int rst_OdolwaGetChargeDuration(game::act::BossOdolwa* odolwa) {
  if (odolwa->command == u8(game::act::BossOdolwa::ChargeCommand::DanceThrust)) {
    const float probability_long_charge = odolwa->call_bug_counter == 0 ? 0.9 : 0.25;
    return game::RandomFloat() <= probability_long_charge ? 88 : 0;
  }
  return 2 * static_cast<int>(odolwa->anim_duration * 1.5f + 0.5f);
}

extern "C" RST_HOOK bool rst_OdolwaShouldMoveToPhase2(game::act::BossOdolwa* odolwa) {
  if (odolwa->call_bug_counter)
    return false;
  return odolwa->call_bug_timer > 1500 || odolwa->life <= 10;
}

extern "C" RST_HOOK bool rst_OdolwaCheckIsInvincible(game::act::BossOdolwa* odolwa) {
  // Nintendo uses an invincibility timer to ensure some attacks such as kicking
  // don't deal too much damage and send Link flying.
  // The problem is that this system also prevents landing attacks whenever Odolwa is kicking...
  // The workaround is to check whether an attack has landed in addition to checking the timer.
  static bool s_collided_last_time = false;
  if (odolwa->invincibility_timer == 0)
    return false;
  const bool collided =
      std::any_of(odolwa->collision.begin(), odolwa->collision.end(), game::IsCollided);
  if (s_collided_last_time) {
    s_collided_last_time = collided;
    return true;
  }
  s_collided_last_time = collided;
  return !collided;
}

static void OdolwaHandleCollisionFreeze(game::act::BossOdolwa* boss) {
  using namespace game;
  boss->ChangeCalcFunction(odolwa_frozen);
  boss->timer = 60;
  if (boss->field_30C_delta == 0 && boss->field_30C == 0)
    boss->field_30C_delta = 2;
  EmitDamageFlash(*boss, 0, 0x78, 0, 0x3C);
  boss->invincibility_timer = 23;
  sound::PlayEffect(*boss, sound::EffectId::NA_SE_EN_COMMON_FREEZE);
  boss->do_not_use_shield = 0;
}

static int OdolwaGetDamage(const game::act::BossOdolwa* boss, const game::CollisionInfo* info) {
  if (boss->damage_type == game::act::DamageType::Hookshot)
    return 0;
  const game::AttackType type = info->colliding_info->GetType();
  const auto* reactions = util::GetPointer<game::act::OdolwaDamageReaction>(0x65E49C);
  const auto& reaction = reactions[u8(type)];
  switch (reaction.type) {
  case game::act::OdolwaDamageReaction::Type::ApplyDamageMultiplier:
    return boss->damage * reaction.value;
  case game::act::OdolwaDamageReaction::Type::AddExtraDamage:
    return boss->damage + reaction.value;
  case game::act::OdolwaDamageReaction::Type::SetDamage:
    return reaction.value;
  default:
    return boss->damage;
  }
}

static void OdolwaJumpAway(game::act::BossOdolwa* boss) {
  const auto jump_away =
      util::GetPointer<void(game::act::BossOdolwa*, game::GlobalContext*, int)>(0x2FE074);
  jump_away(boss, GetContext().gctx, 2);
}

RST_HOOK void OdolwaHandleRegularCollision(game::act::BossOdolwa* boss) {
  using namespace game;
  auto* gctx = GetContext().gctx;

  if (boss->odolwa_calc == odolwa_stunned_eye_exposed_taking_damage)
    return;

  const auto it = std::find_if(boss->collision.begin(), boss->collision.end(), IsCollided);
  if (it == boss->collision.end())
    return;

  util::Print("%s: detected collision - idx=%zu damage_type=0x%x damage=%u attack_type=%u",
              __func__, std::distance(boss->collision.begin(), it), u8(boss->damage_type),
              boss->damage, u8(it->colliding_info->GetType()));

  it->flags1.Clear(CollisionInfo::Flag1::Collided);
  switch (boss->damage_type) {
  case act::DamageType::Type1:
    OdolwaHandleCollisionFreeze(boss);
    return;
  case act::DamageType::FireArrow:
    boss->field_1C35 = 1;
    break;
  case act::DamageType::IceArrow:
    boss->field_1C35 = 0xA;
    OdolwaHandleCollisionFreeze(boss);
    return;
  case act::DamageType::Type4:
    boss->field_1C35 = 0x14;
    gctx->SpawnActor(act::Id::ClearTag, 0, 0, 0, 4, boss->target_pos.pos);
    break;
  case act::DamageType::TypeB:
    boss->field_1C35 = 0x28;
    OdolwaHandleCollisionFreeze(boss);
    return;
  case act::DamageType::TypeC:
    boss->field_1C35 = 0x1E;
    gctx->SpawnActor(act::Id::ClearTag, 0, 0, 3, 4, boss->target_pos.pos);
    break;
  default:
    break;
  }

  auto* moth_swarm = util::BitCastPtr<act::Actor*>(gctx, 0xF000 + 4);
  util::Write<u16>(moth_swarm, 0x1FE, 0);

  // Apply damage.
  const int damage = OdolwaGetDamage(boss, it);
  boss->life -= damage;
  boss->cycle_life -= damage;

  if (boss->life <= 0) {
    boss->actor_util.PlayAnimFull(0xE, boss->field_208);
    boss->anim_duration = boss->actor_util.GetAnimDuration(0xE);
    boss->actor_util.field_89 = 0;
    boss->timer2 = 0;
    boss->ChangeCalcFunction(odolwa_dying);
    sound::PlayEffect(*boss, sound::EffectId::NA_SE_EN_FANTOM_DEAD);
    boss->flags.Clear(act::Actor::Flag::Targetable);
    boss->shield_timer_2 = 0x5DC;
    boss->intro_timer = 0;
    boss->intro_state = 0;
    sound::ControlStream(sound::StreamPlayer::DEFAULT_PLAYER, 1, 1);
    util::Write<u8>(moth_swarm, 0x1F8, 0xFA);
    gctx->ChangeActorType(*moth_swarm, act::Type::Boss);
    sound::PlayEffect(*boss, sound::EffectId::NA_SE_EN_MIBOSS_DEAD);
    gctx->EmitLastDamageSound(*boss);
    EmitDamageEffect(*it, DamageEffect::Damage);
    return;
  }

  if (damage == 0) {
    EmitDamageEffect(*it, DamageEffect::NoDamage);
  } else {
    EmitDamageEffect(*it, DamageEffect::Damage);
    if (!it->colliding_info->IsType(AttackType::Arrow))
      EmitDamageFlash(*boss, 0x400000, 0x64, 0, 0x17);
  }
  boss->field_364 *= 0.5f;
  boss->field_368 *= 0.5f;

  if (boss->cycle_life <= 0) {
    OdolwaJumpAway(boss);
    boss->invincibility_timer = 8;
    boss->cycle_life = 3;
    sound::PlayEffect(*boss, sound::EffectId::NA_SE_EN_MIBOSS_DAMAGE);
  } else {
    switch (boss->damage_type) {
    case act::DamageType::DekuNut: {
      EmitDamageFlash(*boss, 0, 0x80, 0, 0x17);
      boss->actor_util.PlayAnim(0x13, boss->field_20C);
      boss->actor_util.field_89 = 0;
      if (boss->field_30C_delta == 0 && boss->field_30C == 0)
        boss->field_30C_delta = 2;
      if (boss->odolwa_calc != odolwa_weakened) {
        boss->timer = 0x69;
        boss->ChangeCalcFunction(odolwa_weakened);
      }
      boss->do_not_use_shield = 0;
      sound::PlayEffect(*boss, sound::EffectId::NA_SE_EN_MIBOSS_DAMAGE);
      boss->invincibility_timer = 0x17;
      break;
    }
    case act::DamageType::Hookshot:
      break;
    default:
      if (it->colliding_info->IsType(game::AttackType::DekuFlower) &&
          boss->odolwa_calc == odolwa_charging) {
        boss->actor_util.PlayAnimFull(0x2D, -3.0);
        boss->anim_duration = boss->actor_util.GetAnimDuration(0x2D);
        boss->ChangeCalcFunction(odolwa_stunned_2);
      } else {
        if (boss->field_30C_delta == 0 && boss->field_30C == 0)
          boss->field_30C_delta = 2;
        if (boss->odolwa_calc != odolwa_weakened || boss->actor_util.state.id == 27) {
          if (it->colliding_info->IsType(AttackType::Arrow) ||
              (boss->odolwa_calc != odolwa_weakened && boss->actor_util.state.id != 27)) {
            boss->timer = 60;
          }
          boss->actor_util.PlayAnim(0x13, 0.0);
          boss->actor_util.field_89 = 0;
          boss->ChangeCalcFunction(odolwa_weakened);
        }
      }
      sound::PlayEffect(*boss, sound::EffectId::NA_SE_EN_MIBOSS_DAMAGE);
      boss->invincibility_timer = 8;
      break;
    }
  }

  boss->do_not_use_shield = 0;
  boss->taunting_timer = 1800;
  // Clear the remaining collision flags.
  for (auto& collision : boss->collision)
    collision.flags1.Clear(CollisionInfo::Flag1::Collided);
}

void FixOdolwa() {
  auto* gctx = GetContext().gctx;
  auto* boss = gctx->FindActorWithId<game::act::BossOdolwa>(game::act::Id::BossOdolwa,
                                                            game::act::Type::Boss);
  if (!boss)
    return;

  if (boss->damage_table) {
    boss->damage_table->entries[u8(game::AttackType::Arrow)].damage = 0;
  }

  if (boss->intro_state == 3) {
    // Re-implement the SFX and BGM triggers.
    if (boss->intro_timer == 0) {
      const auto play_boss_bgm = util::GetPointer<void(game::sound::StreamId)>(0x15D198);
      play_boss_bgm(game::sound::StreamId::NA_BGM_BOSS00);
    }
    // Skip the eye opening part of the intro animation.
    if (boss->intro_timer == 2) {
      boss->actor_util.state.position = 32.666653;
      boss->intro_timer = 49;
    }
    if (boss->intro_timer == 60 || boss->intro_timer == 114 || boss->intro_timer == 176)
      game::sound::PlayEffect(*boss, game::sound::EffectId::NA_SE_EN_MIBOSS_SWORD);
    return;
  }

  const bool preparing_charge =
      boss->odolwa_calc == odolwa_preparing &&
      boss->command == u8(game::act::BossOdolwa::ChargeCommand::DanceThrust);
  if (preparing_charge || boss->odolwa_calc == odolwa_waiting) {
    if (boss->distance_to_link <= 120.0f && boss->odolwa_calc_prev != odolwa_jump_landed &&
        !gctx->GetPlayerActor()->flags3.IsSet(game::act::Player::Flag3::DekuInFlower)) {
      OdolwaJumpAway(boss);
    }
  }
}

extern "C" RST_HOOK bool rst_IsGohtCollided(game::act::BossGoht* goht) {
  if (!goht->goht_flags.IsSet(game::act::BossGoht::Flag::FinishedPlayingStunCutsceneOnce) ||
      goht->field_3998) {
    return false;
  }
  return std::any_of(goht->collision.begin(), goht->collision.end(), game::IsCollided);
}

void FixGoht() {
  const game::GlobalContext* gctx = GetContext().gctx;
  auto* goht =
      gctx->FindActorWithId<game::act::BossGoht>(game::act::Id::BossGoht, game::act::Type::Boss);
  if (!goht)
    return;

  // Prevent the eye from popping out.
  goht->eye_scale = {0.8, 0.8, 0.05};
  // Disable the "eye pops out" cutscene.
  goht->goht_flags.Set(game::act::BossGoht::Flag::FinishedPlayingStunCutsceneOnce);
}

extern "C" RST_HOOK int rst_GetGyorgCollisionResponse(game::act::BossGyorg* gyorg) {
  const auto calc_fn = reinterpret_cast<uintptr_t>(gyorg->gyorg_calc);
  constexpr uintptr_t stunned = util::GetAddr(0x2802C8);
  constexpr uintptr_t stunned_2 = util::GetAddr(0x5560C0);
  constexpr uintptr_t stunned_attacked = util::GetAddr(0x27AAD0);
  return util::Contains(std::array{stunned, stunned_2, stunned_attacked}, calc_fn) ? 2 : 1;
}

struct GyorgFixState {
  std::optional<u32> frames_since_inhale_attack_end;
};

void FixGyorg() {
  static std::optional<GyorgFixState> state{};
  auto* gctx = GetContext().gctx;
  auto* gyorg =
      gctx->FindActorWithId<game::act::BossGyorg>(game::act::Id::BossGyorg, game::act::Type::Boss);
  if (!gyorg) {
    state.reset();
    return;
  }

  if (!state) {
    util::Print("%s: initialising state", __func__);
    state.emplace();
  }

  if (state->frames_since_inhale_attack_end.has_value())
    ++*state->frames_since_inhale_attack_end;

  const auto gyorg_eating_link = (decltype(gyorg->gyorg_calc))util::GetAddr(0x557900);
  if (gyorg->gyorg_calc == gyorg_eating_link)
    state->frames_since_inhale_attack_end = 0;

  // Disable the first stun cutscene, which is known to be buggy.
  gyorg->field_F24 |= 1;
  gyorg->eyeball_flags = 0;
  gyorg->eyeball_scale = {0.0, 0.0, 0.0};

  // Dying while being inhaled by Gyorg causes Link to exit the "held (by Gyorg or Majora's whips)"
  // state. After the fairy revival cutscene, Link enters the Zora swim state (0x21d5b8) and then
  // switches to the frozen state (0x20d96c), even though he's supposed to be in the "held" state.
  // If this situation is detected, clear the FreezeLink flag.
  const auto player = gctx->GetPlayerActor();
  const auto link_handle_frozen = (decltype(player->state_handler_fn))util::GetAddr(0x20D96C);
  if (gyorg->gyorg_calc == gyorg_eating_link && player->state_handler_fn == link_handle_frozen &&
      player->flags1.TestAndClear(game::act::Player::Flag1::FreezeLink)) {
    util::Print("%s: clearing FreezeLink flag", __func__);
  }

  // Hacky workaround for a very rare bug that causes Link to be frozen *after* being spit out of
  // Gyorg, despite the fact that a state handler change is supposed to clear the FreezeLink flag...
  // This forces the flag to be cleared after an inhale attack has ended.
  if (state->frames_since_inhale_attack_end && *state->frames_since_inhale_attack_end <= 30 &&
      player->flags1.TestAndClear(game::act::Player::Flag1::FreezeLink)) {
    util::Print("%s: clearing FreezeLink flag after inhale attack", __func__);
  }
}

struct TwinmoldFixState {
  s8 blue_prev_life;
  s8 red_prev_life;
  game::act::BossTwinmold::Status red_prev_status;
  u16 red_prev_hit_counter;
  bool is_hit_counter_sane;
};

void FixTwinmold() {
  static std::optional<TwinmoldFixState> state{};
  const game::GlobalContext* gctx = GetContext().gctx;

  auto* red_twinmold = gctx->FindActorWithId<game::act::BossTwinmold>(game::act::Id::BossTwinmold,
                                                                      game::act::Type::Boss);
  if (!red_twinmold) {
    state.reset();
    return;
  }

  auto* blue_twinmold = red_twinmold->other_twinmold_actor;

  if (state) {
    // Red Twinmold has 12 HP (after killing their blue friend).
    //
    // Spinning it deals 3-5 damage points based on lin_vel_xxx:
    //     boss->life -= 3 + (5 - 2) * player->lin_vel_xxx;
    // It is possible to deal 5 damage to the boss by spinning the main stick,
    // but that's not obvious at all...
    //
    // If the player spins the main stick (which is not an obvious thing to do...),
    // killing Red Twinmold takes 3 identical cycles.
    // If not, 4 cycles (!) are required.
    //
    // Let's make that less tedious and less boring by reducing the number of required cycles
    // (1 if the player touches the stick, 2 otherwise).
    if (state->red_prev_life > red_twinmold->life) {
      util::Print("%s: dealing more damage to Red Twinmold", __func__);
      red_twinmold->life -= 8;
    }

    // Only update the hit counter if it is sane. One way of ensuring that condition is satisfied
    // is to only consider the counter to be sane after the player has hit Twinmold once.
    if (red_twinmold->hit_counter == 8)
      state->is_hit_counter_sane = true;

    // 10 hits are required to stun Red or Blue Twinmold. This would have been acceptable
    // if it weren't for the fact that Red Twinmold regularly burrows back into sand during phase
    // 2 and the hit counter is reset every time that happens. This makes for a confusing
    // experience the first time the player fights Twinmold, as there is nothing in the game that
    // indicates that the hit counter resets every time (and it's still frustrating on subsequent
    // playthroughs).
    //
    // Fix that by restoring the previous hit counter after it's been reset by the game.
    const bool was_reset = red_twinmold->hit_counter == 9 && state->red_prev_hit_counter != 9;
    const bool is_legit_reset = red_twinmold->status == game::act::BossTwinmold::Status::Stunned;
    if (state->is_hit_counter_sane && was_reset && !is_legit_reset) {
      util::Print("%s: restoring hit counter (%u)", __func__, state->red_prev_hit_counter);
      red_twinmold->hit_counter = state->red_prev_hit_counter;
    }
  } else {
    util::Print("%s: initialising state", __func__);
    state.emplace();
    state->is_hit_counter_sane = false;
  }

  state->blue_prev_life = blue_twinmold->life;
  state->red_prev_life = red_twinmold->life;
  state->red_prev_status = red_twinmold->status;
  if (state->is_hit_counter_sane)
    state->red_prev_hit_counter = red_twinmold->hit_counter;
}

void FixBosses() {
  FixOdolwa();
  FixGoht();
  FixGyorg();
  FixTwinmold();
}

}  // namespace rst
