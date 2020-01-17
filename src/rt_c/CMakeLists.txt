set(CMAKE_C_COMPILE_OBJECT
"<CMAKE_C_COMPILER> -fPIC <DEFINES> <FLAGS> -o <OBJECT> -c <SOURCE>")

add_library(verona_rt_c
  verona_c.cc
)

target_link_libraries(verona_rt_c verona_rt)

add_executable(rt_c_test test.c)
target_link_libraries(rt_c_test verona_rt_c)
install(TARGETS rt_c_test RUNTIME DESTINATION .)
