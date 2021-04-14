cmake_minimum_required(VERSION 3.5)

project(SfmlGui)

set(CMAKE_CXX_STANDARD 14)
set(INCLUDE_DIRECTORY "include")
set(SOURCES_DIRECTORY "src")


set(HEADERS
        ${INCLUDE_DIRECTORY}/Fonts.h ${INCLUDE_DIRECTORY}/Button.h ${INCLUDE_DIRECTORY}/ButtonStates.h
        ${INCLUDE_DIRECTORY}/TextBox.h ${INCLUDE_DIRECTORY}/Resources.h ${INCLUDE_DIRECTORY}/Observer.h
        ${INCLUDE_DIRECTORY}/SfmlGui.h)

set(SOURCES
        ${SOURCES_DIRECTORY}/Fonts.cpp ${SOURCES_DIRECTORY}/Button.cpp ${SOURCES_DIRECTORY}/ButtonStates.cpp
        ${SOURCES_DIRECTORY}/TextBox.cpp ${SOURCES_DIRECTORY}/Resources.cpp ${SOURCES_DIRECTORY}/Observer.cpp)

find_package(SFML 2.5.1 COMPONENTS system window graphics)

add_library(SfmlGui ${HEADERS} ${SOURCES})
target_include_directories(SfmlGui PRIVATE ${INCLUDE_DIRECTORY})
target_link_libraries (SfmlGui sfml-graphics sfml-window sfml-system)