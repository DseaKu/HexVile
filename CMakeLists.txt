cmake_minimum_required(VERSION 3.10)
project(DreamOfHexVile VERSION 1.0)

# set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master # Or a specific version tag like 5.0
)

FetchContent_MakeAvailable(raylib)

set(SOURCES
    src/main.cpp
    src/game.cpp
    src/hex_tile_grid.cpp
    src/player.cpp
    src/texture_handler.cpp
    src/font_handler.cpp
    src/ui_handler.cpp
)

add_executable(game ${SOURCES})

target_include_directories(game PRIVATE includes)

# The target_link_libraries command will now automatically
# use the fetched raylib
target_link_libraries(game PRIVATE raylib m)

add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:game>/assets
)

