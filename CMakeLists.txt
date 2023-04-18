cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(USE_FOLDERS ON)
project(ProjetFinAnnee)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(libmaths)

set(LIBGL_BUILD_DEMO OFF)
add_subdirectory(libGL)

add_subdirectory(external)
add_subdirectory(game)

if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT game)
endif()