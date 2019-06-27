#include "game/context.h"

#include "game/player.h"

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

}  // namespace game
