cmake_minimum_required(VERSION 3.5)

project(Netboost VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

set(HEADRES ncodes.hpp netinit.hpp Address.hpp Packet.hpp Socket.hpp 
TcpSocket.hpp UdpSocket.hpp TcpEntity.hpp TcpServer.hpp TcpClient.hpp)

set(SOURCES Address.cpp Packet.cpp 
TcpSocket.cpp UdpSocket.cpp TcpEntity.cpp TcpServer.cpp TcpClient.cpp)

add_library(Netboost ${HEADERS} ${SOURCES})