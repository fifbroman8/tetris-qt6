cmake_minimum_required(VERSION 3.21)
project(TetrisQt6 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(tetris-qt6
    src/main.cpp
    src/gamewindow.cpp
    src/gamewindow.h
    src/gameboard.cpp
    src/gameboard.h
    src/tetromino.cpp
    src/tetromino.h
)

target_link_libraries(tetris-qt6
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
