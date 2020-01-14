cmake_minimum_required(VERSION 3.0.0)

project(
  Volta
  VERSION 1.0.0
  DESCRIPTION "Volatile read and write for C++"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

add_library(volta volta/volta.cc)
target_include_directories(volta PUBLIC "")
