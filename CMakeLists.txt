cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(kazamori
	VERSION 0.0.1
	DESCRIPTION "Machine learning and deep learning library"
	LANGUAGES C
)


option(BUILD_MUL_CLASS_TEST "Building multiclass classification test" ON)
option(BUILD_DIG_RECOG_TEST "Building digit recognition test" ON)


set(CMAKE_C_STANDARD 23 CACHE STRING "C standart of 2023 year")
set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -D${CMAKE_BUILD_TYPE}")
message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	message("Building with -mwindows")
	string(APPEND CMAKE_C_FLAGS " -mwindows")
endif()
message("CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")


# core
add_subdirectory(core)


# GUI
if (BUILD_MUL_CLASS_TEST OR BUILD_DIG_RECOG_TEST)
	add_subdirectory(GUI)
endif()


# Multiclass classification
if (BUILD_MUL_CLASS_TEST)
	add_executable(MUL_CLASS)

	target_sources(MUL_CLASS PRIVATE
		mul_class/main.c
	)

	target_include_directories(MUL_CLASS PRIVATE
	    core
	    GUI
	)

	target_link_libraries(MUL_CLASS PRIVATE
	    core
	    GUI
	)
endif()


# Digit recognition
if (BUILD_DIG_RECOG_TEST)
	add_executable(DIG_RECOG)

	target_sources(DIG_RECOG PRIVATE
		dig_recog/main.c
	)

	target_include_directories(DIG_RECOG PRIVATE
	    core
	    GUI
	)

	target_link_libraries(DIG_RECOG PRIVATE
	    core
	    GUI
	)
endif()
