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
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/dbus
   ${CMAKE_CURRENT_SOURCE_DIR}/settings
   ${CMAKE_CURRENT_BUILD_DIR}
)

#add_subdirectory(dbus)

option(BUILD_EXAMPLES "Build example applications" FALSE)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)

enable_testing(true)

add_subdirectory(tests)

set(QtNetworkManager_HEADERS
    accesspoint.h
    activeconnection.h
    adsldevice.h
    bluetoothdevice.h
    bonddevice.h
    bridgedevice.h
    connection.h
    device.h
    dhcp4config.h
    dhcp6config.h
    infinibanddevice.h
    ipaddress.h
    iproute.h
    ipconfig.h
    manager.h
    modemdevice.h
    olpcmeshdevice.h
    secretagent.h
    settings.h
    vlandevice.h
    vpnconnection.h
    vpnplugin.h
    wimaxdevice.h
    wimaxnsp.h
    wireddevice.h
    wirelessdevice.h
    wirelessnetwork.h
    wirelessnetworkinterfaceenvironment.h
    generic-types.h
    QtNetworkManager-export.h
)

set(QtNetworkManager_PRIVATE_HEADERS
    manager_p.h
    settings_p.h
    settings/802-3-ethernet_p.h
    settings/802-11-olpc-mesh_p.h
    settings/802-11-wireless_p.h
    settings/802-11-wireless-security_p.h
    settings/802-1x_p.h
    settings/adsl_p.h
    settings/bluetooth_p.h
    settings/bond_p.h
    settings/bridge_p.h
    settings/bridgeport_p.h
    settings/cdma_p.h
    settings/connection_p.h
    settings/gsm_p.h
    settings/ipv4_p.h
    settings/ipv6_p.h
    settings/infiniband_p.h
    settings/ppp_p.h
    settings/pppoe_p.h
    settings/serial_p.h
    #settings/template_p.h
    settings/vlan_p.h
    settings/vpn_p.h
    settings/wimax_p.h
)

set(QtNetworkManager_PART_SRCS
    accesspoint.cpp
    activeconnection.cpp
    adsldevice.cpp
    bluetoothdevice.cpp
    bonddevice.cpp
    bridgedevice.cpp
    connection.cpp
    device.cpp
    dhcp4config.cpp
    dhcp6config.cpp
    infinibanddevice.cpp
    ipaddress.cpp
    iproute.cpp
    ipconfig.cpp
    #nmpropertyhelper.cpp
    manager.cpp
    modemdevice.cpp
    olpcmeshdevice.cpp
    secretagent.cpp
    settings.cpp
    vlandevice.cpp
    vpnconnection.cpp
    vpnplugin.cpp
    wimaxdevice.cpp
    wimaxnsp.cpp
    wireddevice.cpp
    wirelessdevice.cpp
    wirelessnetwork.cpp
    wirelessnetworkinterfaceenvironment.cpp
    generic-types.cpp
)

set(DBUS_INTERFACE_SRCS
   dbus/nm-access-pointinterface.cpp
   dbus/nm-active-connectioninterface.cpp
   dbus/nm-agent-managerinterface.cpp
   dbus/nm-device-adslinterface.cpp
   dbus/nm-device-bondinterface.cpp
   dbus/nm-device-bridgeinterface.cpp
   dbus/nm-device-btinterface.cpp
   dbus/nm-device-ethernetinterface.cpp
   dbus/nm-device-infinibandinterface.cpp
   dbus/nm-device-modeminterface.cpp
   dbus/nm-device-olpc-meshinterface.cpp
   dbus/nm-device-vlaninterface.cpp
   dbus/nm-device-wifiinterface.cpp
   dbus/nm-device-wimaxinterface.cpp
   dbus/nm-deviceinterface.cpp
   dbus/nm-dhcp4-configinterface.cpp
   dbus/nm-dhcp6-configinterface.cpp
   dbus/nm-ip4-configinterface.cpp
   dbus/nm-ip6-configinterface.cpp
   dbus/nm-managerinterface.cpp
   dbus/nm-ppp-managerinterface.cpp
   #dbus/nm-secret-agentinterface.cpp
   dbus/nm-secret-agentadaptor.cpp
   dbus/nm-settings-connectioninterface.cpp
   dbus/nm-settingsinterface.cpp
   dbus/nm-vpn-connectioninterface.cpp
   dbus/nm-vpn-plugininterface.cpp
   dbus/nm-wimax-nspinterface.cpp
   )

set(CONNECTIONS_SETTINGS_SRCS
    settings/802-3-ethernet.cpp
    settings/802-11-olpc-mesh.cpp
    settings/802-11-wireless.cpp
    settings/802-11-wireless-security.cpp
    settings/802-1x.cpp
    settings/adsl.cpp
    settings/bluetooth.cpp
    settings/bond.cpp
    settings/bridge.cpp
    settings/bridgeport.cpp
    settings/cdma.cpp
    settings/connection.cpp
    settings/gsm.cpp
    settings/ipv4.cpp
    settings/ipv6.cpp
    settings/infiniband.cpp
    settings/ppp.cpp
    settings/pppoe.cpp
    settings/setting.cpp
    settings/serial.cpp
    #settings/template.cpp
    settings/vlan.cpp
    settings/vpn.cpp
    settings/wimax.cpp
    )

set(CONNECTIONS_SETTINGS_HEADERS
    settings/802-3-ethernet.h
    settings/802-11-olpc-mesh.h
    settings/802-11-wireless.h
    settings/802-11-wireless-security.h
    settings/802-1x.h
    settings/adsl.h
    settings/bluetooth.h
    settings/bond.h
    settings/bridge.h
    settings/bridgeport.h
    settings/cdma.h
    settings/connection.h
    settings/gsm.h
    settings/ipv4.h
    settings/ipv6.h
    settings/infiniband.h
    settings/ppp.h
    settings/pppoe.h
    settings/setting.h
    settings/serial.h
    #settings/template.h
    settings/vlan.h
    settings/vpn.h
    settings/wimax.h
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
   DESTINATION
   include/QtNetworkManager COMPONENT Devel)

install( FILES
   ${CONNECTIONS_SETTINGS_HEADERS}
   DESTINATION
   include/QtNetworkManager/settings COMPONENT Devel)

set_property(TARGET QtNetworkManager PROPERTY COMPILE_DEFINITIONS MAKE_NMQT_LIB)
set_property(TARGET QtNetworkManager PROPERTY VERSION ${VERSION})
set_property(TARGET QtNetworkManager PROPERTY SOVERSION 0)

message(STATUS "Writing pkg-config file...")
configure_file(${CMAKE_SOURCE_DIR}/QtNetworkManager.pc.cmake ${CMAKE_BINARY_DIR}/QtNetworkManager.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/QtNetworkManager.pc DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/pkgconfig/")
