set(CMAKE_C_COMPILE_OBJECT
"<CMAKE_C_COMPILER> -fPIC <DEFINES> <FLAGS> -o <OBJECT> -c <SOURCE>")

add_library(verona_rt_c
  verona_c.cc
)

target_link_libraries(verona_rt_c verona_rt)

add_executable(verona_c_test verona_c_test.c)
target_link_libraries(verona_c_test verona_rt_c)
install(TARGETS verona_c_test RUNTIME DESTINATION .)
