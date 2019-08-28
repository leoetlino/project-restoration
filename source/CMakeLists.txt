cmake_minimum_required(VERSION 3.13)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

project(restoration C CXX ASM)

add_executable(newcode
  common/context.cpp
  common/context.h
  common/debug.cpp
  common/debug.h
  common/flags.h
  common/svc.s
  common/types.h
  common/utils.h
  game/actor.cpp
  game/actor.h
  game/actors/boss_goht.h
  game/actors/boss_gyorg.h
  game/actors/boss_twinmold.h
  game/as.cpp
  game/as.h
  game/camera.cpp
  game/camera.h
  game/collision.cpp
  game/collision.h
  game/common_data.cpp
  game/common_data.h
  game/context.cpp
  game/context.h
  game/items.cpp
  game/items.h
  game/message.cpp
  game/message.h
  game/pad.cpp
  game/pad.h
  game/player.cpp
  game/player.h
  game/random.cpp
  game/random.h
  game/state.cpp
  game/state.h
  game/static_context.cpp
  game/static_context.h
  game/sound.cpp
  game/ui.cpp
  game/ui.h
  game/ui/message_window.h
  rst/fixes/boss.cpp
  rst/fixes/boss.h
  rst/fixes/boss.s
  rst/fixes/ocarina.cpp
  rst/fixes/time.cpp
  rst/fixes.cpp
  rst/fixes.h
  rst/input.cpp
  rst/link.cpp
  rst/link.h
  rst/main.cpp
  rst/trampolines.s
)
set_target_properties(newcode PROPERTIES SUFFIX ".elf")
target_compile_options(newcode PRIVATE -fdiagnostics-color=always -Wall -Wextra -std=c++17 -fno-exceptions -fomit-frame-pointer -ffunction-sections)
target_include_directories(newcode PRIVATE ./)

if (NOT DEFINED CODEADDR)
  message(FATAL_ERROR "Set CODEADDR")
endif()
target_link_options(newcode PRIVATE "-Wl,--gc-sections,-Ttext-segment=${CODEADDR}")
add_custom_target(newcode_product ALL
  DEPENDS newcode
  COMMAND ${CMAKE_OBJCOPY} -O binary newcode.elf newcode.bin
  COMMAND sh -c "${CMAKE_OBJDUMP} -t newcode.elf>newcode.sym"
  BYPRODUCTS newcode.bin newcode.sym
)

include(Version.cmake)
