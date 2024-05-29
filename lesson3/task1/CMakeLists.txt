cmake_minimum_required (VERSION 3.8)
project ("task1")
add_executable (task1 "task1.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET task1 PROPERTY CXX_STANDARD 20)
endif()

