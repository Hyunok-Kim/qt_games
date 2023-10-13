cmake_minimum_required(VERSION 3.5)

project(Tetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

qt_standard_project_setup()

qt_add_executable(Tetris main.cpp
    resource.qrc
    point.h
    game.h game.cpp
    menuscene.h menuscene.cpp
    settingsscene.h settingsscene.cpp
    gamescene.h gamescene.cpp
    view.h view.cpp)

target_link_libraries(Tetris PRIVATE Qt6::Widgets Qt6::Multimedia)

include(GNUInstallDirs)
install(TARGETS Tetris
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
