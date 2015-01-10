# add_subdirectory(dbus)

include_directories(
   ${NETWORKMANAGER_INCLUDE_DIRS}
   ${NM-UTIL_INCLUDE_DIRS}
   ${NM-GLIB_INCLUDE_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}/dbus
   ${CMAKE_CURRENT_SOURCE_DIR}/settings
)

set(NetworkManagerQt_PART_SRCS
    device.cpp
    accesspoint.cpp
    activeconnection.cpp
    adsldevice.cpp
    bluetoothdevice.cpp
    bonddevice.cpp
    bridgedevice.cpp
    connection.cpp
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
    generictypes.cpp
)

set(NetworkManagerQt_SETTINGS_SRCS
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

if (${NETWORKMANAGER_VERSION} VERSION_EQUAL 0.9.10.0 OR ${NETWORKMANAGER_VERSION} VERSION_GREATER 0.9.10.0)
    set(NetworkManagerQt_PART_SRCS
        ${NetworkManagerQt_PART_SRCS}
        genericdevice.cpp
        gredevice.cpp
        macvlandevice.cpp
        teamdevice.cpp
        tundevice.cpp
        vethdevice.cpp
        )

    set(NetworkManagerQt_SETTINGS_SRCS
        ${NetworkManagerQt_SETTINGS_SRCS}
        settings/teamsetting.cpp
        )

    set(DBUS_INTERFACE_SRCS
        ${DBUS_INTERFACE_SRCS}
        dbus/nm-device-genericinterface.cpp
        dbus/nm-device-greinterface.cpp
        dbus/nm-device-macvlaninterface.cpp
        dbus/nm-device-teaminterface.cpp
        dbus/nm-device-tuninterface.cpp
        dbus/nm-device-vethinterface.cpp
        )
endif()

add_library(KF5NetworkManagerQt SHARED ${NetworkManagerQt_PART_SRCS} ${NetworkManagerQt_SETTINGS_SRCS} ${DBUS_INTERFACE_SRCS})
generate_export_header(KF5NetworkManagerQt BASE_NAME NetworkManagerQt)
add_library(KF5::NetworkManagerQt ALIAS KF5NetworkManagerQt)

target_include_directories(KF5NetworkManagerQt INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/NetworkManagerQt>")

target_link_libraries(KF5NetworkManagerQt PUBLIC Qt5::Core Qt5::Network Qt5::DBus)

set_target_properties(KF5NetworkManagerQt PROPERTIES VERSION ${NETWORKMANAGERQT_VERSION_STRING}
                                                   SOVERSION ${NETWORKMANAGERQT_SOVERSION}
                                                   EXPORT_NAME NetworkManagerQt
)

########### static lib for tests  ###############
add_library(KF5NetworkManagerQt_static STATIC ${NetworkManagerQt_PART_SRCS} ${NetworkManagerQt_SETTINGS_SRCS} ${DBUS_INTERFACE_SRCS})
set_target_properties(KF5NetworkManagerQt_static PROPERTIES COMPILE_FLAGS -DNMQT_STATIC=1)

target_link_libraries(KF5NetworkManagerQt_static PUBLIC Qt5::Core Qt5::Network Qt5::DBus)
target_include_directories(KF5NetworkManagerQt_static PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..;${CMAKE_CURRENT_BINARY_DIR}/..>")

ecm_generate_headers(NetworkManagerQt_HEADERS
  HEADER_NAMES
  AccessPoint
  ActiveConnection
  AdslDevice
  BluetoothDevice
  BondDevice
  BridgeDevice
  Connection
  Device
  Dhcp4Config
  Dhcp6Config
  GenericDevice
  GenericTypes
  GreDevice
  InfinibandDevice
  IpAddress
  IpConfig
  IpRoute
  MacVlanDevice
  Manager
  ModemDevice
  OlpcMeshDevice
  SecretAgent
  Settings
  TeamDevice
  TunDevice
  Utils
  VethDevice
  VlanDevice
  VpnConnection
  VpnPlugin
  WimaxDevice
  WimaxNsp
  WiredDevice
  WirelessDevice
  WirelessNetwork

  REQUIRED_HEADERS NetworkManagerQt_HEADERS
)

ecm_generate_headers(NetworkManagerQt_SETTINGS_HEADERS
  HEADER_NAMES
  AdslSetting
  BluetoothSetting
  BondSetting
  BridgeSetting
  BridgePortSetting
  CdmaSetting
  ConnectionSettings
  GsmSetting
  InfinibandSetting
  Ipv4Setting
  Ipv6Setting
  OlpcMeshSetting
  PppoeSetting
  PppSetting
  Security8021xSetting
  SerialSetting
  Setting
  TeamSetting
  VlanSetting
  VpnSetting
  WimaxSetting
  WiredSetting
  WirelessSecuritySetting
  WirelessSetting

  RELATIVE settings
  REQUIRED_HEADERS NetworkManagerQt_SETTINGS_HEADERS
)

install(TARGETS KF5NetworkManagerQt EXPORT KF5NetworkManagerQtTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/networkmanagerqt_export.h
  ${NetworkManagerQt_HEADERS}
  ${NetworkManagerQt_SETTINGS_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/NetworkManagerQt COMPONENT Devel
)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME NetworkManagerQt LIB_NAME KF5NetworkManagerQt DEPS "core" FILENAME_VAR PRI_FILENAME)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
