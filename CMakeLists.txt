cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(
	kazamori
	VERSION 0.0.1
	LANGUAGES C
)


option(svm_building "" ON)
option(app_building "" OFF)


add_subdirectory(core)


set(SDL2_DIR ${CMAKE_SOURCE_DIR}/external/SDL2-2.28.5/cmake)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("++ SDL2_NO_MWINDOWS")
	set(SDL2_NO_MWINDOWS ON)
endif()
find_package(SDL2 REQUIRED)
configure_file(
	${CMAKE_SOURCE_DIR}/external/SDL2-2.28.5/x86_64-w64-mingw32/bin/SDL2.dll
	${CMAKE_BINARY_DIR}/SDL2.dll COPYONLY
)


if(app_building)
	message("++ APP building")
	add_executable(app)

	target_sources(app PRIVATE
		src/main.c
	)

	target_include_directories(app PRIVATE
	    ${SDL2_INCLUDE_DIRS}
	    core
	)

	target_link_libraries(app PRIVATE
	    ${SDL2_LIBRARIES}
	    core
	)

	target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic -D Debug) # -mwindows - убирает консоль при запуске *.exe
	target_compile_features(app PRIVATE c_std_23)
endif()


if(svm_building)
	message("++ SVM building")
	add_executable(svm)

	target_sources(svm PRIVATE
		svm/main.c
	)

	target_include_directories(svm PRIVATE
	    ${SDL2_INCLUDE_DIRS}
	    core
	)

	target_link_libraries(svm PRIVATE
	    ${SDL2_LIBRARIES}
	    core
	)

	target_compile_options(svm PRIVATE -Wall -Wextra -Wpedantic -D Debug) # -mwindows - убирает консоль при запуске *.exe
	target_compile_features(svm PRIVATE c_std_23)
endif()
