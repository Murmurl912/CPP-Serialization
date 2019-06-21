cmake_minimum_required(VERSION 3.14)
project(Serialization)

set(CMAKE_CXX_STANDARD 14)

add_executable(Serialization main.cpp tinyxml2.h tinyxml2.cpp serialization/serializable.h serialization/serializable.cpp)
