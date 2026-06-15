add_library(blackline_app STATIC
  src/bl_terrain.c
)

target_include_directories(blackline_app PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
)
