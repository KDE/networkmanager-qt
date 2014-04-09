project(createConnectionExample)

cmake_minimum_required(VERSION 2.8.12)

find_package(KF5NetworkManagerQt ${KF5_VERSION} REQUIRED)

include_directories(${NETWORKMANAGER_INCLUDE_DIRS})

set(createConnectionExample_SRCS
   main.cpp
)

add_executable(createConnectionExample ${createConnectionExample_SRCS})
target_link_libraries(createConnectionExample
    KF5::NetworkManagerQt
    Qt5::Core
    Qt5::Network)
