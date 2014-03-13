set(NetworkManagerQt_settings_CAMEL_HEADERS
    Security8021xSetting
    AdslSetting
    BluetoothSetting
    BondSetting
    BridgeSetting
    BridgePortSetting
    CdmaSetting
    ConnectionSettings
    GsmSetting
    WiredSetting
    InfinibandSetting
    Ipv4Setting
    Ipv6Setting
    OlpcMeshSetting
    PppSetting
    PppoeSetting
    SerialSetting
    Setting
    TeamSetting
    VlanSetting
    VpnSetting
    WimaxSetting
    WirelessSetting
    WirelessSecuritySetting
)

if (${NETWORKMANAGER_VERSION} VERSION_EQUAL 0.9.9.0 OR ${NETWORKMANAGER_VERSION} VERSION_GREATER 0.9.9.0)
    set(NetworkManagerQt_settings_CAMEL_HEADERS
        ${NetworkManagerQt_settings_CAMEL_HEADERS}
        TeamSetting
        )
endif()

install(FILES
   ${NetworkManagerQt_settings_CAMEL_HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NetworkManagerQt5/NetworkManagerQt
   COMPONENT Devel)
