cmake_minimum_required(VERSION 3.15)
project(LMX)

set(CMAKE_CXX_STANDARD 20)

# Add MSVC compatibility support
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

# Define DLL export macros
if(BUILD_SHARED_LIBS OR WIN32)
    add_definitions(-DLMX_DLL)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
add_executable(lm main.cpp common/repl.cpp common/file_run.cpp)
add_subdirectory(compiler)
add_subdirectory(runtime)
target_link_libraries(lm lmc lmvm )
