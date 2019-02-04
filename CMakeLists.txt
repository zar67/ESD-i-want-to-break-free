cmake_minimum_required(VERSION 3.9.2)
include(CMake/platform.cmake)
project(Breakout)

## set to true to use dlls instead of static libs ##
set(BUILD_SHARED_LIBS false)

## itch.io and gamedata settings ##
set(GAMEDATA_FOLDER "GameData")
set(ITCHIO_USER     "")

## files used to build this game
add_executable(
        ${PROJECT_NAME}
        "Source/main.cpp"
        "Source/Game.h"
        "Source/Game.cpp"
        "Source/Components/GameObject.h"
        "Source/Components/GameObject.cpp"
        "Source/Components/SpriteComponent.h"
        "Source/Components/SpriteComponent.cpp"
        "Source/Utility/Rect.h"
        "Source/Utility/Rect.cpp"
        "Source/Utility/Vector2.h"
        "Source/Utility/Vector2.cpp" )

## utility scripts
set(ENABLE_SOUND OFF CACHE BOOL "Adds SoLoud to the Project" FORCE)
include(CMake/compilation.cmake)
include(CMake/datpak.cmake)
