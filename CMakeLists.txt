cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(Chess)

find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)

include_directories(inc)

set(HEADERS inc/stdinc.hpp inc/chessboard.hpp inc/chesspiece.hpp inc/eventsystem.hpp
            inc/game.hpp inc/gamescene.hpp inc/state.hpp inc/canvas.hpp inc/widgets.hpp
            inc/assetmanager.hpp inc/logger.hpp inc/animation.hpp inc/menu.hpp inc/cursor.hpp)
set(SOURCES src/main.cpp src/chessboard.cpp src/gamescene.cpp src/eventsystem.cpp
            src/state.cpp src/game.cpp src/assetmanager.cpp src/canvas.cpp src/widgets.cpp
            src/animation.cpp src/logger.cpp src/menu.cpp src/cursor.cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets/
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Assets/)

add_executable(${CMAKE_PROJECT_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
