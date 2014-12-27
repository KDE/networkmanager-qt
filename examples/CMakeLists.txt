project(exampleNetworkManagerQt)

cmake_minimum_required(VERSION 2.8.12)

find_package(KF5NetworkManagerQt ${KF5_VERSION} REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
find_package(NetworkManager 0.9.8.4 REQUIRED)

find_package(Qt5 5.2.0 CONFIG REQUIRED COMPONENTS Core DBus Network)

include_directories(${NETWORKMANAGER_INCLUDE_DIRS})

set(exampleNetworkManagerQt_SRCS
   main.cpp
)

add_executable(exampleNetworkManagerQt ${exampleNetworkManagerQt_SRCS})
target_link_libraries(exampleNetworkManagerQt
    KF5::NetworkManagerQt
    Qt5::Core
    Qt5::Network)

add_subdirectory(createconnection)
