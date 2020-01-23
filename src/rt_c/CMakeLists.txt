set(CMAKE_C_COMPILE_OBJECT
"<CMAKE_C_COMPILER> -fPIC <DEFINES> <FLAGS> -o <OBJECT> -c <SOURCE>")

# Shared library
add_library(verona_rt_c SHARED verona_rt_c.cc)
target_link_libraries(verona_rt_c verona_rt)
install(TARGETS verona_rt_c RUNTIME DESTINATION .)
set_target_properties(verona_rt_c PROPERTIES OUTPUT_NAME verona)

# Shared library w/ systematic testing
add_library(verona_rt_c_sys SHARED verona_rt_c.cc)
target_compile_definitions(verona_rt_c_sys PRIVATE USE_SYSTEMATIC_TESTING)
target_link_libraries(verona_rt_c_sys verona_rt)
install(TARGETS verona_rt_c_sys RUNTIME DESTINATION .)
set_target_properties(verona_rt_c_sys PROPERTIES OUTPUT_NAME verona-sys)

# Static library
add_library(verona_rt_c_static verona_rt_c.cc)
target_link_libraries(verona_rt_c_static verona_rt)
install(TARGETS verona_rt_c_static RUNTIME DESTINATION .)
set_target_properties(verona_rt_c_static PROPERTIES OUTPUT_NAME verona-static)

# Static library w/ systematic testing
add_library(verona_rt_c_static_sys verona_rt_c.cc)
target_compile_definitions(verona_rt_c_static_sys PRIVATE USE_SYSTEMATIC_TESTING)
target_link_libraries(verona_rt_c_static_sys verona_rt)
install(TARGETS verona_rt_c_static_sys RUNTIME DESTINATION .)
set_target_properties(verona_rt_c_static_sys PROPERTIES OUTPUT_NAME verona-sys-static)

# Example program
add_executable(verona_rt_c_test verona_rt_c_test.c)
target_link_libraries(verona_rt_c_test verona_rt_c_static)
install(TARGETS verona_rt_c_test RUNTIME DESTINATION .)
set_target_properties(verona_rt_c_test PROPERTIES OUTPUT_NAME libverona-example)

# Example program w/ systematic testing
add_executable(verona_rt_c_test_sys verona_rt_c_test.c)
target_compile_definitions(verona_rt_c_test_sys PRIVATE USE_SYSTEMATIC_TESTING)
target_link_libraries(verona_rt_c_test_sys verona_rt_c_static_sys)
install(TARGETS verona_rt_c_test_sys RUNTIME DESTINATION .)
set_target_properties(verona_rt_c_test_sys PROPERTIES OUTPUT_NAME libverona-example-sys)
