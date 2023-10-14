cmake_minimum_required(VERSION 3.5)

project(JanJump LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(JanJump main.cpp
    game.h game.cpp
    resource.qrc
    point.h
    gamescene.h gamescene.cpp
    view.h view.cpp)

target_link_libraries(JanJump PRIVATE Qt6::Widgets)

include(GNUInstallDirs)
install(TARGETS JanJump
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
