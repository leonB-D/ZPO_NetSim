cmake_minimum_required(VERSION 3.27)
project(ZPO_NetSim)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        include
)

set(SOURCE_FILES
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        src/storage_types.cpp
        src/tests.cpp
        src/factory.cpp
)

add_executable(netsim ${SOURCE_FILES} main.cpp)
