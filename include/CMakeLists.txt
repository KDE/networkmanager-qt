set(NetworkManagerQt_CAMEL_HEADERS
    WimaxDevice
    WirelessDevice
)

install(FILES
   ${NetworkManagerQt_CAMEL_HEADERS}
   DESTINATION
   include/NetworkManagerQt COMPONENT Devel)