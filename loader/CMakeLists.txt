cmake_minimum_required(VERSION 3.13)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

project(loader C ASM)

add_executable(loader
  source/loader.c
  source/svc.s
)
set_target_properties(loader PROPERTIES SUFFIX ".elf")
target_compile_options(loader PRIVATE -Os -fomit-frame-pointer -ffunction-sections)

if (NOT DEFINED CODEADDR OR NOT DEFINED DATAADDR)
  message(FATAL_ERROR "Set CODEADDR and DATAADDR")
endif()
configure_file(linker_template.ld linker.ld)
target_link_options(loader PRIVATE "-Wl,-Tlinker.ld")
add_custom_target(loader_product ALL
  DEPENDS loader
  COMMAND ${CMAKE_OBJCOPY} -O binary loader.elf loader.bin
  COMMAND sh -c "${CMAKE_OBJDUMP} -t loader.elf>loader.sym"
  BYPRODUCTS loader.bin loader.sym
)
