#pragma once

#include "common/flags.h"
#include "common/types.h"
#include "game/ui.h"

namespace game {

class Allocator;
class FileEntity;
namespace ui {
class MessageWindow;
}

enum class Language : u32 {
  JpJp = 0,
  UsEn = 1,
  UsFr = 2,
  UsEs = 3,
  EuEn = 4,
  EuDe = 5,
  EuFr = 6,
  EuEs = 7,
  EuIt = 8,
  EuNl = 9,
  TwCn = 0xA,
  TwZh = 0xB,
  TwEn = 0xC,
  KrKr = 0xD,
};
constexpr size_t NumLanguages = 0xE;

struct MessageResHeader {
  /// Must be 'GMSG'
  int magic;
  /// Must be 1
  u16 version_major;
  /// Must be 0
  u16 version_minor;
  /// Must be non zero
  int num_msgs;
  /// Must be non zero. Determines the size of a message entry.
  rst::BitSet<32, u32, Language> languages;
};

struct MessageResText {
  /// Offset to the message text relative to the start of the file.
  u32 offset;
  /// Length (including null terminator)
  u32 length;
};

struct MessageResEntry {
  /// Message ID.
  u16 id;
  u16 field_2;
  u32 field_4;
  u32 flags;
  // Variable length array. Length is equal to the number of languages in the header.
  MessageResText texts[1];
};

struct MessageReader;

struct MessageText {
  MessageReader* reader;
  const void* ptr;
  u32 size;
};

struct Message {
  u32 msgid;
  bool is_flag1;
  bool is_flag1000;
  bool is_flag2000;
  u8 gap_7;
  u32 flag10;
  u32 flag100;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
  int field_20;
  MessageText texts[NumLanguages];
};
static_assert(sizeof(Message) == 0xCC);

struct MessageData {
  bool Get(u16 id, Message* out) const;

  void* vtable;
  MessageResHeader* res_header;
  MessageResEntry* res_entries;
  u32 res_size;
  u32 res_entry_size;
};

struct MessageParser {
  virtual void start(void* ctx, MessageText* text);
  virtual void end(int, int);
  virtual void copy(void*);
  virtual void cmd1();
  virtual void cmd2();
  virtual void cmd3();
  virtual void cmd4();
  virtual void cmd5();
  virtual void cmd6();
  virtual void cmd7();
  virtual void cmd8();
  virtual void cmd9();
  virtual void cmdA();
  virtual void cmdB();
  virtual void cmdC();
  virtual void cmdD();
  virtual void cmdE();
  virtual void cmdF();
  virtual void cmd10();
  virtual void cmd11(u32);
  virtual void cmd12();
  virtual void cmd13();
  virtual void cmd14();
  virtual void cmd15(u32);
  virtual void cmd16();
  virtual void cmd17();
  virtual void cmd18();
  virtual void cmd19();
  virtual void cmd1A();
  virtual void cmd1E();
  virtual void cmd1F();
  virtual void cmd1B();
  virtual void cmd1C(u32);
  virtual void cmd1d();
  virtual void cmd20(u32, char*, char*);
  virtual void cmd21();
  virtual void cmd22(u32);
  virtual void cmd23();
  virtual void cmd24();
  virtual void cmd25(u32);
  virtual void cmd26(u32);
  virtual void cmd27();
  virtual void cmd28();
  virtual void cmd29(u32, u32);
  virtual void cmd2B(bool);
  virtual void cmd2C();
  virtual void cmd2D(u32);
  virtual void cmd2E();
  virtual void cmd2F(u32);
  virtual void cmd30();
  virtual void cmd31(bool);
  virtual void cmd32(u32);
  virtual void cmd2A(u32);
  virtual void cmd33();
  virtual void cmd34();
  virtual void cmd35();
  virtual void cmd36();
  virtual void cmd37();
  virtual void cmd38();
  virtual void cmd39();
  virtual void cmd3A(u32);
  virtual void cmd3B(char*);
  virtual void cmd3C();
  virtual void cmd3D();

  MessageData data;
  int field_1C;
  int total_size;
  int used_size;
  u32 field_28;
  u32 field_2C;
  void* ctx;
  int field_34;
  u32 field_38;
  u32 lang;
  u32 field_40;
  char field_44;
  char field_45;
  char field_46;
  char field_47;
  int field_48;
};

struct MessageSet {
  void* vtable;
  MessageParser parser;
};

struct MessageMgr {
  static MessageMgr& Instance();

  struct File {
    FileEntity* entity;
    void* data;
    int size;
  };

  u32 handlers;
  File file_msg;
  File file_font;
  File file_font_jpn8;
  Language lang;
  ui::MessageWindow* message_window;
  u32 field_30;
  int field_34;
  MessageSet message_set;
  ui::Font font;
  ui::Font font_jpn8;
  u32 message_buffer;
  Allocator* allocator;
  u8 initialised;
  u8 gap_B5[3];
  float field_B8[16];
  u8 gap_F8[4][64];
  u32 field_1F8;
  u32 field_1FC;
  u32 field_200;
  u32 field_204;
  int field_208;
};
static_assert(sizeof(MessageMgr) == 0x20C);

void PlayMessagePassSound();
void PlayMessageCancelSound();

}  // namespace game
