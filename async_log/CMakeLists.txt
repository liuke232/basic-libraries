cmake_minimum_required(VERSION 3.0.0)
project(async_log)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-Wall -Wextra -O3)

include_directories(${CMAKE_SOURCE_DIR}/inc)
include_directories(${CMAKE_SOURCE_DIR}/../lib/include)

file(GLOB SRC_LIST "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_library(
    asynclog
    SHARED
    ${SRC_LIST}
)

link_directories(${CMAKE_BINARY_DIR})
link_directories(${CMAKE_SOURCE_DIR}/../lib/static)

add_executable(main ${CMAKE_SOURCE_DIR}/test/main.cpp)

target_link_libraries(main asynclog jsonparser)