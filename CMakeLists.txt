cmake_minimum_required(VERSION 3.17)

project(
    RaynderCast 
    VERSION 0.1.0
    LANGUAGES
    CXX
)

set(CMAKE_DEBUG_POSTFIX
    d
)

find_package(SDL2 CONFIG REQUIRED)

add_library(${PROJECT_NAME} SHARED)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
    PROPERTIES VERSION ${PROJECT_VERSION}
               SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_sources(${PROJECT_NAME} PRIVATE
    src/raynder.cpp
    src/player.cpp
    src/map.cpp
    src/raycaster.cpp

    src/renderer/renderer.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    SDL2::SDL2
)

set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY CXX_VISIBILITY_PRESET "hidden"
)
set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY VISIBILITY_INLINES_HIDDEN TRUE
)

include(GenerateExportHeader)

generate_export_header(
    ${PROJECT_NAME}
    EXPORT_FILE_NAME
    export/raynder/export_raynder.h
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/export>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>   
)

include(GNUInstallDirs)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT "${PROJECT_NAME}Targets"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/export/raynder/export_raynder.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/raynder
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "RaynderCastConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/RaynderCastConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/RaynderCastConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/raynder
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/RaynderCastConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/RaynderCastConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/raynder
)

install(
    EXPORT RaynderCastTargets
    FILE RaynderCastTargets.cmake
    NAMESPACE RaynderCast::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/raynder
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_BUILD)
else()
    add_definitions(-DRELEASE_BUILD)
endif()