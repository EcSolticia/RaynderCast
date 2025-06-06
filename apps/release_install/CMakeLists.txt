cmake_minimum_required(VERSION 3.17)

project(RaynderCast_test CXX)

find_package(RaynderCast CONFIG)

add_executable(RayTest "test.cpp")

target_link_libraries(RayTest PRIVATE RaynderCast)