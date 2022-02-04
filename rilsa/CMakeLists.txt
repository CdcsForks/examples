cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(RILSA VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE CORE_SOURCES
	mod.cpp
)

set(SOURCE_FILES ${CORE_SOURCES})
set(INCLUDE_DIRECTORIES)
set(LINK_DIRECTORIES)
set(LINK_LIBRARIES)

if (APPLE) 
	set (LINK_LIBRARIES ${LINK_LIBRARIES} "-framework Cocoa")
endif()

include(../sdk/Geode.cmake)
set_target_properties(RILSA PROPERTIES PREFIX "" OUTPUT_NAME "RILSA")
