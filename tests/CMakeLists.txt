set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

FIND_PACKAGE (KDE4 REQUIRED)

include(KDE4Defaults)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${QTNETWORK_INCLUDE_DIRS}
                     ${QTNETWORKMANAGER_INCLUDE_DIRS})

# Wireless setting tests

set(wirelesssetting_SRCS wirelesssetting.cpp )

kde4_add_unit_test(wirelesssetting TESTNAME libnm-wirelesssetting ${wirelesssetting_SRCS})

target_link_libraries(wirelesssetting QtNetworkManager QtCore QtNetwork ${QT_QTTEST_LIBRARY})

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
