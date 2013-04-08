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
    generic-types.cpp
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

set(DBUS_INTERFACES_XML
   dbus/introspection/nm-access-point.xml
   dbus/introspection/nm-active-connection.xml
   dbus/introspection/nm-agent-manager.xml
   dbus/introspection/nm-device-adsl.xml
   dbus/introspection/nm-device-bond.xml
   dbus/introspection/nm-device-bridge.xml
   dbus/introspection/nm-device-bt.xml
   dbus/introspection/nm-device-ethernet.xml
   dbus/introspection/nm-device-infiniband.xml
   dbus/introspection/nm-device-modem.xml
   dbus/introspection/nm-device-olpc-mesh.xml
   dbus/introspection/nm-device-vlan.xml
   dbus/introspection/nm-device-wifi.xml
   dbus/introspection/nm-device-wimax.xml
   dbus/introspection/nm-device.xml
   dbus/introspection/nm-dhcp4-config.xml
   dbus/introspection/nm-dhcp6-config.xml
   dbus/introspection/nm-ip4-config.xml
   dbus/introspection/nm-ip6-config.xml
   dbus/introspection/nm-manager.xml
   dbus/introspection/nm-ppp-manager.xml
   dbus/introspection/nm-settings-connection.xml
   dbus/introspection/nm-settings.xml
   dbus/introspection/nm-vpn-connection.xml
   dbus/introspection/nm-vpn-plugin.xml
   dbus/introspection/nm-wimax-nsp.xml
)

set_source_files_properties(${DBUS_INTERFACES_XML} PROPERTIES INCLUDE generic-types.h)
set_source_files_properties(${DBUS_INTERFACES_XML} PROPERTIES NO_NAMESPACE TRUE)

FOREACH (_current_FILE ${DBUS_INTERFACES_XML})
    GET_FILENAME_COMPONENT(_infile ${_current_FILE} ABSOLUTE)
    # get the part before the ".xml" suffix
    STRING(REGEX REPLACE "(.*[/\\.])?([^\\.]+)\\.xml" "\\2" _basename ${_current_FILE})
    STRING(REPLACE "-" "_" _basename ${_basename})
    STRING(TOLOWER ${_basename} _basename)
    QT4_ADD_DBUS_INTERFACE(QtNetworkManager_PART_SRCS ${_infile} ${_basename}interface)
ENDFOREACH (_current_FILE ${DBUS_INTERFACES_XML})

qt4_add_dbus_adaptor(QtNetworkManager_PART_SRCS
    dbus/introspection/nm-secret-agent.xml
    secretagent.h
    NetworkManager::SecretAgent
    nm_secret_agentadaptor
)

qt4_wrap_cpp(QtNetworkManager_MOC ${QtNetworkManager_PRIVATE_HEADERS})
qt4_automoc(${QtNetworkManager_PART_SRCS})

add_library(QtNetworkManager SHARED ${QtNetworkManager_PART_SRCS} ${CONNECTIONS_SETTINGS_SRCS} ${QtNetworkManager_MOC} )

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
