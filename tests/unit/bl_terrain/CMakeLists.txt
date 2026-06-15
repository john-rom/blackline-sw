add_executable(test_bl_terrain
  src/test_bl_terrain.cpp
)

target_link_libraries(test_bl_terrain PRIVATE
  blackline_app
)

add_test(NAME bl_terrain COMMAND test_bl_terrain)
