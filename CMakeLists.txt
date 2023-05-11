cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
project(Chess)

find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)

include_directories(inc)

set(HEADERS inc/stdinc.hpp inc/chessboard.hpp inc/chesspiece.hpp inc/eventsystem.hpp
            inc/game.hpp inc/gamescene.hpp inc/state.hpp inc/canvas.hpp inc/widgets.hpp
            inc/assetmanager.hpp inc/logger.hpp inc/animation.hpp inc/menu.hpp inc/math.hpp
            inc/cursor.hpp inc/prefs.hpp inc/networkclient.hpp inc/message.hpp inc/simdjson.h)
set(SOURCES src/main.cpp src/chessboard.cpp src/gamescene.cpp src/eventsystem.cpp
            src/state.cpp src/game.cpp src/assetmanager.cpp src/canvas.cpp src/widgets.cpp
            src/animation.cpp src/logger.cpp src/menu.cpp src/math.cpp src/cursor.cpp src/prefs.cpp
            src/networkclient.cpp src/message.cpp src/simdjson.cpp)

# Set the path to the assets directory
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/Assets")

# Set the destination path for the assets directory
set(ASSETS_DEST "${CMAKE_BINARY_DIR}/Assets")

# Create a custom command to copy the assets only when they have changed
add_custom_command(
    OUTPUT "${ASSETS_DEST}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_DIR}" "${ASSETS_DEST}"
    DEPENDS ${ASSETS_FILES}
    COMMENT "Copying assets to build directory"
)

# Add the assets directory as a dependency for the build
add_custom_target(copy_assets ALL DEPENDS "${ASSETS_DEST}")
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets/
#            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Assets/)

add_executable(${CMAKE_PROJECT_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
