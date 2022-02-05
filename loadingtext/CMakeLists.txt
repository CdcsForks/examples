cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(LoadingTextExample VERSION 0.1.0 LANGUAGES C CXX)

set(SOURCE_FILES main.cpp)
set(INCLUDE_DIRECTORIES)
set(LINK_DIRECTORIES)
set(LINK_LIBRARIES)

if (APPLE) 
	set (LINK_LIBRARIES ${LINK_LIBRARIES} "-framework Cocoa")
endif()

include(../sdk/Geode.cmake)
set_target_properties(LoadingTextExample PROPERTIES PREFIX "" OUTPUT_NAME "LoadingTextExample")
