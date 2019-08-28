#pragma once

#include "common/types.h"

namespace game::ui {

class Screen;

enum class ScreenType {
  Black,
  Main,
  Schedule,
  Quest,
  Items,
  Map,
  Masks,
  /// "Play your instrument" screen
  Ocarina,
};
Screen* GetScreen(ScreenType screen);
bool OpenScreen(ScreenType screen);

bool CheckCurrentScreen(ScreenType screen);

class Layout {
public:
  void Calc();
  u8 internal[0x170];
};

class LayoutFile;

class LayoutMgr {
public:
  static LayoutMgr& Instance();

  Layout* MakeLayout(LayoutFile* file, int x = 0x30000);
  Layout* MakeLayout(const char* name);
  template <typename T>
  T* MakeLayout(const char* name) {
    return static_cast<T*>(MakeLayout(name));
  }

  void FreeLayout(Layout* layout);
};

class PackageMgr {
public:
  static PackageMgr& Instance();

  LayoutFile* GetLayoutFile(const char* name);

  int GetHandle(const char* name);
  bool LoadPackage(int handle, bool x = true);
  bool UnloadPackage(int handle);
  bool IsLoading() const;
};

class Font {
  u8 internal[0x14];
};

}  // namespace game::ui
