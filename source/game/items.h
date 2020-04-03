#pragma once

#include <array>

#include "common/types.h"

namespace game {

enum class ItemId : u8 {
  Ocarina = 0x0,
  Arrow = 1,
  FireArrow = 2,
  IceArrow = 3,
  LightArrow = 4,
  FairyOcarina = 5,
  Bomb = 6,
  Bombchu = 7,
  DekuStick = 8,
  DekuNuts = 9,
  MagicBean = 0xa,
  FairySlingshot = 0xb,
  PowderKeg = 0xc,
  PictographBox = 0xd,
  LensOfTruth = 0xe,
  Hookshot = 0xf,
  GreatFairySword = 0x10,
  HookshotUnused = 0x11,

  Bottle = 0x12,
  RedPotion = 0x13,
  GreenPotion = 0x14,
  BluePotion = 0x15,
  Fairy = 0x16,
  DekuPrincess = 0x17,
  Milk = 0x18,
  MilkHalf = 0x19,
  Fish = 0x1a,
  Bug = 0x1b,
  BlueFire = 0x1c,
  Poe = 0x1d,
  BigPoe = 0x1e,
  Water = 0x1f,
  HotSpringWater = 0x20,
  ZoraEgg = 0x21,
  GoldDust = 0x22,
  MagicalMushroom = 0x23,
  SeaHorse = 0x24,
  ChateauRomani = 0x25,
  MysteryMilk = 0x26,
  MysteryMilkSpoiled = 0x27,

  MoonTear = 0x28,
  LandTitleDeed = 0x29,
  SwampTitleDeed = 0x2a,
  MountainTitleDeed = 0x2b,
  OceanTitleDeed = 0x2c,
  RoomKey = 0x2d,
  LetterToMama = 0x2e,
  LetterToKafei = 0x2f,
  PendantOfMemories = 0x30,

  MapUnused = 0x31,

  DekuMask = 0x32,
  GoronMask = 0x33,
  ZoraMask = 0x34,
  FierceDeityMask = 0x35,
  MaskOfTruth = 0x36,
  KafeiMask = 0x37,
  AllNightMask = 0x38,
  BunnyHood = 0x39,
  KeatonMask = 0x3a,
  GaroMask = 0x3b,
  RomaniMask = 0x3c,
  CircusLeaderMask = 0x3d,
  PostmanHat = 0x3e,
  CoupleMask = 0x3f,
  GreatFairyMask = 0x40,
  GibdoMask = 0x41,
  DonGeroMask = 0x42,
  KamaroMask = 0x43,
  CaptainHat = 0x44,
  StoneMask = 0x45,
  BremenMask = 0x46,
  BlastMask = 0x47,
  MaskOfScents = 0x48,
  GiantMask = 0x49,

  BowWithFireArrow = 0x4a,
  BowWithIceArrow = 0x4b,
  BowWithLightArrow = 0x4c,

  KokiriSword = 0x4d,
  RazorSword = 0x4e,
  GildedSword = 0x4f,

  FishingPass = 0x50,

  HeroShield = 0x51,
  MirrorShield = 0x52,

  Quiver = 0x53,
  LargeQuiver = 0x54,
  BiggestQuiver = 0x55,

  BombBag = 0x56,
  BigBombBag = 0x57,
  BiggestBombBag = 0x58,

  GreenWallet = 0x59,
  AdultWallet = 0x5a,
  GiantWallet = 0x5b,

  FishingRod = 0x5c,

  OdolwaRemains = 0x5d,
  GohtRemains = 0x5e,
  GyorgRemains = 0x5f,
  TwinmoldRemains = 0x60,

  SonataOfAwakening = 0x61,
  GoronLullaby = 0x62,
  NewWaveBossaNova = 0x63,
  ElegyOfEmptiness = 0x64,
  OathToOrder = 0x65,
  NocturneOfShadow = 0x66,
  SongOfTime = 0x67,
  SongOfHealing = 0x68,
  EponaSong = 0x69,
  SongOfSoaring = 0x6a,
  SongOfStorms = 0x6b,
  SunSong = 0x6c,

  BombersNotebook = 0x6d,

  GoldSkulltula = 0x6e,
  HeartContainer = 0x6f,
  PieceOfHeart = 0x70,

  X71 = 0x71,
  X72 = 0x72,
  X73 = 0x73,

  BossKey = 0x74,
  Compass = 0x75,
  Map = 0x76,
  StrayFairy = 0x77,
  SmallKey = 0x78,

  None = 0xff,
};

constexpr bool ItemIsBottled(ItemId item) {
  return ItemId::Bottle <= item && item <= ItemId::MysteryMilkSpoiled;
}

constexpr bool ItemIsGear(ItemId item) {
  return ItemId::MoonTear <= item && item <= ItemId::PendantOfMemories;
}

constexpr bool ItemIsMask(ItemId item) {
  return ItemId::DekuMask <= item && item <= ItemId::GiantMask;
}

constexpr bool ItemIsTransformationMask(ItemId item) {
  return ItemId::DekuMask <= item && item <= ItemId::FierceDeityMask;
}

constexpr bool ItemIsNonTransformationMask(ItemId item) {
  return ItemId::MaskOfTruth <= item && item <= ItemId::MaskOfScents;
}

constexpr bool ItemIsRegular(ItemId item) {
  return item < ItemId::Bottle;
}

enum class Action : u8 {
  None = 0,
  Arrow = 0x9,
  FireArrow = 0xa,
  IceArrow = 0xb,
  LightArrow = 0xc,
  Hookshot = 0xd,
  DekuBubble = 0x12,
  PictographBox = 0x13,
  Ocarina = 0x14,
  GiantMask = 0x4F,
  FierceDeityMask = 0x50,
  GoronMask = 0x51,
  ZoraMask = 0x52,
  DekuMask = 0x53,
};

Action GetActionForItem(ItemId item);

// Mask IDs are action IDs - 0x3b
enum class MaskId : u8 {
  GiantMask = 0x14,
  FierceDeityMask = 0x15,
  GoronMask = 0x16,
  ZoraMask = 0x17,
  DekuMask = 0x18,
};

bool HasOcarina();
bool HasItem(ItemId item_id);
bool HasMask(ItemId item_id);

/// Checks whether an item is usable.
///
/// Calls the global context "update usable buttons" function to determine whether an item is
/// usable. This is unsafe because of side effects and might result in item usability being updated
/// too often (which patches the Restricted Items glitch).
bool CanUseItemUnsafe(ItemId item_id);

/// Checks whether an item is usable as if it were equipped to an item button (X/Y/I/II) (if the
/// item is not the ocarina or pictobox).
///
/// Might return incorrect results for edge cases.
bool CanUseItem(ItemId item_id);

}  // namespace game
