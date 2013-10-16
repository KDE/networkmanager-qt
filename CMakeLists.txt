cmake_minimum_required(VERSION 2.8)

project(NetworkManagerQt)

set(VERSION 0.9.0.1)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
set(LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (32/64)" )

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(Qt4 REQUIRED)
find_package(NetworkManager 0.9.8.0 REQUIRED)

if (DISABLE_MODEMMANAGERQT)
    message(STATUS "Disabling ModemManagerQt support")
    set(WITH_MODEMMANAGERQT 0)
else()
    pkg_check_modules(MODEMMANAGERQT ModemManagerQt)
    if (MODEMMANAGERQT_FOUND)
        message(STATUS "Enabling ModemManagerQt support")
        set(WITH_MODEMMANAGERQT 1)
    else()
        message(STATUS "ModemManagerQt not found")
        set(WITH_MODEMMANAGERQT 0)
    endif()
endif()

add_definitions(-DWITH_MODEMMANAGERQT=${WITH_MODEMMANAGERQT} ${QT_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${QT_INCLUDES}
                    ${MODEMMANAGERQT_INCLUDE_DIRS} ${CMAKE_INCLUDE_PATH})
link_directories(${MODEMMANAGERQT_LIBRARY_DIRS} ${CMAKE_LIBRARY_PATH})

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

option(DISABLE_TESTING "Disable unit tests" FALSE)
if (DISABLE_TESTING)
    message(STATUS "Disabling unit tests")
else()
    enable_testing(true)
    add_subdirectory(tests)
endif()

set(NetworkManagerQt_HEADERS
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
    utils.h
    vlandevice.h
    vpnconnection.h
    vpnplugin.h
    wimaxdevice.h
    wimaxnsp.h
    wireddevice.h
    wirelessdevice.h
    wirelessnetwork.h
    generic-types.h
    NetworkManagerQt-export.h
)

set(NetworkManagerQt_PRIVATE_HEADERS
    manager_p.h
    settings_p.h
    #settings/template_p.h
)

set(NetworkManagerQt_PART_SRCS
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
    manager.cpp
    modemdevice.cpp
    olpcmeshdevice.cpp
    secretagent.cpp
    settings.cpp
    utils.cpp
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
    settings/wiredsetting.cpp
    settings/olpcmeshsetting.cpp
    settings/wirelesssetting.cpp
    settings/wirelesssecuritysetting.cpp
    settings/security8021xsetting.cpp
    settings/adslsetting.cpp
    settings/bluetoothsetting.cpp
    settings/bondsetting.cpp
    settings/bridgesetting.cpp
    settings/bridgeportsetting.cpp
    settings/cdmasetting.cpp
    settings/connectionsettings.cpp
    settings/gsmsetting.cpp
    settings/ipv4setting.cpp
    settings/ipv6setting.cpp
    settings/infinibandsetting.cpp
    settings/pppsetting.cpp
    settings/pppoesetting.cpp
    settings/setting.cpp
    settings/serialsetting.cpp
    #settings/template.cpp
    settings/vlansetting.cpp
    settings/vpnsetting.cpp
    settings/wimaxsetting.cpp
    )

set(CONNECTIONS_SETTINGS_HEADERS
    settings/wiredsetting.h
    settings/olpcmeshsetting.h
    settings/wirelesssetting.h
    settings/wirelesssecuritysetting.h
    settings/security8021xsetting.h
    settings/adslsetting.h
    settings/bluetoothsetting.h
    settings/bondsetting.h
    settings/bridgesetting.h
    settings/bridgeportsetting.h
    settings/cdmasetting.h
    settings/connectionsettings.h
    settings/gsmsetting.h
    settings/ipv4setting.h
    settings/ipv6setting.h
    settings/infinibandsetting.h
    settings/pppsetting.h
    settings/pppoesetting.h
    settings/setting.h
    settings/serialsetting.h
    #settings/template.h
    settings/vlansetting.h
    settings/vpnsetting.h
    settings/wimaxsetting.h
    )

qt4_wrap_cpp(NetworkManagerQt_MOC ${NetworkManagerQt_PRIVATE_HEADERS})
qt4_automoc(${NetworkManagerQt_PART_SRCS})
qt4_automoc(${DBUS_INTERFACE_SRCS})

add_library(NetworkManagerQt SHARED ${NetworkManagerQt_PART_SRCS} ${DBUS_INTERFACE_SRCS} ${CONNECTIONS_SETTINGS_SRCS} ${NetworkManagerQt_MOC} )

if (MODEMMANAGERQT_FOUND)
    target_link_libraries(NetworkManagerQt ModemManagerQt ${QT_QTCORE_LIBRARY} ${QT_QTDBUS_LIBRARY} ${QT_QTNETWORK_LIBRARY})
else()
    target_link_libraries(NetworkManagerQt ${QT_QTCORE_LIBRARY} ${QT_QTDBUS_LIBRARY} ${QT_QTNETWORK_LIBRARY})
endif()
install(TARGETS NetworkManagerQt DESTINATION lib${LIB_SUFFIX})

install( FILES
   ${NetworkManagerQt_HEADERS}
   DESTINATION
   include/NetworkManagerQt COMPONENT Devel)

install( FILES
   ${CONNECTIONS_SETTINGS_HEADERS}
   DESTINATION
   include/NetworkManagerQt/settings COMPONENT Devel)

set_property(TARGET NetworkManagerQt PROPERTY COMPILE_DEFINITIONS MAKE_NETWORKMANAGERQT_LIB)
set_property(TARGET NetworkManagerQt PROPERTY VERSION ${VERSION})
set_property(TARGET NetworkManagerQt PROPERTY SOVERSION 0)

message(STATUS "Writing pkg-config file...")
configure_file(${CMAKE_SOURCE_DIR}/NetworkManagerQt.pc.cmake ${CMAKE_BINARY_DIR}/NetworkManagerQt.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/NetworkManagerQt.pc DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/pkgconfig/")

add_subdirectory(include)

##################  apidox ################################
find_package(Doxygen)

if(DOXYGEN_EXECUTABLE)
  configure_file(${CMAKE_SOURCE_DIR}/.Doxyfile.cmake ${CMAKE_BINARY_DIR}/Doxyfile)

  if(EXISTS ${QT_DOC_DIR}/html)
    set(QTDOCS "${QT_DOC_DIR}/html")
  else(EXISTS ${QT_DOC_DIR}/html)
    set(QTDOCS "http://qt-project.org/doc/latest/")
  endif(EXISTS ${QT_DOC_DIR}/html)

  add_custom_target(
    apidox
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile)
endif(DOXYGEN_EXECUTABLE)
