#include "game/items.h"

#include <algorithm>

#include "common/context.h"
#include "common/debug.h"
#include "common/utils.h"
#include "game/common_data.h"

namespace game {

Action GetActionForItem(ItemId item) {
  // 0x68F800 in Joker 1.0
  static constexpr Action table[] = {
      Action::Ocarina,   Action::Arrow,         Action::FireArrow,
      Action::IceArrow,  Action::LightArrow,    Action::PictographBox,
      Action(0xE),       Action(0x10),          Action(0x7),
      Action(0x12),      Action(0x30),          Action::PictographBox,
      Action(0xF),       Action::PictographBox, Action(0x54),
      Action(0xD),       Action(0x6),           Action::PictographBox,
      Action(0x15),      Action(0x23),          Action(0x25),
      Action(0x24),      Action(0x29),          Action(0x1A),
      Action(0x26),      Action(0x27),          Action(0x16),
      Action(0x20),      Action(0x20),          Action(0x21),
      Action(0x22),      Action(0x17),          Action(0x18),
      Action(0x19),      Action(0x1B),          Action(0x1E),
      Action(0x1D),      Action(0x28),          Action(0x2A),
      Action(0x2B),      Action(0x2C),          Action(0x2D),
      Action(0x31),      Action(0x32),          Action(0x33),
      Action(0x2E),      Action(0x35),          Action(0x2F),
      Action(0x38),      Action(0x3A),          Action::DekuMask,
      Action::GoronMask, Action::ZoraMask,      Action::FierceDeityMask,
      Action(0x3C),      Action(0x3D),          Action(0x3E),
      Action(0x3F),      Action(0x40),          Action(0x41),
      Action(0x42),      Action(0x43),          Action(0x44),
      Action(0x45),      Action(0x46),          Action(0x47),
      Action(0x48),      Action(0x49),          Action(0x4A),
      Action(0x4B),      Action(0x4C),          Action(0x4D),
      Action(0x4E),      Action::GiantMask,     Action::FireArrow,
      Action::IceArrow,  Action::LightArrow,    Action(0x3),
      Action(0x4),       Action(0x5),           Action(0x6),
      Action::None,      Action::None,          Action::None,
  };
  if (u8(item) >= std::size(table))
    return Action(0xffffffff);
  return table[u8(item)];
}

bool HasOcarina() {
  const auto& items = GetCommonData().save.inventory.items;
  return items[0] == ItemId::Ocarina;
}

bool HasItem(ItemId item_id) {
  const auto& items = GetCommonData().save.inventory.items;
  return std::any_of(items.begin(), items.end(), [&](ItemId id) { return item_id == id; });
}

bool HasMask(ItemId item_id) {
  const auto& masks = GetCommonData().save.inventory.masks;
  return std::any_of(masks.begin(), masks.end(), [&](ItemId id) { return item_id == id; });
}

bool CanUseItemUnsafe(ItemId item_id) {
  if (item_id == ItemId::Ocarina) {
    return GetCommonData().usable_btns[u8(UsableButton::Ocarina)] != ButtonIsUsable::No;
  }
  if (item_id == ItemId::PictographBox) {
    return GetCommonData().usable_btns[u8(UsableButton::PictographBox)] != ButtonIsUsable::No;
  }

  auto* gctx = rst::GetContext().gctx;
  if (!gctx)
    return false;

  // If the player is using the pictobox, return false since most items (other than the ocarina)
  // cannot be used and because the game won't update item usability status properly.
  auto* player = gctx->GetPlayerActor();
  if (!player)
    return false;
  if (player->action_type == act::Player::ActionType::Type2)
    return false;

  // This is a terrible hack to use the official item usability logic.
  // Assign the item to an action button, then update item usability by calling
  // a MM3D function, then restore the original assignment.

  FormEquipmentData& equipment = GetCommonData().save.equipment.data[0];
  const FormEquipmentData equipment_copy = equipment;

  equipment.item_btn_i = item_id;
  const auto update_usability = rst::util::GetPointer<void(GlobalContext*)>(0x1884B0);
  update_usability(gctx);

  const bool is_usable = GetCommonData().usable_btns[u8(UsableButton::I)] != ButtonIsUsable::No;

  equipment = equipment_copy;
  update_usability(gctx);
  return is_usable;
}

constexpr std::array<std::array<bool, 0x72>, 5> ItemUsabilityTable = {{
    {
        false,  // FierceDeity.Ocarina
        false,  // FierceDeity.Arrow
        false,  // FierceDeity.FireArrow
        false,  // FierceDeity.IceArrow
        false,  // FierceDeity.LightArrow
        false,  // FierceDeity.FairyOcarina
        false,  // FierceDeity.Bomb
        false,  // FierceDeity.Bombchu
        false,  // FierceDeity.DekuStick
        false,  // FierceDeity.DekuNuts
        false,  // FierceDeity.MagicBean
        false,  // FierceDeity.FairySlingshot
        false,  // FierceDeity.PowderKeg
        false,  // FierceDeity.PictographBox
        false,  // FierceDeity.LensOfTruth
        false,  // FierceDeity.Hookshot
        false,  // FierceDeity.GreatFairySword
        false,  // FierceDeity.HookshotUnused
        true,   // FierceDeity.Bottle
        true,   // FierceDeity.RedPotion
        true,   // FierceDeity.GreenPotion
        true,   // FierceDeity.BluePotion
        true,   // FierceDeity.Fairy
        true,   // FierceDeity.DekuPrincess
        true,   // FierceDeity.Milk
        true,   // FierceDeity.MilkHalf
        true,   // FierceDeity.Fish
        true,   // FierceDeity.Bug
        true,   // FierceDeity.BlueFire
        true,   // FierceDeity.Poe
        true,   // FierceDeity.BigPoe
        true,   // FierceDeity.Water
        true,   // FierceDeity.HotSpringWater
        true,   // FierceDeity.ZoraEgg
        true,   // FierceDeity.GoldDust
        true,   // FierceDeity.MagicalMushroom
        true,   // FierceDeity.SeaHorse
        true,   // FierceDeity.ChateauRomani
        true,   // FierceDeity.MysteryMilk
        true,   // FierceDeity.MysteryMilkSpoiled
        false,  // FierceDeity.MoonTear
        false,  // FierceDeity.LandTitleDeed
        false,  // FierceDeity.SwampTitleDeed
        false,  // FierceDeity.MountainTitleDeed
        false,  // FierceDeity.OceanTitleDeed
        false,  // FierceDeity.RoomKey
        false,  // FierceDeity.LetterToMama
        false,  // FierceDeity.LetterToKafei
        false,  // FierceDeity.PendantOfMemories
        false,  // FierceDeity.MapUnused
        true,   // FierceDeity.DekuMask
        true,   // FierceDeity.GoronMask
        true,   // FierceDeity.ZoraMask
        true,   // FierceDeity.FierceDeityMask
        true,   // FierceDeity.MaskOfTruth
        true,   // FierceDeity.KafeiMask
        true,   // FierceDeity.AllNightMask
        true,   // FierceDeity.BunnyHood
        true,   // FierceDeity.KeatonMask
        true,   // FierceDeity.GaroMask
        true,   // FierceDeity.RomaniMask
        true,   // FierceDeity.CircusLeaderMask
        true,   // FierceDeity.PostmanHat
        true,   // FierceDeity.CoupleMask
        true,   // FierceDeity.GreatFairyMask
        true,   // FierceDeity.GibdoMask
        true,   // FierceDeity.DonGeroMask
        true,   // FierceDeity.KamaroMask
        true,   // FierceDeity.CaptainHat
        true,   // FierceDeity.StoneMask
        true,   // FierceDeity.BremenMask
        true,   // FierceDeity.BlastMask
        true,   // FierceDeity.MaskOfScents
        false,  // FierceDeity.GiantMask
        false,  // FierceDeity.BowWithFireArrow
        false,  // FierceDeity.BowWithIceArrow
        false,  // FierceDeity.BowWithLightArrow
        false,  // FierceDeity.KokiriSword
        false,  // FierceDeity.RazorSword
        false,  // FierceDeity.GildedSword
        false,  // FierceDeity.FishingPass
        false,  // FierceDeity.HeroShield
        false,  // FierceDeity.MirrorShield
        false,  // FierceDeity.Quiver
        false,  // FierceDeity.LargeQuiver
        false,  // FierceDeity.BiggestQuiver
        false,  // FierceDeity.BombBag
        false,  // FierceDeity.BigBombBag
        false,  // FierceDeity.BiggestBombBag
        false,  // FierceDeity.GreenWallet
        false,  // FierceDeity.AdultWallet
        false,  // FierceDeity.GiantWallet
        false,  // FierceDeity.FishingRod
        false,  // FierceDeity.OdolwaRemains
        false,  // FierceDeity.GohtRemains
        false,  // FierceDeity.GyorgRemains
        false,  // FierceDeity.TwinmoldRemains
        false,  // FierceDeity.SonataOfAwakening
        false,  // FierceDeity.GoronLullaby
        false,  // FierceDeity.NewWaveBossaNova
        false,  // FierceDeity.ElegyOfEmptiness
        false,  // FierceDeity.OathToOrder
        false,  // FierceDeity.NocturneOfShadow
        false,  // FierceDeity.SongOfTime
        false,  // FierceDeity.SongOfHealing
        false,  // FierceDeity.EponaSong
        false,  // FierceDeity.SongOfSoaring
        false,  // FierceDeity.SongOfStorms
        false,  // FierceDeity.SunSong
        false,  // FierceDeity.BombersNotebook
        false,  // FierceDeity.GoldSkulltula
        false,  // FierceDeity.HeartContainer
        false,  // FierceDeity.PieceOfHeart
        false,  // FierceDeity.X71
    },
    {
        true,   // Goron.Ocarina
        false,  // Goron.Arrow
        false,  // Goron.FireArrow
        false,  // Goron.IceArrow
        false,  // Goron.LightArrow
        false,  // Goron.FairyOcarina
        false,  // Goron.Bomb
        false,  // Goron.Bombchu
        false,  // Goron.DekuStick
        false,  // Goron.DekuNuts
        false,  // Goron.MagicBean
        false,  // Goron.FairySlingshot
        true,   // Goron.PowderKeg
        true,   // Goron.PictographBox
        true,   // Goron.LensOfTruth
        false,  // Goron.Hookshot
        false,  // Goron.GreatFairySword
        false,  // Goron.HookshotUnused
        true,   // Goron.Bottle
        true,   // Goron.RedPotion
        true,   // Goron.GreenPotion
        true,   // Goron.BluePotion
        true,   // Goron.Fairy
        true,   // Goron.DekuPrincess
        true,   // Goron.Milk
        true,   // Goron.MilkHalf
        true,   // Goron.Fish
        true,   // Goron.Bug
        true,   // Goron.BlueFire
        true,   // Goron.Poe
        true,   // Goron.BigPoe
        true,   // Goron.Water
        true,   // Goron.HotSpringWater
        true,   // Goron.ZoraEgg
        true,   // Goron.GoldDust
        true,   // Goron.MagicalMushroom
        true,   // Goron.SeaHorse
        true,   // Goron.ChateauRomani
        true,   // Goron.MysteryMilk
        true,   // Goron.MysteryMilkSpoiled
        true,   // Goron.MoonTear
        true,   // Goron.LandTitleDeed
        true,   // Goron.SwampTitleDeed
        true,   // Goron.MountainTitleDeed
        true,   // Goron.OceanTitleDeed
        true,   // Goron.RoomKey
        true,   // Goron.LetterToMama
        true,   // Goron.LetterToKafei
        true,   // Goron.PendantOfMemories
        true,   // Goron.MapUnused
        true,   // Goron.DekuMask
        true,   // Goron.GoronMask
        true,   // Goron.ZoraMask
        true,   // Goron.FierceDeityMask
        true,   // Goron.MaskOfTruth
        true,   // Goron.KafeiMask
        true,   // Goron.AllNightMask
        true,   // Goron.BunnyHood
        true,   // Goron.KeatonMask
        true,   // Goron.GaroMask
        true,   // Goron.RomaniMask
        true,   // Goron.CircusLeaderMask
        true,   // Goron.PostmanHat
        true,   // Goron.CoupleMask
        true,   // Goron.GreatFairyMask
        true,   // Goron.GibdoMask
        true,   // Goron.DonGeroMask
        true,   // Goron.KamaroMask
        true,   // Goron.CaptainHat
        true,   // Goron.StoneMask
        true,   // Goron.BremenMask
        true,   // Goron.BlastMask
        true,   // Goron.MaskOfScents
        false,  // Goron.GiantMask
        false,  // Goron.BowWithFireArrow
        false,  // Goron.BowWithIceArrow
        false,  // Goron.BowWithLightArrow
        false,  // Goron.KokiriSword
        false,  // Goron.RazorSword
        false,  // Goron.GildedSword
        false,  // Goron.FishingPass
        false,  // Goron.HeroShield
        false,  // Goron.MirrorShield
        false,  // Goron.Quiver
        false,  // Goron.LargeQuiver
        false,  // Goron.BiggestQuiver
        false,  // Goron.BombBag
        false,  // Goron.BigBombBag
        false,  // Goron.BiggestBombBag
        false,  // Goron.GreenWallet
        false,  // Goron.AdultWallet
        false,  // Goron.GiantWallet
        false,  // Goron.FishingRod
        false,  // Goron.OdolwaRemains
        false,  // Goron.GohtRemains
        false,  // Goron.GyorgRemains
        false,  // Goron.TwinmoldRemains
        false,  // Goron.SonataOfAwakening
        false,  // Goron.GoronLullaby
        false,  // Goron.NewWaveBossaNova
        false,  // Goron.ElegyOfEmptiness
        false,  // Goron.OathToOrder
        false,  // Goron.NocturneOfShadow
        false,  // Goron.SongOfTime
        false,  // Goron.SongOfHealing
        false,  // Goron.EponaSong
        false,  // Goron.SongOfSoaring
        false,  // Goron.SongOfStorms
        false,  // Goron.SunSong
        false,  // Goron.BombersNotebook
        false,  // Goron.GoldSkulltula
        false,  // Goron.HeartContainer
        false,  // Goron.PieceOfHeart
        false,  // Goron.X71
    },
    {
        true,   // Zora.Ocarina
        false,  // Zora.Arrow
        false,  // Zora.FireArrow
        false,  // Zora.IceArrow
        false,  // Zora.LightArrow
        false,  // Zora.FairyOcarina
        false,  // Zora.Bomb
        false,  // Zora.Bombchu
        false,  // Zora.DekuStick
        false,  // Zora.DekuNuts
        false,  // Zora.MagicBean
        false,  // Zora.FairySlingshot
        false,  // Zora.PowderKeg
        true,   // Zora.PictographBox
        true,   // Zora.LensOfTruth
        false,  // Zora.Hookshot
        false,  // Zora.GreatFairySword
        false,  // Zora.HookshotUnused
        true,   // Zora.Bottle
        true,   // Zora.RedPotion
        true,   // Zora.GreenPotion
        true,   // Zora.BluePotion
        true,   // Zora.Fairy
        true,   // Zora.DekuPrincess
        true,   // Zora.Milk
        true,   // Zora.MilkHalf
        true,   // Zora.Fish
        true,   // Zora.Bug
        true,   // Zora.BlueFire
        true,   // Zora.Poe
        true,   // Zora.BigPoe
        true,   // Zora.Water
        true,   // Zora.HotSpringWater
        true,   // Zora.ZoraEgg
        true,   // Zora.GoldDust
        true,   // Zora.MagicalMushroom
        true,   // Zora.SeaHorse
        true,   // Zora.ChateauRomani
        true,   // Zora.MysteryMilk
        true,   // Zora.MysteryMilkSpoiled
        true,   // Zora.MoonTear
        true,   // Zora.LandTitleDeed
        true,   // Zora.SwampTitleDeed
        true,   // Zora.MountainTitleDeed
        true,   // Zora.OceanTitleDeed
        true,   // Zora.RoomKey
        true,   // Zora.LetterToMama
        true,   // Zora.LetterToKafei
        true,   // Zora.PendantOfMemories
        true,   // Zora.MapUnused
        true,   // Zora.DekuMask
        true,   // Zora.GoronMask
        true,   // Zora.ZoraMask
        true,   // Zora.FierceDeityMask
        true,   // Zora.MaskOfTruth
        true,   // Zora.KafeiMask
        true,   // Zora.AllNightMask
        true,   // Zora.BunnyHood
        true,   // Zora.KeatonMask
        true,   // Zora.GaroMask
        true,   // Zora.RomaniMask
        true,   // Zora.CircusLeaderMask
        true,   // Zora.PostmanHat
        true,   // Zora.CoupleMask
        true,   // Zora.GreatFairyMask
        true,   // Zora.GibdoMask
        true,   // Zora.DonGeroMask
        true,   // Zora.KamaroMask
        true,   // Zora.CaptainHat
        true,   // Zora.StoneMask
        true,   // Zora.BremenMask
        true,   // Zora.BlastMask
        true,   // Zora.MaskOfScents
        false,  // Zora.GiantMask
        false,  // Zora.BowWithFireArrow
        false,  // Zora.BowWithIceArrow
        false,  // Zora.BowWithLightArrow
        false,  // Zora.KokiriSword
        false,  // Zora.RazorSword
        false,  // Zora.GildedSword
        false,  // Zora.FishingPass
        false,  // Zora.HeroShield
        false,  // Zora.MirrorShield
        false,  // Zora.Quiver
        false,  // Zora.LargeQuiver
        false,  // Zora.BiggestQuiver
        false,  // Zora.BombBag
        false,  // Zora.BigBombBag
        false,  // Zora.BiggestBombBag
        false,  // Zora.GreenWallet
        false,  // Zora.AdultWallet
        false,  // Zora.GiantWallet
        false,  // Zora.FishingRod
        false,  // Zora.OdolwaRemains
        false,  // Zora.GohtRemains
        false,  // Zora.GyorgRemains
        false,  // Zora.TwinmoldRemains
        false,  // Zora.SonataOfAwakening
        false,  // Zora.GoronLullaby
        false,  // Zora.NewWaveBossaNova
        false,  // Zora.ElegyOfEmptiness
        false,  // Zora.OathToOrder
        false,  // Zora.NocturneOfShadow
        false,  // Zora.SongOfTime
        false,  // Zora.SongOfHealing
        false,  // Zora.EponaSong
        false,  // Zora.SongOfSoaring
        false,  // Zora.SongOfStorms
        false,  // Zora.SunSong
        false,  // Zora.BombersNotebook
        false,  // Zora.GoldSkulltula
        false,  // Zora.HeartContainer
        false,  // Zora.PieceOfHeart
        false,  // Zora.X71
    },
    {
        true,   // Deku.Ocarina
        false,  // Deku.Arrow
        false,  // Deku.FireArrow
        false,  // Deku.IceArrow
        false,  // Deku.LightArrow
        false,  // Deku.FairyOcarina
        false,  // Deku.Bomb
        false,  // Deku.Bombchu
        false,  // Deku.DekuStick
        true,   // Deku.DekuNuts
        false,  // Deku.MagicBean
        false,  // Deku.FairySlingshot
        false,  // Deku.PowderKeg
        true,   // Deku.PictographBox
        true,   // Deku.LensOfTruth
        false,  // Deku.Hookshot
        false,  // Deku.GreatFairySword
        false,  // Deku.HookshotUnused
        true,   // Deku.Bottle
        true,   // Deku.RedPotion
        true,   // Deku.GreenPotion
        true,   // Deku.BluePotion
        true,   // Deku.Fairy
        true,   // Deku.DekuPrincess
        true,   // Deku.Milk
        true,   // Deku.MilkHalf
        true,   // Deku.Fish
        true,   // Deku.Bug
        true,   // Deku.BlueFire
        true,   // Deku.Poe
        true,   // Deku.BigPoe
        true,   // Deku.Water
        true,   // Deku.HotSpringWater
        true,   // Deku.ZoraEgg
        true,   // Deku.GoldDust
        true,   // Deku.MagicalMushroom
        true,   // Deku.SeaHorse
        true,   // Deku.ChateauRomani
        true,   // Deku.MysteryMilk
        true,   // Deku.MysteryMilkSpoiled
        true,   // Deku.MoonTear
        true,   // Deku.LandTitleDeed
        true,   // Deku.SwampTitleDeed
        true,   // Deku.MountainTitleDeed
        true,   // Deku.OceanTitleDeed
        true,   // Deku.RoomKey
        true,   // Deku.LetterToMama
        true,   // Deku.LetterToKafei
        true,   // Deku.PendantOfMemories
        true,   // Deku.MapUnused
        true,   // Deku.DekuMask
        true,   // Deku.GoronMask
        true,   // Deku.ZoraMask
        true,   // Deku.FierceDeityMask
        true,   // Deku.MaskOfTruth
        true,   // Deku.KafeiMask
        true,   // Deku.AllNightMask
        true,   // Deku.BunnyHood
        true,   // Deku.KeatonMask
        true,   // Deku.GaroMask
        true,   // Deku.RomaniMask
        true,   // Deku.CircusLeaderMask
        true,   // Deku.PostmanHat
        true,   // Deku.CoupleMask
        true,   // Deku.GreatFairyMask
        true,   // Deku.GibdoMask
        true,   // Deku.DonGeroMask
        true,   // Deku.KamaroMask
        true,   // Deku.CaptainHat
        true,   // Deku.StoneMask
        true,   // Deku.BremenMask
        true,   // Deku.BlastMask
        true,   // Deku.MaskOfScents
        false,  // Deku.GiantMask
        false,  // Deku.BowWithFireArrow
        false,  // Deku.BowWithIceArrow
        false,  // Deku.BowWithLightArrow
        false,  // Deku.KokiriSword
        false,  // Deku.RazorSword
        false,  // Deku.GildedSword
        false,  // Deku.FishingPass
        false,  // Deku.HeroShield
        false,  // Deku.MirrorShield
        false,  // Deku.Quiver
        false,  // Deku.LargeQuiver
        false,  // Deku.BiggestQuiver
        false,  // Deku.BombBag
        false,  // Deku.BigBombBag
        false,  // Deku.BiggestBombBag
        false,  // Deku.GreenWallet
        false,  // Deku.AdultWallet
        false,  // Deku.GiantWallet
        false,  // Deku.FishingRod
        false,  // Deku.OdolwaRemains
        false,  // Deku.GohtRemains
        false,  // Deku.GyorgRemains
        false,  // Deku.TwinmoldRemains
        false,  // Deku.SonataOfAwakening
        false,  // Deku.GoronLullaby
        false,  // Deku.NewWaveBossaNova
        false,  // Deku.ElegyOfEmptiness
        false,  // Deku.OathToOrder
        false,  // Deku.NocturneOfShadow
        false,  // Deku.SongOfTime
        false,  // Deku.SongOfHealing
        false,  // Deku.EponaSong
        false,  // Deku.SongOfSoaring
        false,  // Deku.SongOfStorms
        false,  // Deku.SunSong
        false,  // Deku.BombersNotebook
        false,  // Deku.GoldSkulltula
        false,  // Deku.HeartContainer
        false,  // Deku.PieceOfHeart
        false,  // Deku.X71
    },
    {
        true,   // Human.Ocarina
        true,   // Human.Arrow
        true,   // Human.FireArrow
        true,   // Human.IceArrow
        true,   // Human.LightArrow
        true,   // Human.FairyOcarina
        true,   // Human.Bomb
        true,   // Human.Bombchu
        true,   // Human.DekuStick
        true,   // Human.DekuNuts
        true,   // Human.MagicBean
        true,   // Human.FairySlingshot
        false,  // Human.PowderKeg
        true,   // Human.PictographBox
        true,   // Human.LensOfTruth
        true,   // Human.Hookshot
        true,   // Human.GreatFairySword
        true,   // Human.HookshotUnused
        true,   // Human.Bottle
        true,   // Human.RedPotion
        true,   // Human.GreenPotion
        true,   // Human.BluePotion
        true,   // Human.Fairy
        true,   // Human.DekuPrincess
        true,   // Human.Milk
        true,   // Human.MilkHalf
        true,   // Human.Fish
        true,   // Human.Bug
        true,   // Human.BlueFire
        true,   // Human.Poe
        true,   // Human.BigPoe
        true,   // Human.Water
        true,   // Human.HotSpringWater
        true,   // Human.ZoraEgg
        true,   // Human.GoldDust
        true,   // Human.MagicalMushroom
        true,   // Human.SeaHorse
        true,   // Human.ChateauRomani
        true,   // Human.MysteryMilk
        true,   // Human.MysteryMilkSpoiled
        true,   // Human.MoonTear
        true,   // Human.LandTitleDeed
        true,   // Human.SwampTitleDeed
        true,   // Human.MountainTitleDeed
        true,   // Human.OceanTitleDeed
        true,   // Human.RoomKey
        true,   // Human.LetterToMama
        true,   // Human.LetterToKafei
        true,   // Human.PendantOfMemories
        true,   // Human.MapUnused
        true,   // Human.DekuMask
        true,   // Human.GoronMask
        true,   // Human.ZoraMask
        true,   // Human.FierceDeityMask
        true,   // Human.MaskOfTruth
        true,   // Human.KafeiMask
        true,   // Human.AllNightMask
        true,   // Human.BunnyHood
        true,   // Human.KeatonMask
        true,   // Human.GaroMask
        true,   // Human.RomaniMask
        true,   // Human.CircusLeaderMask
        true,   // Human.PostmanHat
        true,   // Human.CoupleMask
        true,   // Human.GreatFairyMask
        true,   // Human.GibdoMask
        true,   // Human.DonGeroMask
        true,   // Human.KamaroMask
        true,   // Human.CaptainHat
        true,   // Human.StoneMask
        true,   // Human.BremenMask
        true,   // Human.BlastMask
        true,   // Human.MaskOfScents
        true,   // Human.GiantMask
        true,   // Human.BowWithFireArrow
        true,   // Human.BowWithIceArrow
        true,   // Human.BowWithLightArrow
        false,  // Human.KokiriSword
        false,  // Human.RazorSword
        false,  // Human.GildedSword
        false,  // Human.FishingPass
        false,  // Human.HeroShield
        false,  // Human.MirrorShield
        false,  // Human.Quiver
        false,  // Human.LargeQuiver
        false,  // Human.BiggestQuiver
        false,  // Human.BombBag
        false,  // Human.BigBombBag
        false,  // Human.BiggestBombBag
        false,  // Human.GreenWallet
        false,  // Human.AdultWallet
        false,  // Human.GiantWallet
        false,  // Human.FishingRod
        false,  // Human.OdolwaRemains
        false,  // Human.GohtRemains
        false,  // Human.GyorgRemains
        false,  // Human.TwinmoldRemains
        false,  // Human.SonataOfAwakening
        false,  // Human.GoronLullaby
        false,  // Human.NewWaveBossaNova
        false,  // Human.ElegyOfEmptiness
        false,  // Human.OathToOrder
        false,  // Human.NocturneOfShadow
        false,  // Human.SongOfTime
        false,  // Human.SongOfHealing
        false,  // Human.EponaSong
        false,  // Human.SongOfSoaring
        false,  // Human.SongOfStorms
        false,  // Human.SunSong
        false,  // Human.BombersNotebook
        false,  // Human.GoldSkulltula
        false,  // Human.HeartContainer
        false,  // Human.PieceOfHeart
        false,  // Human.X71
    },
}};

static bool IsBossRoomScene(GlobalContext* gctx) {
  return rst::util::IsAnyOf(gctx->scene, SceneId::OdolwaLair, SceneId::GohtLair, SceneId::GyorgLair,
                            SceneId::TwinmoldLair, SceneId::MajoraLair);
}

bool CanUseItem(ItemId item) {
  const auto& cdata = GetCommonData();

  if (item == ItemId::Ocarina)
    return cdata.usable_btns[u8(UsableButton::Ocarina)] != ButtonIsUsable::No;

  if (item == ItemId::PictographBox)
    return cdata.usable_btns[u8(UsableButton::PictographBox)] != ButtonIsUsable::No;

  auto* gctx = rst::GetContext().gctx;
  if (!gctx)
    return false;

  auto* player = gctx->GetPlayerActor();
  if (!player)
    return false;

  // If the player is using the pictobox, return false since most items (other than the ocarina)
  // cannot be used.
  if (player->action_type == act::Player::ActionType::Type2)
    return false;

  const auto get_item_usability_mode = rst::util::GetPointer<int(GlobalContext*)>(0x1E827C);
  const auto mode = get_item_usability_mode(gctx);

  // Riding or Honey & Darling minigame (and more?)
  if (player->flags1.IsSet(act::Player::Flag1::Riding) || cdata.save.anonymous_72 & 1 ||
      (!(cdata.save.anonymous_13 & 2) && gctx->field_C531 >= 2)) {
    return false;
  }

  if (gctx->scene == SceneId::SwampFishingHole || gctx->scene == SceneId::OceanFishingHole) {
    return item == ItemId::ZoraMask || item == ItemId::FierceDeityMask ||
           ItemIsNonTransformationMask(item) || (ItemIsTransformationMask(item) && mode < 2);
  }

  const bool is_beaver_race = cdata.field_3696 == 1 && cdata.sub1.entrance == 0x8E10;
  const bool is_goron_race = cdata.sub1.entrance == 0xD010;
  if ((is_beaver_race || is_goron_race) && !gctx->field_C529_one_to_clear_input &&
      !gctx->field_C85A) {
    return false;
  }

  if (cdata.save.anonymous_13 & 2)
    return false;

  if (cdata.save.anonymous_138 & 0x20)
    return false;

  if (cdata.save.anonymous_130 & 8)
    return false;

  // Cutscene map
  if (gctx->scene == SceneId::Cutscenes && cdata.setup2 == 6)
    return false;

  if ((cdata.save.anonymous_12 & 0x10) && !player->flags3.IsSet(act::Player::Flag3::DekuNutsOnB))
    return false;

  if (player->flags3.IsSet(act::Player::Flag3::DekuNutsOnB))
    return false;

  if (2 <= mode && mode <= 4) {
    if (item == ItemId::ZoraMask || (item == ItemId::FierceDeityMask && IsBossRoomScene(gctx)) ||
        ItemIsNonTransformationMask(item)) {
      return true;
    }
    return mode == 2 && !ItemIsRegular(item) && ItemIsBottled(item);
  }

  if (player->flags1.IsSet(act::Player::Flag1::Unk200000))
    return item == ItemId::LensOfTruth;

  if (player->flags1.IsSet(act::Player::Flag1::Unk2000) &&
      cdata.usable_btns[0] != ButtonIsUsable::No) {
    return false;
  }

  // Check the item usability table now.
  const auto& usable_item_table = ItemUsabilityTable[u8(cdata.save.player_form)];
  if (u8(item) >= usable_item_table.size()) {
    rst::util::Print("%s - ERROR - invalid item ID", __func__);
    return false;
  }
  // The item being allowed in the table is only a necessary condition for being allowed to use the
  // item; it is not a sufficient one.
  if (!usable_item_table[u8(item)]) {
    return false;
  }

  if (player->id != act::Id::Player)
    return false;

  if (player->active_mask_id == MaskId::GiantMask)
    return item == ItemId::GiantMask;

  if (item == ItemId::GiantMask)
    return gctx->scene == SceneId::TwinmoldLair;

  if (item == ItemId::FierceDeityMask)
    return IsBossRoomScene(gctx);

  if (gctx->actors.field_4) {
    if (rst::util::IsAnyOf(item, ItemId::FireArrow, ItemId::IceArrow, ItemId::LightArrow))
      return false;
  }

  if (ItemIsGear(item) || ItemIsBottled(item))
    return !gctx->hud_state.prohibit_gear_or_bottles;

  if (ItemIsMask(item))
    return !gctx->hud_state.prohibit_masks;

  return !gctx->hud_state.prohibit_regular_items;
}

}  // namespace game
