set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

FIND_PACKAGE (KDE4 REQUIRED)

include(KDE4Defaults)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${QTNETWORK_INCLUDE_DIRS}
                     ${QTNETWORKMANAGER_INCLUDE_DIRS})

# Connection setting tests

set(connectionsetting_SRCS connectionsetting.cpp)

kde4_add_unit_test(connectionsetting TESTNAME libnm-connectionsetting ${connectionsetting_SRCS})

target_link_libraries(connectionsetting QtNetworkManager QtCore QtNetwork ${QT_QTTEST_LIBRARY})

# Wireless setting tests

set(wirelesssetting_SRCS wirelesssetting.cpp )

kde4_add_unit_test(wirelesssetting TESTNAME libnm-wirelesssetting ${wirelesssetting_SRCS})

target_link_libraries(wirelesssetting QtNetworkManager QtCore QtNetwork ${QT_QTTEST_LIBRARY})

# Wireless security setting tests

set(wirelesssecuritysetting_SRCS wirelesssecuritysetting.cpp )

kde4_add_unit_test(wirelesssecuritysetting TESTNAME libnm-wirelesssecuritysetting ${wirelesssecuritysetting_SRCS})

target_link_libraries(wirelesssecuritysetting QtNetworkManager QtCore QtNetwork ${QT_QTTEST_LIBRARY})


# Wired setting tests

set(wiredsetting_SRCS wiredsetting.cpp )

kde4_add_unit_test(wiredsetting TESTNAME libnm-wiredsetting ${wiredsetting_SRCS})

target_link_libraries(wiredsetting QtNetworkManager QtCore QtNetwork ${QT_QTTEST_LIBRARY})

# GSM setting tests

set(gsmsetting_SRCS gsmsetting.cpp)

kde4_add_unit_test(gsmsetting TESTNAME libnm-gsmsetting ${gsmsetting_SRCS})

target_link_libraries(gsmsetting QtNetworkManager QtCore QtNetwork ${QT_QTTEST_LIBRARY})

# IPv4 setting tests

set(ipv4setting_SRCS ipv4setting.cpp)

kde4_add_unit_test(ipv4setting TESTNAME libnm-ipv4setting ${ipv4setting_SRCS})

target_link_libraries(ipv4setting QtNetworkManager QtCore QtNetwork ${QT_QTTEST_LIBRARY})

# IPv6 setting tests

set(ipv6setting_SRCS ipv6setting.cpp)

kde4_add_unit_test(ipv6setting TESTNAME libnm-ipv6setting ${ipv6setting_SRCS})

target_link_libraries(ipv6setting QtNetworkManager QtCore QtNetwork ${QT_QTTEST_LIBRARY})

# Bluetooth setting tests

set(bluetoothsetting_SRCS bluetoothsetting.cpp)

kde4_add_unit_test(bluetoothsetting TESTNAME libnm-bluetoothsetting ${bluetoothsetting_SRCS})

target_link_libraries(bluetoothsetting QtNetworkManager QtCore QtNetwork ${QT_QTTEST_LIBRARY})

# Bond setting tests

set(bondsetting_SRCS bondsetting.cpp)

kde4_add_unit_test(bondsetting TESTNAME libnm-bondsetting ${bondsetting_SRCS})

target_link_libraries(bondsetting QtNetworkManager QtCore QtNetwork ${QT_QTTEST_LIBRARY})

# Cdma setting tests

set(cdmasetting_SRCS cdmasetting.cpp)

kde4_add_unit_test(cdmasetting TESTNAME libnm-cdmasetting ${cdmasetting_SRCS})

target_link_libraries(cdmasetting QtNetworkManager QtCore QtNetwork ${QT_QTTEST_LIBRARY})

# Infiniband setting tests

set(infiniband_SRCS infinibandsetting.cpp)

kde4_add_unit_test(infiniband TESTNAME libnm-infiniband ${infiniband_SRCS})

target_link_libraries(infiniband QtNetworkManager QtCore QtNetwork ${QT_QTTEST_LIBRARY})
