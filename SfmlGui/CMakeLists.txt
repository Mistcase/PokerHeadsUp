cmake_minimum_required(VERSION 3.5)

project(SfmlGui)

set(CMAKE_CXX_STANDARD 14)
set(INCLUDE_DIRECTORY "include")
set(SOURCES_DIRECTORY "src")


set(HEADERS
        ${INCLUDE_DIRECTORY}/Fonts.h ${INCLUDE_DIRECTORY}/Button.h ${INCLUDE_DIRECTORY}/ButtonStates.h
        ${INCLUDE_DIRECTORY}/TextBox.h ${INCLUDE_DIRECTORY}/Resources.h ${INCLUDE_DIRECTORY}/Observer.h ${INCLUDE_DIRECTORY}/Types.h
        ${INCLUDE_DIRECTORY}/SfmlGui.h)

set(SOURCES
        ${SOURCES_DIRECTORY}/Fonts.cpp ${SOURCES_DIRECTORY}/Button.cpp ${SOURCES_DIRECTORY}/ButtonStates.cpp
        ${SOURCES_DIRECTORY}/TextBox.cpp ${SOURCES_DIRECTORY}/Resources.cpp ${SOURCES_DIRECTORY}/Observer.cpp)

if (WIN32)
    set(SFML_DIRECTORY_PATH ../SFML_Win)
	set(SFML_BIN_DIRECTORY ${SFML_DIRECTORY_PATH}/bin)
	
	#add sfml libraries
	add_library(Graphics STATIC IMPORTED)
	set_property(TARGET Graphics
 			 PROPERTY IMPORTED_LOCATION sfml-graphics-d.lib)

	add_library(System STATIC IMPORTED)
	set_property(TARGET System
 			 PROPERTY IMPORTED_LOCATION sfml-system-d.lib)

	add_library(Window STATIC IMPORTED)
	set_property(TARGET Window
 			 PROPERTY IMPORTED_LOCATION sfml-window-d.lib)
	#add sfml_libraries
	
	
	add_library(SfmlGui ${HEADERS} ${SOURCES})
	target_include_directories(SfmlGui PUBLIC ${SFML_DIRECTORY_PATH}/include/)
	target_include_directories(SfmlGui PRIVATE ${INCLUDE_DIRECTORY})
	target_link_directories(SfmlGui PUBLIC ${SFML_DIRECTORY_PATH}/lib/)
	target_link_libraries(SfmlGui Graphics System Window)
endif (WIN32)

if (APPLE)
	find_package(SFML 2.5.1 COMPONENTS system window graphics)
	add_library(SfmlGui ${HEADERS} ${SOURCES})
	target_include_directories(SfmlGui PRIVATE ${INCLUDE_DIRECTORY})
	target_link_libraries (SfmlGui sfml-graphics sfml-window sfml-system)
endif(APPLE)

#add_library(SfmlGui ${HEADERS} ${SOURCES})
#target_include_directories(SfmlGui PRIVATE ${INCLUDE_DIRECTORY})
#target_link_libraries (SfmlGui sfml-graphics sfml-window sfml-system)