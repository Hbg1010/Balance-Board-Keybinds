cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(wiibinds VERSION 1.0.0)

if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(SUBPROJECT YES)
endif()

option(CPACK_MONOLITHIC_INSTALL "Only produce a single component installer, rather than multi-component." NO)

###
# Perform build configuration of dependencies
###

# Locally-developed modules dist'ed with this app - always have this first.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#include(UseBackportedModules)
# include(DoxygenTargets)

if(NOT BUILD_SHARED_LIBS)
	add_definitions(-DWIIUSE_STATIC)
endif()

if(NOT WIN32 AND NOT APPLE)
	set(LINUX YES)
	find_package(Bluez REQUIRED)
	include_directories(${BLUEZ_INCLUDE_DIRS})

	include("GNUInstallDirs")
else()
	set(LINUX NO)
	set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if(WIN32)
	find_package(WinHID REQUIRED)
	include_directories(${WINHID_INCLUDE_DIRS})
	add_definitions(-D_WIN32_WINNT=0x0501)
endif()

###
# Build the project
###

# The lib is in the "src" subdirectory
add_subdirectory(include/wiiuse)

# if(NOT SUBPROJECT)
# 	# Example apps
# 	if(BUILD_EXAMPLE)
# 		add_subdirectory(example)
# 	endif()

# 	if(BUILD_EXAMPLE_SDL)
# 		add_subdirectory(example-sdl)
# 	endif()
# endif()

if(SUBPROJECT)
	set(WIIUSE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/wiiuse" PARENT_SCOPE)
	set(WIIUSE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/wiiuse" PARENT_SCOPE)
	set(WIIUSE_LIBRARY wiiuse PARENT_SCOPE)
	set(WIIUSE_LIBRARIES wiiuse PARENT_SCOPE)
	set(WIIUSE_FOUND ON PARENT_SCOPE)
endif()

if(NOT SUBPROJECT)
###
# Set packaging options (for CPack)
###

	if(WIN32)
		set(DOC_DIR .)
	else()
		set(DOC_DIR share/doc/wiiuse)

		configure_file(wiiuse.pc.in
			${CMAKE_CURRENT_BINARY_DIR}/wiiuse.pc
			@ONLY)
		install(FILES ${CMAKE_CURRENT_BINARY_DIR}/wiiuse.pc
			DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
	endif()

	# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
	set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

	include(GetCompilerInfoString)
	get_compiler_info_string(_compiler)

	set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${_compiler}")

	# Include the packaging system now that we have it all set up
	include(CPack)

	cpack_add_component(docs HIDDEN)

	cpack_add_component(development
		DISPLAY_NAME "Development Files")

	cpack_add_component(examples
		DISPLAY_NAME "Examples")

	cpack_add_component(runtime
		DISPLAY_NAME "Runtime Library"
		REQUIRED)

    file(GLOB_RECURSE SOURCES src/*.cpp)
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
	target_link_libraries(wiibinds wiiuse)

    if (NOT DEFINED ENV{GEODE_SDK})
        message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
    else()
        message(STATUS "Found Geode: $ENV{GEODE_SDK}")
    endif()

    add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

    setup_geode_mod(${PROJECT_NAME})
endif()
