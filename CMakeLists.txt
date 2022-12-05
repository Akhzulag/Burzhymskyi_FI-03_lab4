cmake_minimum_required(VERSION 3.21)
project(Burzhymskyi_FI_03_lab4)

add_subdirectory(./googletest-main)
set(LIBRARIES
        gtest
        pthread)

set(CMAKE_CXX_STANDARD 14)

add_executable(Burzhymskyi_FI_03_lab4 Test.cpp GF2.cpp GF2.h )
target_link_libraries(Burzhymskyi_FI_03_lab4 gtest gtest_main)