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
    iptunnelsettingtest
    ipv4settingtest
    ipv6settingtest
    olpcmeshsettingtest
    pppsettingtest
    pppoesettingtest
    proxysettingtest
    serialsettingtest
    tunsettingtest
    usersettingtest
    vlansettingtest
    vxlansettingtest
    vpnsettingtest
    wimaxsettingtest
    wiredsettingtest
    wirelesssettingtest
    wirelesssecuritysettingtest
)

