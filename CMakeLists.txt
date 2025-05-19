cmake_minimum_required(VERSION 3.12)

project(RaynderCast CXX)

set(CMAKE_DEBUG_POSTFIX
    d
)

find_package(SDL2 CONFIG REQUIRED)

add_library(${PROJECT_NAME} SHARED
    src/raynder.cpp
    src/player.cpp
    src/renderer.cpp
    src/map.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/raynder
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    SDL2::SDL2
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_BUILD)
else()
    add_definitions(-DRELEASE_BUILD)
endif()