set(PROJECT_NAME "PuckMan")

project(${PROJECT_NAME})
cmake_minimum_required(VERSION 2.8.12)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/headers/Enemy.cpp
	src/headers/Enemy.h
	src/headers/FontSprite.cpp
	src/headers/FontSprite.h
	src/headers/GameState.cpp
	src/headers/GameState.h
	src/headers/Globals.cpp
	src/headers/Globals.h
	src/headers/Player.cpp
	src/headers/Player.h
	src/headers/Sprite.cpp
	src/headers/Sprite.h
	src/headers/Tile.cpp
	src/headers/Tile.h
	src/headers/Vector2.cpp
	src/headers/Vector2.h
)

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")

file(COPY src/assets DESTINATION bin)
