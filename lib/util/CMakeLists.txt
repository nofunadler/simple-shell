add_library(util)

target_sources(util
  PRIVATE
  pipes.c
  simple_parser.c
  util.c
)

target_compile_features(util
  PRIVATE c_std_99
)

target_compile_options(util
  PRIVATE
  -Wall
  -Wextra
  -Wpedantic
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(util
    PRIVATE
    -g
    -Og
    -fsanitize=address,undefined
  )

  target_link_options(util
    PRIVATE
    -fsanitize=address,undefined
  )
else()
  target_compile_options(util
    PRIVATE
    -O3
  )
endif()

target_include_directories(util
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)