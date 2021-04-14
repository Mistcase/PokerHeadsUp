cmake_minimum_required(VERSION 3.17)
project(PokerHeadsUp)

set(CMAKE_CXX_STANDARD 14)

set(NETBOOST_INCLUDE_DIRECTORY "../Netboost")
set(SFML_GUI_INCLUDE_DIRECTORY "../SfmlGui/include")
set(INCLUDE_DIRECTORY "include")
set(SOURCES_DIRECTORY "src")

set(HEADERS ${INCLUDE_DIRECTORY}/Application.h ${INCLUDE_DIRECTORY}/NetClient.h
        ${INCLUDE_DIRECTORY}/Cards.h ${INCLUDE_DIRECTORY}/CombinationIdentifier.h ${INCLUDE_DIRECTORY}/Deck.h
         ${INCLUDE_DIRECTORY}/GameState.h ${INCLUDE_DIRECTORY}/MainMenuState.h
        ${INCLUDE_DIRECTORY}/Notifications.h ${INCLUDE_DIRECTORY}/Player.h 
        ${INCLUDE_DIRECTORY}/PokerButton.h
        ${INCLUDE_DIRECTORY}/Settings.h ${INCLUDE_DIRECTORY}/State.h ${INCLUDE_DIRECTORY}/TableSlots.h
        ${INCLUDE_DIRECTORY}/MessageBox.h)

set(SOURCES
        ${SOURCES_DIRECTORY}/Application.cpp ${SOURCES_DIRECTORY}/NetClient.cpp
        ${SOURCES_DIRECTORY}/Cards.cpp ${SOURCES_DIRECTORY}/CombinationIdentifier.cpp 
        ${SOURCES_DIRECTORY}/GameState.cpp ${SOURCES_DIRECTORY}/MainMenuState.cpp
        ${SOURCES_DIRECTORY}/Notifications.cpp ${SOURCES_DIRECTORY}/Player.cpp 
        ${SOURCES_DIRECTORY}/PokerButton.cpp
        ${SOURCES_DIRECTORY}/State.cpp
        ${SOURCES_DIRECTORY}/Deck.cpp ${SOURCES_DIRECTORY}/MessageBox.cpp ${SOURCES_DIRECTORY}/main.cpp)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

add_executable(PokerHeadsUp ${HEADERS} ${SOURCES})

target_include_directories(PokerHeadsUp PRIVATE ${INCLUDE_DIRECTORY} ${NETBOOST_INCLUDE_DIRECTORY} ${SFML_GUI_INCLUDE_DIRECTORY})

add_library(Netboost STATIC IMPORTED)
set_property(TARGET Netboost PROPERTY
             IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../Netboost/build/libNetboost.a)

add_library(SfmlGui STATIC IMPORTED)
set_property(TARGET SfmlGui PROPERTY
             IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../SfmlGui/build/libSfmlGui.a)

target_link_libraries (PokerHeadsUp Netboost SfmlGui sfml-graphics sfml-window sfml-system)

install(DIRECTORY Data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Data)