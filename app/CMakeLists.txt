cmake_minimum_required(VERSION 3.20)

project(app)

add_executable(app
  src/main.cpp
)

target_include_directories(app
PRIVATE
  ../lib/chip8/include
)

target_link_libraries(app
  chip8
)
