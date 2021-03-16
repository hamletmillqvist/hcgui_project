cmake_minimum_required (VERSION 3.0.0)

# Project settings
project (
    HCGUI
    VERSION 0.1
    DESCRIPTION "Console Grapchical User Interface"
)

set(HCGUI_DIR ${CMAKE_CURRENT_BINARY_DIR}/..)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${HCGUI_DIR}/bin)

# Store include directory
include_directories(${HCGUI_DIR}/include)

# Add project important files
file(GLOB_RECURSE HEADERS "${HCGUI_DIR}/include/hcgui/*.h")
file(GLOB_RECURSE SOURCES "${HCGUI_DIR}/source/*.cpp")

# Generate DLL file and make DLL available for all example applications
add_library(hcgui SHARED ${SOURCES} ${HEADERS})
target_include_directories(hcgui PUBLIC ${HCGUI_DIR}/bin)

# Compile example applications
add_subdirectory(${HCGUI_DIR}/examples ${CMAKE_SOURCE_DIR}/examples)
