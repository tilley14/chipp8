cmake_minimum_required(VERSION 3.20)

project(test)

add_executable(test
  src/main.cpp
  src/unittest.cpp
)

target_include_directories(test
PRIVATE
  ../src
)
