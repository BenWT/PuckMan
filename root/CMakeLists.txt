set(PROJECT_NAME "PuckMan")

project(${PROJECT_NAME})
cmake_minimum_required(VERSION 2.8.12)

include(conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/headers/GameState.h
	src/headers/Globals.h
	src/headers/Player.h
	src/headers/Sprite.h
	src/headers/Tile.h
	src/headers/Vector2.h
)

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")

file(COPY src/assets DESTINATION bin)
