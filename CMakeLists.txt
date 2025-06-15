# Benjamin Eder, 210894

cmake_minimum_required(VERSION 3.21)

project(RattleRoll VERSION 1.0 LANGUAGES C)

set(CMAKE_DEFAULT_BUILD_TYPE)

if(NOT CMAKE_BUILD_TYPE)
  # Use DEBUG as default 
  set(CMAKE_BUILD_TYPE "Debug")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_C_COMPILER "/usr/bin/clang")

file(GLOB_RECURSE RATTLEROLL_SOURCES 
  ${PROJECT_SOURCE_DIR}/src/*.c
  ${PROJECT_SOURCE_DIR}/src/*.c
)

add_executable(RattleRoll ${RATTLEROLL_SOURCES})

set_property(TARGET RattleRoll PROPERTY C_STANDARD 17)
set_target_properties(RattleRoll PROPERTIES OUTPUT_NAME "rattle_roll")

target_compile_options(RattleRoll PRIVATE -Wall -Wextra -Werror)
