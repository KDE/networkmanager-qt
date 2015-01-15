if (${NETWORKMANAGER_VERSION} VERSION_EQUAL 1.0.0 OR ${NETWORKMANAGER_VERSION} VERSION_GREATER 1.0.0)
include_directories(${NM-CORE_INCLUDE_DIRS})
else()
include_directories(${NETWORKMANAGER_INCLUDE_DIRS})
endif()

include_directories(
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
