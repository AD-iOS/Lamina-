set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE RUNTIME_SRC *.cpp */*.cpp)
add_library(lmvm SHARED ${RUNTIME_SRC})
