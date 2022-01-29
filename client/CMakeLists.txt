cmake_minimum_required(VERSION 3.21.4)

project(client)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.78 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

file(GLOB ALL_SRCS
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    )

add_executable(client ${ALL_SRCS})
target_link_libraries(client PUBLIC pthread)
target_link_libraries(client LINK_PUBLIC ${Boost_LIBRARIES})

