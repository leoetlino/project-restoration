#pragma once

#include <array>

#include "common/types.h"

namespace game {

enum class ItemId : u8 {
  Ocarina = 0x0,
  Bow = 1,
  FireArrow = 2,
  IceArrow = 3,
  LightArrow = 4,
  Bomb = 6,
  Bombchu = 7,
  DekuStick = 8,
  DekuNuts = 9,
  MagicBean = 0xa,
  PowderKeg = 0xc,
  PictographBox = 0xd,
  LensOfTruth = 0xe,
  Hookshot = 0xf,
  GreatFairySword = 0x10,
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
  Poe = 0x1d,
  BigPoe = 0x1e,
  Water = 0x1f,
  HotSpringWater = 0x20,
  ZoraEgg = 0x21,
  GoldDust = 0x22,
  MagicalMushroom = 0x23,
  SeaHorse = 0x24,
  ChateauRomani = 0x25,
  MoonTear = 0x28,
  DekuMask = 0x32,
  GoronMask = 0x33,
  ZoraMask = 0x34,
  FierceDeityMask = 0x35,
  None = 0xff,
};

enum class Action : u8 {
  None = 0,
  Bow = 0x9,
  FireArrow = 0xa,
  IceArrow = 0xb,
  LightArrow = 0xc,
  PictographBox = 0x13,
  Ocarina = 0x14,
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
bool HasMask(ItemId item_id);

bool CanUseItem(ItemId item_id);

}  // namespace game
