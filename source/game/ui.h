#pragma once

#include <string_view>

#include "common/flags.h"
#include "common/types.h"

namespace game::ui {

// Classes with known, official names:
//
// core::LayoutBuilder
// core::LayoutClass
// core::Package
// core::Project
// joker::ExtraPadCheckDialog
// joker::LayoutDrawManager
// joker::LayoutFactory
// joker::LayoutManager
// joker::TextSprite

class Layout;
class LayoutBase;
class LayoutClass;
class ResAnimEntry;
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

enum class PaneType : u16 {
  Null = 0,
  Type1 = 1,
  Rect = 2,
  Text = 3,
  Pane = 4,
  PaneEx = 5,
  Pane2 = 6,
  Pane2Ex = 7,
};

template <typename T>
struct Array {
  T* data;
  size_t size;

  auto begin() const { return data; }
  auto end() const { return data + size; }
  explicit operator bool() const { return data != nullptr; }
  auto& operator[](size_t index) { return data[index]; }
  const auto& operator[](size_t index) const { return data[index]; }
};

class Pane {
public:
  virtual ~Pane();
  virtual void m1();
  virtual void m2();
  virtual void m3();
  virtual void m4();
  virtual void getVec4(Vec4* vec, Matrix34* mtx);
  virtual void m6();
  virtual void m7();

  PaneType GetType() const { return type; }
  const char* GetName() const { return name; }

private:
  PaneType type;
  const char* name;
};
static_assert(sizeof(Pane) == 0xC);

class PaneNull : public Pane {
public:
  Vec3 translate;
};
static_assert(sizeof(PaneNull) == 0x18);

class Pane1 : public Pane {
public:
  Vec3 translate;
  float z_multiplier;
};
static_assert(sizeof(Pane1) == 0x1C);

class PaneRect : public Pane {
public:
  Vec3 translate;
  float width, height;
};
static_assert(sizeof(PaneRect) == 0x20);

class TextSprite : public Pane {
public:
  u8 internal[0x300];
};
static_assert(sizeof(TextSprite) == 0x30C);

struct PaneExArg {
  Vec3 translate;
  float width, height;
  Vec2 rotate;
  Vec2 scale;
  u16 a;
  u16 b;
  Vec4 colors[4];
};
static_assert(sizeof(PaneExArg) == 0x68);

class PaneEx : public Pane {
public:
  bool enable_translate;
  void* field_10;
  PaneExArg arg;
};
static_assert(sizeof(PaneEx) == 0x7C);

struct WidgetPos {
  enum class Flag : u32 {
    Visible = 1,

    DefaultTranslateX = 0x100,
    DefaultTranslateY = 0x200,
    DefaultTranslateZ = 0x400,

    DefaultScaleX = 0x800,
    DefaultScaleY = 0x1000,
    DefaultScaleZ = 0x2000,

    DefaultRotateX = 0x4000,
    DefaultRotateY = 0x8000,
    DefaultRotateZ = 0x10000,

    Visible2 = 0x20000,

    DefaultField24_X = 0x40000,
    DefaultField24_Y = 0x80000,

    DefaultField2C_X = 0x100000,
    DefaultField2C_Y = 0x200000,

    DefaultField34 = 0x400000,

    DefaultColorR = 0x800000,
    DefaultColorG = 0x1000000,
    DefaultColorB = 0x2000000,
    DefaultColorA = 0x4000000,
    // alias for DefaultColorA
    DefaultOpacity = 0x4000000,
  };

  void SetVisible(bool visible) {
    if (flags.IsSet(Flag::Visible) == visible)
      return;
    flags.Set(Flag::Visible, visible);
    ValueChanged(Flag::Visible2, visible, true);
  }

  void SetOpacity(float opacity) {
    if (color(3) == opacity)
      return;
    color(3) = opacity;
    ValueChanged(Flag::DefaultOpacity, opacity, 1.0f);
  }

  void TranslateChanged() {
    ValueChanged(Flag::DefaultTranslateX, translate.x, 1.0f);
    ValueChanged(Flag::DefaultTranslateY, translate.y, 1.0f);
    ValueChanged(Flag::DefaultTranslateZ, translate.z, 1.0f);
  }

  void ScaleChanged() {
    ValueChanged(Flag::DefaultScaleX, scale.x, 1.0f);
    ValueChanged(Flag::DefaultScaleY, scale.y, 1.0f);
    ValueChanged(Flag::DefaultScaleZ, scale.z, 1.0f);
  }

