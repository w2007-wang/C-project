cmake_minimum_required(VERSION 3.5)

project(renouqi LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        gameboard.cpp
        gameboard.h
        doll.cpp
        doll.h
        StartWindow.cpp
        StartWindow.h
        resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(renouqi
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(renouqi ${PROJECT_SOURCES})
endif()

target_link_libraries(renouqi PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(renouqi PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(renouqi)
endif()