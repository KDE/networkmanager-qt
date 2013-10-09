# Steps to generate the dbus interface files
# 1) Place the NetworkManager introspection dbus xml in introspection/
# 2) Update the list of xml files below that we want to generate interfaces for
# 3) Enable dbus subdirectory in ../CMakeLists.txt
#    Make, in order to generate the interface sources
# 3.1) Add annotations to the xml so qdbusxml2cpp knows what type or typedef to use
#      Add typedefs as needed to generic-types.h
# 4)   Add the list of generated files needed for the plugin to FILES_TO_BUILD
# 6) Copy the working generated interface sources into the source dir
# 5) Edit the sources as required to get them to build
# 5.1) Fix the inclusion guards as the compiler ignores anything after a '-' so the compiler thinks
#      headers have already been included
# Run do-hand-edits to update the diff of the interfaces vs what is generated; this speeds up regenerating them since you just have to apply the diff to make them compile

# Just the hardware related files to begin with, as these are already present in Solid
#find_package(Qt4 REQUIRED)

set(INTERFACE_INTROSPECTION_XML_FILES
   introspection/nm-access-point.xml
   introspection/nm-active-connection.xml
   introspection/nm-agent-manager.xml
   introspection/nm-device-adsl.xml
   introspection/nm-device-bond.xml
   introspection/nm-device-bridge.xml
   introspection/nm-device-bt.xml
   introspection/nm-device-ethernet.xml
   introspection/nm-device-infiniband.xml
   introspection/nm-device-modem.xml
   introspection/nm-device-olpc-mesh.xml
   introspection/nm-device-vlan.xml
   introspection/nm-device-wifi.xml
   introspection/nm-device-wimax.xml
   introspection/nm-device.xml
   introspection/nm-dhcp4-config.xml
   introspection/nm-dhcp6-config.xml
   introspection/nm-ip4-config.xml
   introspection/nm-ip6-config.xml
   introspection/nm-manager.xml
   introspection/nm-ppp-manager.xml
   introspection/nm-secret-agent.xml
   introspection/nm-settings-connection.xml
   introspection/nm-settings.xml
   introspection/nm-vpn-connection.xml
   introspection/nm-vpn-plugin.xml
   introspection/nm-wimax-nsp.xml
   )

set_property( SOURCE ${INTERFACE_INTROSPECTION_XML_FILES} PROPERTY INCLUDE generic-types.h)

set( ADAPTER_INTROSPECTION_XML_FILES
   )

set_source_files_properties( ${INTERFACE_INTROSPECTION_XML_FILES} ${ADAPTER_INTROSPECTION_XML_FILES} PROPERTIES NO_NAMESPACE TRUE )

set( FILES_TO_BUILD
   )

QT4_ADD_DBUS_INTERFACES( FILES_TO_BUILD ${INTERFACE_INTROSPECTION_XML_FILES} )
qt4_add_dbus_adaptor( FILES_TO_BUILD introspection/nm-secret-agent.xml "secretagent.h" "NetworkManager::SecretAgent")
foreach(adaptor ${ADAPTER_INTROSPECTION_XML_FILES})
    qt4_add_dbus_adaptor( FILES_TO_BUILD ${adaptor} "generic-types.h")
endforeach(adaptor ${ADAPTER_INTROSPECTION_XML_FILES})

#message(status "before patch in ${CMAKE_CURRENT_BUILD_DIR}")
#EXECUTE_PROCESS(COMMAND patch -p1 INPUT_FILE hand_edits.diff WORKING_DIRECTORY .})
#message(status "after patch")

add_library(networkmanagerdbus ${FILES_TO_BUILD})

TARGET_LINK_LIBRARIES(networkmanagerdbus ${QT_QTCORE_LIBRARY} ${QT_QTDBUS_LIBRARY})
