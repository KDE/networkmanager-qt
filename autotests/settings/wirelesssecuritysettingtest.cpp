/*
    Copyright 2012-2013  Jan Grulich <jgrulich@redhat.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "wirelesssecuritysettingtest.h"

#include <nm-version.h>
#if NM_CHECK_VERSION(1, 0, 0)
#include <libnm/NetworkManager.h>
#else
#include <nm-setting-wireless-security.h>
#endif

#include "settings/wirelesssecuritysetting.h"

#include <QTest>

void WirelessSecuritySettingTest::testSetting_data()
{
    QTest::addColumn<QString>("keyMgmt");
    QTest::addColumn<quint32>("wepTxKeyidx");
    QTest::addColumn<QString>("authAlg");
    QTest::addColumn<QStringList>("proto");
    QTest::addColumn<QStringList>("pairwise");
    QTest::addColumn<QStringList>("group");
    QTest::addColumn<QString>("leapUsername");
    QTest::addColumn<QString>("wepKey0");
    QTest::addColumn<QString>("wepKey1");
    QTest::addColumn<QString>("wepKey2");
    QTest::addColumn<QString>("wepKey3");
    QTest::addColumn<quint32>("wepKeyFlags");
    QTest::addColumn<quint32>("wepKeyType");
    QTest::addColumn<QString>("psk");
    QTest::addColumn<quint32>("pskFlags");
    QTest::addColumn<QString>("leapPassword");
    QTest::addColumn<quint32>("leapPasswordFlags");

    QStringList proto;
    proto << "wpa";
    proto << "rsn";

    QStringList pairwise;
    pairwise << "wep40";
    pairwise << "ccmp";

    QStringList group;
    group << "wep104";
    group << "tkip";

    QTest::newRow("setting1")
            << QString("none")          // keyMgmt
            << (quint32) 1              // wepTxKeyIdx
            << QString("open")          // authAlg
            << proto                    // proto
            << pairwise                 // pairwise
            << group                    // group
            << QString("login")         // leapUsername
            << QString("abcde")         // wepKey0
            << QString("fghij")         // wepKey1
            << QString("klmno")         // wepKey2
            << QString("pqrst")         // wepKey3
            << (quint32) 1              // wepKeyFlags
            << (quint32) 1              // wepKeyType
            << QString("12345678")      // psk
            << (quint32) 2              // pskFlags
            << QString("leappass")      // leapPassword
            << (quint32) 4;             // leapPasswordFlags
}

void WirelessSecuritySettingTest::testSetting()
{
    QFETCH(QString, keyMgmt);
    QFETCH(quint32, wepTxKeyidx);
    QFETCH(QString, authAlg);
    QFETCH(QStringList, proto);
    QFETCH(QStringList, pairwise);
    QFETCH(QStringList, group);
    QFETCH(QString, leapUsername);
    QFETCH(QString, wepKey0);
    QFETCH(QString, wepKey1);
    QFETCH(QString, wepKey2);
    QFETCH(QString, wepKey3);
    QFETCH(quint32, wepKeyFlags);
    QFETCH(quint32, wepKeyType);
    QFETCH(QString, psk);
    QFETCH(quint32, pskFlags);
    QFETCH(QString, leapPassword);
    QFETCH(quint32, leapPasswordFlags);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_KEY_MGMT), keyMgmt);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_WEP_TX_KEYIDX), wepTxKeyidx);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_AUTH_ALG), authAlg);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_PROTO), proto);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_PAIRWISE), pairwise);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_GROUP), group);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_LEAP_USERNAME), leapUsername);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_WEP_KEY0), wepKey0);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_WEP_KEY1), wepKey1);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_WEP_KEY2), wepKey2);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_WEP_KEY3), wepKey3);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_WEP_KEY_FLAGS), wepKeyFlags);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_WEP_KEY_TYPE), wepKeyType);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_PSK), psk);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_PSK_FLAGS), pskFlags);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_LEAP_PASSWORD), leapPassword);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SECURITY_LEAP_PASSWORD_FLAGS), leapPasswordFlags);

    NetworkManager::WirelessSecuritySetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    // Will fail if set some default values, because they are skipped in toMap() method
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        QCOMPARE(it.value(), map1.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(WirelessSecuritySettingTest)
