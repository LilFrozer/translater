cmake_minimum_required(VERSION 3.10)
project(uniClipboardPrj)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(uniClipboardPrj
  src/main.cpp
)

target_include_directories(uniClipboardPrj PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
