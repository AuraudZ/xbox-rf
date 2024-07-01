cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(xbox-rf C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(rf
  rf.c
)
pico_enable_stdio_usb(rf 1)
pico_enable_stdio_uart(rf 1)
pico_add_extra_outputs(rf)
target_link_libraries(rf pico_stdlib)