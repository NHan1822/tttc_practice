cmake_minimum_required(VERSION 3.20)
project(MarkUnusedVariables)

find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

include(AddLLVM)

add_llvm_library(MarkUnusedVariables MODULE MarkUnusedVariables.cpp PLUGIN_TOOL Clang)
