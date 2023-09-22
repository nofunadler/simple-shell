cmake_minimum_required(VERSION 3.20)
project(simple_shell VERSION 0.0.1 LANGUAGES C)

if(NOT(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU"))
  message(FATAL_ERROR "unsupported target")
endif()

add_executable(sh)

target_sources(sh
  PRIVATE main.c
)

target_compile_features(sh
  PRIVATE c_std_99
)

target_compile_definitions(sh
  PRIVATE -D_XOPEN_SOURCE=500
)

target_compile_options(sh
  PRIVATE
  -Wall
  -Wextra
  -Wpedantic
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(sh
    PRIVATE
    -g
    -Og
    -fsanitize=address,undefined
  )

  target_link_options(sh
    PRIVATE
    -fsanitize=address,undefined
  )
else()
  target_compile_options(sh
    PRIVATE
    -O3
  )
endif()

target_link_libraries(sh
  PRIVATE
  util
)

add_subdirectory(lib/util)