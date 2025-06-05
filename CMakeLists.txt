# Benjamin Eder, 210894
# Based on template

# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.15)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  Dracula
  VERSION 1.0
  LANGUAGES C)

set(CMAKE_C_COMPILER "/usr/bin/clang")

# Link each target with other targets or add options, etc.

add_executable(Adjacency adjacency.c)
set_property(TARGET Adjacency PROPERTY C_STANDARD 17)
set_target_properties(Adjacency PROPERTIES OUTPUT_NAME "adjacency.out")

add_executable(PiSearch monte_carlo_pi.c)
set_property(TARGET PiSearch PROPERTY C_STANDARD 17)
set_target_properties(PiSearch PROPERTIES OUTPUT_NAME "pi_search.out")
target_compile_options(PiSearch PRIVATE -O3)