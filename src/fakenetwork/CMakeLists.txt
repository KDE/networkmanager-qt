project(fakeNetwork)

cmake_minimum_required(VERSION 2.8.12)

find_package(KF5NetworkManagerQt ${KF5_VERSION} REQUIRED)

include_directories(
   ${NETWORKMANAGER_INCLUDE_DIRS}
   ${CMAKE_SOURCE_DIR}/src/settings
)

set(fakeNetwork_SRCS
    accesspoint.cpp
    activeconnection.cpp
    connection.cpp
    device.cpp
    fakenetwork.cpp
    settings.cpp
    wireddevice.cpp
    wirelessdevice.cpp
)

add_library(fakeNetwork STATIC ${fakeNetwork_SRCS})

target_link_libraries(fakeNetwork
    KF5NetworkManagerQt_static
    Qt5::Core
    Qt5::Network
    Qt5::DBus)
