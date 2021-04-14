cmake_minimum_required(VERSION 3.5)

project(UniversalPokerServer)
set(CMAKE_CXX_STANDARD 14)

set(NETBOOST_INCLUDE_DIRECTORY "../Netboost")
set(INCLUDE_DIRECTORY "include")
set(SOURCES_DIRECTORY "src")

set(HEADERS
        ${INCLUDE_DIRECTORY}/Server.h ${INCLUDE_DIRECTORY}/PokerLogicServer.h ${INCLUDE_DIRECTORY}/Player.h
        ${INCLUDE_DIRECTORY}/RingQueue.h ${INCLUDE_DIRECTORY}/Notifications.h)
set(SOURCES
        ${SOURCES_DIRECTORY}/main.cpp ${SOURCES_DIRECTORY}/Server.cpp ${SOURCES_DIRECTORY}/PokerLogicServer.cpp
        ${SOURCES_DIRECTORY}/Player.cpp ${SOURCES_DIRECTORY}/RingQueue ${SOURCES_DIRECTORY}/Notifications.cpp)

add_library(Netboost STATIC IMPORTED)
set_property(TARGET Netboost PROPERTY
             IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../Netboost/build/libNetboost.a)

add_executable(PokerServer ${HEADERS} ${SOURCES})
target_include_directories(PokerServer PRIVATE ${INCLUDE_DIRECTORY} ${NETBOOST_INCLUDE_DIRECTORY})
target_link_libraries (PokerServer Netboost)