cmake_minimum_required(VERSION 3.15)
project(LMX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
add_executable(lm main.cpp common/repl.cpp common/file_run.cpp)
add_subdirectory(compiler)
add_subdirectory(runtime)
target_link_libraries(lm lmc lmvm )
