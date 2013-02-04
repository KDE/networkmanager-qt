set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

FIND_PACKAGE (KDE4 REQUIRED)

include(KDE4Defaults)

MACRO(LIBNM_TESTS)
       FOREACH(_testname ${ARGN})
               kde4_add_unit_test(${_testname} TESTNAME libnm-${_testname} ${_testname}.cpp)
               target_link_libraries(${_testname}
                                     QtNetworkManager
                                     QtCore
                                     QtNetwork
                                     ${QT_QTTEST_LIBRARY})
       ENDFOREACH(_testname)
ENDMACRO(LIBNM_TESTS)

LIBNM_TESTS(
    8021xsetting
    adslsetting
    bluetoothsetting
    bondsetting
    cdmasetting
    connectionsetting
    gsmsetting
    infinibandsetting
    ipv4setting
    ipv6setting
    olpcmeshsetting
    pppsetting
    pppoesetting
    serialsetting
    vlansetting
    wiredsetting
    wirelesssetting
    wirelesssecuritysetting
)
