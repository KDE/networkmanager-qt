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
)

install(FILES
   ${NetworkManagerQt_CAMEL_HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NetworkManagerQt5/NetworkManagerQt
   COMPONENT Devel)

add_subdirectory(settings)
