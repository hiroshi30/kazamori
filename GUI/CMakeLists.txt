# SDL
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("++ SDL2_NO_MWINDOWS ON")
	set(SDL2_NO_MWINDOWS ON)
endif()

set(SDL2_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2-2.30.3)
set(SDL2_DIR ${SDL2_PATH}/cmake)
find_package(SDL2 REQUIRED)
configure_file(
	${SDL2_PATH}/x86_64-w64-mingw32/bin/SDL2.dll
	${CMAKE_BINARY_DIR}/SDL2.dll COPYONLY
)


# SDL2_ttf
set(SDL2_ttf_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2_ttf-2.22.0)
set(SDL2_ttf_DIR ${SDL2_ttf_PATH}/cmake)
find_package(SDL2_ttf REQUIRED)
configure_file(
	${SDL2_ttf_PATH}/x86_64-w64-mingw32/bin/SDL2_ttf.dll
	${CMAKE_BINARY_DIR}/SDL2_ttf.dll COPYONLY
)


# GUI
add_library(GUI SHARED)

target_sources(GUI PRIVATE
	src/main.c
	src/button.c
	src/canvas.c
)

target_include_directories(GUI PUBLIC
	include
	src
	SDL2::SDL2
	SDL2_ttf::SDL2_ttf
)

target_link_libraries(GUI PUBLIC
	SDL2::SDL2main
	SDL2::SDL2
    SDL2_ttf::SDL2_ttf
)

target_compile_options(GUI PRIVATE -mwindows)
set_target_properties(GUI PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/
)
