cmake_minimum_required(VERSION 2.6)

project(server)

set(SRCS
  Cmd.cpp
  Logger.cpp
  Parser.cpp
  Plugins.cpp
  Server.cpp
  SocketIO.cpp
  main.cpp
)
set(CMAKE_CXX_STANDARD 11)

add_executable(server ${SRCS})

find_package(Boost REQUIRED)
target_include_directories(server PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARY_DIRS})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})