  Vec3 translate;
  Vec3 scale;
  Vec3 field_18;
  // default (0.0, 0.0)
  Vec2 field_24;
  // default (1.0, 1.0)
  Vec2 field_2C;
  // default 0.0
  float field_34;
  // RGBA, default #ffffffff
  Vec4 color;
  rst::Flags<Flag> flags;
  rst::Flags<Flag> active_flags;

private:
  template <typename T>
  void ValueChanged(Flag flag, const T& value, const T& default_value) {
    flags.Set(flag);
    active_flags.Set(flag, value == default_value);
  }
};

static_assert(sizeof(WidgetPos) == 0x50);

enum class WidgetType {
  Group = 0,
  Layout = 1,
  MainWidget = 2,
  Pane = 3,
};

class Widget {
public:
  virtual ~Widget();
  virtual void m1();
  virtual void init0(LayoutBase* layout, const char* name);
  virtual void init(LayoutBase* layout, const char* name);
  virtual void reset();
  virtual void calc(Matrix34& mtx, Vec4& vec, Matrix23& mtx23, int, float speed);

  const char* GetName() const { return name; }
  WidgetType GetType() const;
  Widget* GetParent() const { return parent; }
  Layout* GetLayout() const { return layout; }
  Pane* GetPane() const { return pane; }

  Widget* GetWidget(std::string_view name);

  Matrix34& GetMtx() { return mtx; }
  WidgetPos& GetPos() { return pos; }
  WidgetPos& GetOldPos() { return old_pos; }

  void PrintDebug();

private:
  LayoutBase* layout_for_root_widget;
  Widget* parent;
  u32 parent_idx_maybe;
  const char* name;
  Array<Widget*> widgets;
  void* field_1C;
  Layout* layout;
  u16 main_widget_idx;
  u16 field_26;
  Pane* pane;
  Vec4 pane_vec;
  WidgetPos pos;
  WidgetPos old_pos;
  bool initialised;
  bool flag40_set;
  bool field_DE;
  bool field_DF;
  Matrix34 mtx;
  Matrix23 mtx2;
  Vec4 vec4;
  void* field_138;
};
static_assert(sizeof(Widget) == 0x13C);

class MainWidget {
public:
  virtual ~MainWidget();
  Widget widget;
};

class AnimEntry {
public:
  virtual ~AnimEntry();

  ResAnimEntry* GetData() { return data; }

private:
  ResAnimEntry* data;
};

class Anim {
public:
  virtual ~Anim();

  const char* GetName() const { return name; }
  Array<AnimEntry*>& GetEntries() { return entries; }

private:
  float fps = 60.0;
  const char* name = nullptr;
  int min_frame = 0;
  Array<AnimEntry*> entries;
};
static_assert(sizeof(Anim) == 0x18);

class AnimPlayer {
public:
  virtual ~AnimPlayer();

  const char* GetName() const { return name; }
  Anim* GetAnim() const { return anim; }

private:
  const char* name = nullptr;
  bool x8 = false;
  bool x9 = false;
  bool xa = false;
  Anim* anim = nullptr;
  float frame = 0.0;
  float speed = 1.0;
};
static_assert(sizeof(AnimPlayer) == 0x18);

class LayoutBase {
public:
  virtual ~LayoutBase();
  virtual void m1();
  virtual void init(LayoutClass*, MainWidget** main_widgets, int num_main_widgets,
                    AnimPlayer** players, int num_players, const char* name);
  virtual void m3();
  virtual void calc(Matrix34& mtx, Vec4& vec, Matrix23& mtx2, int, float speed);

  void calc(float speed = 0.033333);

  const char* GetName() const { return name; }
  Widget* GetRootWidget() { return &root_widget; }
  const Array<Widget*>& GetWidgets() const { return widgets; }
  const Array<AnimPlayer*>& GetAnimPlayers() const { return players; }
  AnimPlayer* GetAnimPlayer(std::string_view name) const;
  Widget* GetWidget(std::string_view name);
  Pane* GetPane(std::string_view name) const;

private:
  LayoutClass* cl;
  Widget root_widget;
  Array<Widget*> main_widgets;
  Array<AnimPlayer*> players;
  Array<Widget*> widgets;
  Array<Pane*> panes;
  u8 field_164;
  const char* name;
};
static_assert(sizeof(LayoutBase) == 0x16C);

class Layout : public LayoutBase {
private:
  void* field_16C;
};
static_assert(sizeof(Layout) == 0x170);

class LayoutMgr {
public:
  static LayoutMgr& Instance();

  Layout* MakeLayout(int id, int x = 0x30000);
  Layout* MakeLayout(const char* name);
  template <typename T>
  T* MakeLayout(const char* name) {
    return static_cast<T*>(MakeLayout(name));
  }

  void FreeLayout(Layout* layout);
};

class Project {
public:
  static Project& Instance();

  int GetLayoutId(const char* name);

  int GetPackageId(const char* name);
  bool LoadPackage(int id, bool x = true);
  bool UnloadPackage(int id);
  bool IsLoading() const;
};

class Font {
  u8 internal[0x14];
};

class LayoutDrawMgr {
public:
  static LayoutDrawMgr& Instance();
  void ControlLayout(Layout* layout, int a, int b);

private:
  u8 data[0x140];
};

}  // namespace game::ui
