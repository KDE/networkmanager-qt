set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

MACRO(LIBNM_TESTS)
       FOREACH(_testname ${ARGN})
            SET(test_${_testname}_SRCS ${_testname}.cpp)
            qt4_automoc(${test_${_testname}_SRCS})
            add_executable(test_${_testname} ${test_${_testname}_SRCS} ${test_${_testname}_HDRS} ${test_additional})
            target_link_libraries(test_${_testname} ${QT_QTCORE_LIBRARY}
                ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY} NetworkManagerQt)
            ADD_TEST(test_${_testname} test_${_testname})
       ENDFOREACH(_testname)
ENDMACRO(LIBNM_TESTS)

LIBNM_TESTS(
    8021xsetting
    adslsetting
    bluetoothsetting
    bondsetting
    bridgesetting
    bridgeportsetting
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
    wimaxsetting
    wiredsetting
    wirelesssetting
    wirelesssecuritysetting
)
