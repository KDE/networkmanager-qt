cmake_minimum_required(VERSION 2.8)

project(libnm-qt)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

if(NOT DEFINED LIB_SUFFIX)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(LIB_SUFFIX "64" CACHE STRING "Define suffix of library directory name (32/64)" )
  else()
    set(LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (32/64)" )
  endif()
endif()

find_package(Qt4 REQUIRED)
find_package(NetworkManager REQUIRED)

add_definitions(${QT_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${QT_INCLUDES} ${CMAKE_INCLUDE_PATH})

include_directories(
   ${NETWORKMANAGER_INCLUDE_DIRS}
   ${NM-UTIL_INCLUDE_DIRS}
   ${CMAKE_CURRENT_SOURCE_DIR}/dbus
   ${CMAKE_CURRENT_BUILD_DIR}
)

#add_subdirectory(dbus)
#add_subdirectory(fakemanager)

#add_subdirectory(tests)

set(nmqt_HEADERS
manager.h
device.h
wireddevice.h
wirelessdevice.h
accesspoint.h
connection.h
modemdevice.h
bluetoothdevice.h
olpcmeshdevice.h
wimaxdevice.h
wimaxnsp.h
ipv4config.h
ipv6config.h
settings.h
activeconnection.h
vpnconnection.h
wirelessnetwork.h
wirelessnetworkinterfaceenvironment.h
secretagent.h
dbus/generic-types.h
nmqt-export.h
)

set(nmqt_PRIVATE_HEADERS
activeconnection_p.h
bluetoothdevice_p.h
device_p.h
manager_p.h
modemdevice_p.h
olpcmeshdevice_p.h
settings_p.h
wimaxdevice_p.h
wireddevice_p.h
wirelessdevice_p.h
wirelessnetwork_p.h
secretagent_p.h
)

set(nmqt_PART_SRCS
   accesspoint.cpp
   activeconnection.cpp
   vpnconnection.cpp
   connection.cpp
   ipv4config.cpp
   ipv6config.cpp
   manager.cpp
   modemdevice.cpp
   device.cpp
   settings.cpp
   wireddevice.cpp
   wirelessdevice.cpp
   wirelessnetwork.cpp
   wirelessnetworkinterfaceenvironment.cpp
   bluetoothdevice.cpp
   olpcmeshdevice.cpp
   wimaxdevice.cpp
   wimaxnsp.cpp
   secretagent.cpp
   dbus/generic-types.cpp
   #nmpropertyhelper.cpp
)

set(DBUS_INTERFACE_SRCS
   dbus/nm-agent-managerinterface.cpp
   dbus/nm-secret-agentadaptor.cpp
   dbus/nm-active-connectioninterface.cpp
   dbus/nm-access-pointinterface.cpp
   dbus/nm-wimax-nspinterface.cpp
   dbus/nm-device-btinterface.cpp
   dbus/nm-device-ethernetinterface.cpp
   dbus/nm-device-modeminterface.cpp
   dbus/nm-device-wifiinterface.cpp
   dbus/nm-device-wimaxinterface.cpp
   dbus/nm-device-olpc-meshinterface.cpp
   dbus/nm-deviceinterface.cpp
   dbus/nm-ip4-configinterface.cpp
   dbus/nm-ip6-configinterface.cpp
   dbus/nm-dhcp4-configinterface.cpp
   dbus/nm-dhcp6-configinterface.cpp
   dbus/nm-managerinterface.cpp
   dbus/nm-agent-managerinterface.cpp
   dbus/nm-secret-agentadaptor.cpp
   dbus/nm-settingsinterface.cpp
   dbus/nm-settings-connectioninterface.cpp
   dbus/nm-vpn-connectioninterface.cpp
   )

qt4_wrap_cpp(nmqt_MOC ${nmqt_HEADERS} ${nmqt_PRIVATE_HEADERS})
qt4_automoc(${DBUS_INTERFACE_SRCS})

add_library(nm-qt SHARED ${nmqt_MOC} ${nmqt_PART_SRCS} ${DBUS_INTERFACE_SRCS})

find_file(LIBMM_FOUND libmm-qt/manager.h /usr/include ${CMAKE_INCLUDE_PATH})
if (NOT LIBMM_FOUND)
    message(FATAL_ERROR "ERROR: libmm-qt not found")
endif (NOT LIBMM_FOUND)

# TODO: dlopen libmm-qt to make it a runtime dependency instead of compile
# time dependency.
target_link_libraries(nm-qt mm-qt ${QT_QTDBUS_LIBRARY} ${QT_QTNETWORK_LIBRARY})
install(TARGETS nm-qt DESTINATION lib${LIB_SUFFIX})

install( FILES
   ${nmqt_HEADERS}
   DESTINATION
   include/libnm-qt COMPONENT Devel)

set_property(TARGET nm-qt PROPERTY COMPILE_DEFINITIONS MAKE_NMQT_LIB)
set_property(TARGET nm-qt PROPERTY VERSION 0.9.0)
set_property(TARGET nm-qt PROPERTY SOVERSION 0)
