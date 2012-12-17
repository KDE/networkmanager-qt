cmake_minimum_required(VERSION 2.8)

project(libQtNetworkManager)

set(VERSION 0.9.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

set(LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (32/64)" )

find_package(Qt4 REQUIRED)
find_package(NetworkManager REQUIRED)

pkg_check_modules(LIBMODEMMANAGERQT REQUIRED QtModemManager)
if (NOT LIBMODEMMANAGERQT_FOUND)
    message(FATAL_ERROR "ERROR: libQtModemManager not found")
endif (NOT LIBMODEMMANAGERQT_FOUND)

add_definitions(${QT_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${QT_INCLUDES}
                    ${LIBMODEMMANAGERQT_INCLUDE_DIRS} ${CMAKE_INCLUDE_PATH})
link_directories(${LIBMODEMMANAGERQT_LIBRARY_DIRS} ${CMAKE_LIBRARY_PATH})

include_directories(
   ${NETWORKMANAGER_INCLUDE_DIRS}
   ${NM-UTIL_INCLUDE_DIRS}
   ${NM-GLIB_INCLUDE_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}/dbus
   ${CMAKE_CURRENT_SOURCE_DIR}/settings
   ${CMAKE_CURRENT_BUILD_DIR}
)

#add_subdirectory(dbus)
#add_subdirectory(fakemanager)

#add_subdirectory(tests)

add_subdirectory(tests)

set(QtNetworkManager_HEADERS
manager.h
device.h
wireddevice.h
wirelessdevice.h
accesspoint.h
connection.h
modemdevice.h
bluetoothdevice.h
olpcmeshdevice.h
wimaxdevice.h
wimaxnsp.h
ipv4config.h
dhcp4config.h
ipv6config.h
dhcp6config.h
settings.h
activeconnection.h
vpnconnection.h
wirelessnetwork.h
wirelessnetworkinterfaceenvironment.h
secretagent.h
dbus/generic-types.h
QtNetworkManager-export.h
)

set(QtNetworkManager_PRIVATE_HEADERS
settings_p.h
manager_p.h
settings/connectionsettings_p.h
settings/802-3-ethernet_p.h
settings/802-11-wireless_p.h
settings/gsmsetting_p.h
settings/ipv4_p.h
)

set(QtNetworkManager_PART_SRCS
   accesspoint.cpp
   activeconnection.cpp
   vpnconnection.cpp
   connection.cpp
   ipv4config.cpp
   dhcp4config.cpp
   ipv6config.cpp
   dhcp6config.cpp
   manager.cpp
   modemdevice.cpp
   device.cpp
   settings.cpp
   wireddevice.cpp
   wirelessdevice.cpp
   wirelessnetwork.cpp
   wirelessnetworkinterfaceenvironment.cpp
   bluetoothdevice.cpp
   olpcmeshdevice.cpp
   wimaxdevice.cpp
   wimaxnsp.cpp
   secretagent.cpp
   dbus/generic-types.cpp
   #nmpropertyhelper.cpp
)

set(DBUS_INTERFACE_SRCS
   dbus/nm-agent-managerinterface.cpp
   dbus/nm-secret-agentadaptor.cpp
   dbus/nm-active-connectioninterface.cpp
   dbus/nm-access-pointinterface.cpp
   dbus/nm-wimax-nspinterface.cpp
   dbus/nm-device-btinterface.cpp
   dbus/nm-device-ethernetinterface.cpp
   dbus/nm-device-modeminterface.cpp
   dbus/nm-device-wifiinterface.cpp
   dbus/nm-device-wimaxinterface.cpp
   dbus/nm-device-olpc-meshinterface.cpp
   dbus/nm-deviceinterface.cpp
   dbus/nm-ip4-configinterface.cpp
   dbus/nm-ip6-configinterface.cpp
   dbus/nm-dhcp4-configinterface.cpp
   dbus/nm-dhcp6-configinterface.cpp
   dbus/nm-managerinterface.cpp
   dbus/nm-agent-managerinterface.cpp
   dbus/nm-secret-agentadaptor.cpp
   dbus/nm-settingsinterface.cpp
   dbus/nm-settings-connectioninterface.cpp
   dbus/nm-vpn-connectioninterface.cpp
   dbus/nm-vpn-plugininterface.cpp
   )

set(CONNECTIONS_SETTINGS_SRCS
    settings/setting.cpp
    settings/connectionsettings.cpp
    settings/802-3-ethernet.cpp
    settings/802-11-wireless.cpp
    settings/gsmsetting.cpp
    settings/ipv4.cpp
    )

set(CONNECTIONS_SETTINGS_HEADERS
    settings/setting.h
    settings/connectionsettings.h
    settings/802-3-ethernet.h
    settings/802-11-wireless.h
    settings/gsmsetting.h
    settings/ipv4.h
    )

qt4_wrap_cpp(QtNetworkManager_MOC ${QtNetworkManager_PRIVATE_HEADERS})
qt4_automoc(${QtNetworkManager_PART_SRCS})
qt4_automoc(${DBUS_INTERFACE_SRCS})

add_library(QtNetworkManager SHARED ${QtNetworkManager_PART_SRCS} ${DBUS_INTERFACE_SRCS} ${CONNECTIONS_SETTINGS_SRCS} ${QtNetworkManager_MOC} )

# TODO: dlopen libQtModemManager to make it a runtime dependency instead of compile
# time dependency.
target_link_libraries(QtNetworkManager QtModemManager ${QT_QTCORE_LIBRARY} ${QT_QTDBUS_LIBRARY} ${QT_QTNETWORK_LIBRARY})
install(TARGETS QtNetworkManager DESTINATION lib${LIB_SUFFIX})

install( FILES
   ${QtNetworkManager_HEADERS}
   ${CONNECTIONS_SETTINGS_HEADERS}
   DESTINATION
   include/QtNetworkManager COMPONENT Devel)

set_property(TARGET QtNetworkManager PROPERTY COMPILE_DEFINITIONS MAKE_NMQT_LIB)
set_property(TARGET QtNetworkManager PROPERTY VERSION ${VERSION})
set_property(TARGET QtNetworkManager PROPERTY SOVERSION 0)

message(STATUS "Writing pkg-config file...")
configure_file(${CMAKE_SOURCE_DIR}/QtNetworkManager.pc.cmake ${CMAKE_BINARY_DIR}/QtNetworkManager.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/QtNetworkManager.pc DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/pkgconfig/")
