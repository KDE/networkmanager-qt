macro(NETWORKMANAGERQT_AUTOTESTS)
    foreach(_testname ${ARGN})
        ecm_add_test(${_testname}.cpp LINK_LIBRARIES Qt5::Test KF5NetworkManagerQt_static)
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
    vpnsettingtest
    wimaxsettingtest
    wiredsettingtest
    wirelesssettingtest
    wirelesssecuritysettingtest
)

if (${NETWORKMANAGER_VERSION} VERSION_EQUAL 1.1.92 OR ${NETWORKMANAGER_VERSION} VERSION_GREATER 1.1.92)
    ecm_add_test(tunsettingtest.cpp LINK_LIBRARIES Qt5::Test KF5NetworkManagerQt_static)
endif()
