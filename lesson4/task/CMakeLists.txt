cmake_minimum_required (VERSION 3.8)
project ("task")
add_executable (task "task.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET task PROPERTY CXX_STANDARD 20)
endif()

