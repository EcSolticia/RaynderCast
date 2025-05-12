cmake_minimum_required(VERSION 3.12)

project(RaynderCast CXX)

find_package(SDL2 REQUIRED)

add_library(${PROJECT_NAME} STATIC
    src/raynder.cpp
    src/game/game.cpp
    src/game/player.cpp
    src/game/renderer.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/raynder
    PRIVATE ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE SDL2
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)