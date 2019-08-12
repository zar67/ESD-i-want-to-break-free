cmake_minimum_required(VERSION 3.9.2)
include(CMake/platform.cmake)
project(Breakout)

## out of source builds ##
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/bin")

## set to true to use dlls instead of static libs ##
set(BUILD_SHARED_LIBS false)

## itch.io and gamedata settings ##
set(GAMEDATA_FOLDER "GameData")
set(ITCHIO_USER     "")

## files used to build this game
add_executable(
        ${PROJECT_NAME}
        main.cpp Source/Game.cpp Source/Game.h
        Source/Utility/Vector2.cpp Source/Utility/Vector2.h
        Source/Utility/Circle.cpp Source/Utility/Circle.h
        Source/Utility/Rectangle.cpp Source/Utility/Rectangle.h
        Source/Components/GameObject.cpp Source/Components/GameObject.h
        Source/Components/SpriteComponent.cpp Source/Components/SpriteComponent.h
        Source/Components/MovementComponent.cpp Source/Components/MovementComponent.h
        Source/Handlers/CollisionHandler.cpp Source/Handlers/CollisionHandler.h
        Source/Handlers/UIHandler.cpp Source/Handlers/UIHandler.h
        Source/GameObjects/Ball.cpp Source/GameObjects/Ball.h
        Source/GameObjects/Player.cpp Source/GameObjects/Player.h
        Source/LevelData/GameConstants.h
        Source/LevelData/Level.cpp Source/LevelData/Level.h)

## utility scripts
set(ENABLE_SOUND OFF CACHE BOOL "Adds SoLoud to the Project" FORCE)
include(CMake/compilation.cmake)
include(CMake/datpak.cmake)
