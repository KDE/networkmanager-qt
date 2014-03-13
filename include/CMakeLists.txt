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

if (${NETWORKMANAGER_VERSION} VERSION_EQUAL 0.9.9.0 OR ${NETWORKMANAGER_VERSION} VERSION_GREATER 0.9.9.0)
    set(NetworkManagerQt_CAMEL_HEADERS
        ${NetworkManagerQt_CAMEL_HEADERS}
        GenericTypes
        GreDevice
        MacVlanDevice
        TeamDevice
        TunDevice
        VethDevice
        )
endif()

install(FILES
   ${NetworkManagerQt_CAMEL_HEADERS}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NetworkManagerQt5/NetworkManagerQt
   COMPONENT Devel)

add_subdirectory(settings)
