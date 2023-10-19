cmake_minimum_required(VERSION 3.5)

project(Arkanoid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(Arkanoid main.cpp
    game.h game.cpp
    resource.qrc
    level.h level.cpp
    gamescene.h gamescene.cpp
    view.h view.cpp)

target_link_libraries(Arkanoid PRIVATE Qt6::Widgets)

include(GNUInstallDirs)
install(TARGETS Arkanoid
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
