project(exampleNetworkManagerQt)

cmake_minimum_required(VERSION 2.8.12)

find_package(KF5NetworkManagerQt ${KF5_VERSION} REQUIRED)

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
