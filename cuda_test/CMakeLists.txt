project(cuda_test)
cmake_minimum_required(VERSION 3.1)
add_subdirectory(cuda ./cuda/bin)
set(EXTRA_LIBS ${EXTRA_LIBS} cube)
SET(CMAKE_C_COMPILER g++)
list(APPEND CMAKE_C_FLAGS "-lstdc++")

add_executable(cuda_test main.c)
target_link_libraries(cuda_test ${EXTRA_LIBS})
