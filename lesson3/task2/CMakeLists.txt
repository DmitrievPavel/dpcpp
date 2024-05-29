cmake_minimum_required (VERSION 3.8)
project ("task2")
add_executable (task2 "task2.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET task2 PROPERTY CXX_STANDARD 20)
endif()

