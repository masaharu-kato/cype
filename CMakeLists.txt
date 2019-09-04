cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
include_directories("src")

project(array_test)
project(typed_set_test)
project(typed_array_test)
project(typed_vector_test)

add_subdirectory("test")
