cmake_minimum_required(VERSION 3.12)

project(RayTest CXX)

add_executable(${PROJECT_NAME} "test.cpp")

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../build/export
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../build/libRaynderCastd.so.0
)