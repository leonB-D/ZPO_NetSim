cmake_minimum_required(VERSION 3.27)
project(ZPO_NetSim)

set(CMAKE_CXX_STANDARD 17)

include_directories(

        include
)

set(SOURCE_FILES
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        src/storage_types.cpp
        src/factory.cpp
)

add_executable(netsim ${SOURCE_FILES} main_debug.cpp)

# GTEST

add_subdirectory(../googletest-1.17.0 cmake-build-gtest)

set(SOURCE_FILES_TEST
        tests/tests_package.cpp
        tests/tests_storage_types.cpp
        tests/tests_nodes.cpp
)

add_executable(netsim_test ${SOURCE_FILES} ${SOURCE_FILES_TEST} main_gtest.cpp)
target_include_directories(netsim_test PUBLIC
        ../googletest-1.17.0/googletest/include
        ../googletest-1.17.0/googlemock/include
)
target_link_libraries(netsim_test
        gtest
)
