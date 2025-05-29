cmake_minimum_required(VERSION 3.17)

project(RayTest CXX)

find_package(RaynderCast CONFIG)

add_executable(RayTest "test.cpp")

target_link_libraries(RayTest PRIVATE RaynderCast)