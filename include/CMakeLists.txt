set(NetworkManagerQt_CAMEL_HEADERS
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
    GenericTypes
    InfinibandDevice
    IpAddress
    IpConfig
    IpRoute
    Manager
    ModemDevice
    OlpcMeshDevice
    SecretAgent
    Settings
    Utils
    VlanDevice
    VpnConnection
    VpnPlugin
    WimaxDevice
    WimaxNsp
    WiredDevice
    WirelessDevice
    WirelessNetwork
)

install(FILES
   ${NetworkManagerQt_CAMEL_HEADERS}
   DESTINATION
   ${CMAKE_INSTALL_INCLUDEDIR}/NetworkManagerQt COMPONENT Devel)

add_subdirectory(settings)
