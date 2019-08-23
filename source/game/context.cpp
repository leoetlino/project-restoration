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
                                      u16 param, Vec3 pos) {
  auto spawn_actor_wrapper = rst::util::GetPointer<act::Actor*(
      ActorLists*, act::Actor*, GlobalContext*, act::Id, u16, u16, u16, u16 param, Vec3)>(0x22CAAC);
  return spawn_actor_wrapper(&actors, actor, this, id, rx, ry, rz, param, pos);
}

act::Actor* GlobalContext::SpawnActor(act::Id id, u16 rx, u16 ry, u16 rz, u16 param, Vec3 pos) {
  auto spawn_actor_wrapper = rst::util::GetPointer<act::Actor*(
      ActorLists*, GlobalContext*, act::Id, u16, u16, u16, u16 param, Vec3)>(0x22074C);
  return spawn_actor_wrapper(&actors, this, id, rx, ry, rz, param, pos);
}

void GlobalContext::ChangeActorType(act::Actor& actor, act::Type type) {
  actors.GetList(actor.actor_type).need_to_update_lists = true;
  actor.actor_type = type;
}

void GlobalContext::ShowMessage(u16 msgid, int unknown) {
  rst::util::GetPointer<void(GlobalContext*, int msgid, int)>(0x21BAFC)(this, msgid, unknown);
}

void GlobalContext::Talk(act::Actor* actor, int a) {
  GetPlayerActor()->talk_actor = actor;
  rst::util::GetPointer<void(GlobalContext*, act::Actor*, int)>(0x2209A8)(this, actor, a);
}

void GlobalContext::EmitLastDamageSound(const act::Actor& actor) {
  rst::util::GetPointer<void(GlobalContext*, const act::Actor&)>(0x389AC0)(this, actor);
}

}  // namespace game
