cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(balance-board VERSION 1.0.0)
set(WIIUSE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/wiiuse/src")
set(WIIUSE_LIB "/include/wiiuse/src/lib/libwiiuse.so") # or .a for static

file(GLOB_RECURSE SOURCES src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Add main project library
# add_library(${PROJECT_NAME} SHARED src/main.cpp)

# Ensure Geode SDK is found
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

# Add Geode SDK
add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Setup Geode Mod
setup_geode_mod(${PROJECT_NAME})