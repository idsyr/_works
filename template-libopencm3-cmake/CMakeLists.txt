cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-arm-none-eabi.cmake)
include(cmake/stm32f103.cmake)
project(template VERSION 0.1.0 LANGUAGES CXX C ASM)

add_executable(template my-project.cpp)
target_link_libraries(template stm32f103)
set_property(TARGET template PROPERTY CXX_STANDARD 17)
add_linker_script(template PRIVATE generated.stm32f103?8*.ld)
add_flash_targets(template)

