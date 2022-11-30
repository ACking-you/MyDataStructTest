cmake_minimum_required(VERSION 3.22)
project(DataStructTest)

set(CMAKE_CXX_STANDARD 14)
include_directories(./third-part)
include_directories(./datastruct)

add_executable(test_avl_rb tests/test_avl_rb.cpp datastruct/AVLTree.cc)
