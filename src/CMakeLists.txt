cmake_minimum_required(VERSION 3.25)

project(PapercraftSpellCaster CXX)

include(FetchContent)

# Require the C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 10.0.0
)

FetchContent_Declare(
  cmark-gfm
  GIT_REPOSITORY https://github.com/github/cmark-gfm.git
  GIT_TAG 0.29.0.gfm.12
)

FetchContent_MakeAvailable(cmark-gfm)
FetchContent_MakeAvailable(fmt)

add_executable(
  main
  main.cpp
  common.cpp
)
target_include_directories(
  main
  PRIVATE
  ${cmark-gfm_SOURCE_DIR}/src
  ${cmark-gfm_BINARY_DIR}/src
)
target_link_libraries(
  main
  PRIVATE
  fmt::fmt
  libcmark-gfm
  libcmark-gfm-extensions
)
