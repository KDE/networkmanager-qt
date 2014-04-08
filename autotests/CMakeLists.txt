include_directories (${CMAKE_SOURCE_DIR}/src/settings
                     ${NETWORKMANAGER_INCLUDE_DIRS}
                     ${NM-GLIB_INCLUDE_DIRS}
)

include(ECMMarkAsTest)

find_package(Qt5 ${REQUIRED_QT_VERSION} NO_MODULE REQUIRED Test)
set_package_properties(Qt5Test PROPERTIES PURPOSE "Required for autotests")

macro(NETWORKMANAGERQT_AUTOTESTS)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp)
        target_link_libraries(${_testname} Qt5::Test KF5::NetworkManagerQt)
        add_test(networkmanagerqt-${_testname} ${CMAKE_CURRENT_BINARY_DIR}/${_testname})
        ecm_mark_as_test(${_testname})
    endforeach(_testname)
endmacro(NETWORKMANAGERQT_AUTOTESTS)

NETWORKMANAGERQT_AUTOTESTS(
    8021xsettingtest
    adslsettingtest
    bluetoothsettingtest
    bondsettingtest
    bridgesettingtest
    bridgeportsettingtest
    cdmasettingtest
    connectionsettingtest
    gsmsettingtest
    infinibandsettingtest
    ipv4settingtest
    ipv6settingtest
    olpcmeshsettingtest
    pppsettingtest
    pppoesettingtest
    serialsettingtest
    vlansettingtest
    wimaxsettingtest
    wiredsettingtest
    wirelesssettingtest
    wirelesssecuritysettingtest
)
