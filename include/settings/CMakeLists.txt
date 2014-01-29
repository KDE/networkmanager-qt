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
    VlanSetting
    VpnSetting
    WimaxSetting
    WirelessSetting
    WirelessSecuritySetting
)

install(FILES
   ${NetworkManagerQt_settings_CAMEL_HEADERS}
   DESTINATION include/NetworkManagerQt5/NetworkManagerQt
   COMPONENT Devel)
