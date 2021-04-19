cmake_minimum_required(VERSION 3.17)
project(PokerHeadsUp)

set(CMAKE_CXX_STANDARD 17)

set(NETBOOST_INCLUDE_DIRECTORY "../Netboost")
set(SFML_GUI_INCLUDE_DIRECTORY "../SfmlGui/include")
set(INCLUDE_DIRECTORY "include")
set(SOURCES_DIRECTORY "src")

set(HEADERS ${INCLUDE_DIRECTORY}/Application.h ${INCLUDE_DIRECTORY}/NetClient.h
         ${INCLUDE_DIRECTORY}/GameState.h ${INCLUDE_DIRECTORY}/MainMenuState.h
        ${INCLUDE_DIRECTORY}/Notifications.h ${INCLUDE_DIRECTORY}/Player.h 
        ${INCLUDE_DIRECTORY}/PokerButton.h
        ${INCLUDE_DIRECTORY}/Settings.h ${INCLUDE_DIRECTORY}/State.h ${INCLUDE_DIRECTORY}/TableSlots.h
        ${INCLUDE_DIRECTORY}/MessageBox.h)

set(SOURCES
        ${SOURCES_DIRECTORY}/Application.cpp ${SOURCES_DIRECTORY}/NetClient.cpp
        ${SOURCES_DIRECTORY}/GameState.cpp ${SOURCES_DIRECTORY}/MainMenuState.cpp
        ${SOURCES_DIRECTORY}/Notifications.cpp ${SOURCES_DIRECTORY}/Player.cpp 
        ${SOURCES_DIRECTORY}/PokerButton.cpp ${SOURCES_DIRECTORY}/State.cpp ${SOURCES_DIRECTORY}/MessageBox.cpp
        ${SOURCES_DIRECTORY}/main.cpp)


if (APPLE)
find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)
add_library(Netboost STATIC IMPORTED)
set_property(TARGET Netboost PROPERTY
             IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../Netboost/build/libNetboost.a)
			 
add_library(SfmlGui STATIC IMPORTED)
set_property(TARGET SfmlGui PROPERTY
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../SfmlGui/build/libSfmlGui.a)

target_link_libraries (PokerHeadsUp Netboost SfmlGui sfml-graphics sfml-window sfml-system)

install(DIRECTORY Data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Data)
endif(APPLE)
#############################################################################################
if (WIN32)
	add_library(Netboost STATIC IMPORTED)
set_property(TARGET Netboost PROPERTY
             IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../Netboost/build/Release/Netboost.lib)
			 
	#add SfmlGui
	add_library(SfmlGui STATIC IMPORTED)
	set_property(TARGET SfmlGui
 			 PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../SfmlGui/build/Release/SfmlGui.lib)
	
	#Sfml headers
	set(SFML_DIRECTORY_PATH ../SFML_Win)
	set(SFML_INCLUDE_DIRECTORY ${SFML_DIRECTORY_PATH}/include)
	set(SFML_BIN_DIRECTORY ${SFML_DIRECTORY_PATH}/bin)
	set(SFML_GUI_INCLUDE_DIRECTORY ../SfmlGui/include)
	set(SFML_GUI_LIBRARY_DIRECTORY ../SfmlGui/build/libSfmlGui/Release/)
	
	#add sfml libraries
	add_library(Graphics STATIC IMPORTED)
	set_property(TARGET Graphics
 			 PROPERTY IMPORTED_LOCATION sfml-graphics.lib)

	add_library(System STATIC IMPORTED)
	set_property(TARGET System
 			 PROPERTY IMPORTED_LOCATION sfml-system.lib)

	add_library(Window STATIC IMPORTED)
	set_property(TARGET Window
 			 PROPERTY IMPORTED_LOCATION sfml-window.lib)
	#add sfml_libraries
	
	add_executable(PokerHeadsUp ${HEADERS} ${SOURCES})
	target_link_directories(PokerHeadsUp PUBLIC ${SFML_DIRECTORY_PATH}/lib/)
	target_link_libraries(PokerHeadsUp Netboost SfmlGui Graphics System Window)
	install(DIRECTORY Data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/Data)
	install(FILES ${SFML_BIN_DIRECTORY}/sfml-graphics-2.dll
			  ${SFML_BIN_DIRECTORY}/sfml-system-2.dll
			  ${SFML_BIN_DIRECTORY}/sfml-window-2.dll
			  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/)
	
endif(WIN32)



#add_executable(PokerHeadsUp ${HEADERS} ${SOURCES})

target_include_directories(PokerHeadsUp PRIVATE ${SFML_INCLUDE_DIRECTORY} ${INCLUDE_DIRECTORY} ${NETBOOST_INCLUDE_DIRECTORY} ${SFML_GUI_INCLUDE_DIRECTORY})