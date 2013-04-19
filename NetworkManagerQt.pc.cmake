prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
libdir=@CMAKE_INSTALL_PREFIX@/lib@LIB_SUFFIX@
includedir=${prefix}/include

Name: NetworkManagerQt
Description: Convenience Qt library for clients of NetworkManager
Version: @VERSION@

Requires: ModemManagerQt
Cflags: -I${includedir} @CMAKE_INCLUDE_PATH@
Libs: -L${libdir} -lNetworkManagerQt @CMAKE_LIBRARY_PATH@
