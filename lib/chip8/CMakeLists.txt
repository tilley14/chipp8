cmake_minimum_required(VERSION 3.20)

project(chip8)

add_library(chip8
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
