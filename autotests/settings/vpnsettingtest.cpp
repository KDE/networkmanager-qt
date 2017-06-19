/*
    Copyright 2017 Jan Grulich <jgrulich@redhat.com>

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

#include "vpnsettingtest.h"

#include "settings/vpnsetting.h"

#include <nm-version.h>
#if NM_CHECK_VERSION(1, 0, 0)
#include <libnm/NetworkManager.h>
#else
#include <nm-setting-vpn.h>
#endif

#if !NM_CHECK_VERSION(1, 2, 0)
#define NM_SETTING_VPN_PERSISTENT "persistent"
#define NM_SETTING_VPN_TIMEOUT "timeout"
#endif

#include <QTest>

void VpnSettingTest::testSetting_data()
{
    QTest::addColumn<NMStringMap>("data");
    QTest::addColumn<bool>("persistent");
    QTest::addColumn<NMStringMap>("secrets");
    QTest::addColumn<QString>("serviceType");
    QTest::addColumn<uint>("timeout");
    QTest::addColumn<QString>("userName");

    QTest::newRow("setting1")
            << NMStringMap{{"gateway", "kde.org"}}   // data
            << true                                  // persistent
            << NMStringMap{{"password", "kde"}}      // secrets
            << QString("openvpn")                    // service-type
            << (uint) 30                             // timeout
            << QString("test-user");                // user-name
}

void VpnSettingTest::testSetting()
{
    QFETCH(NMStringMap, data);
    QFETCH(bool, persistent);
    QFETCH(NMStringMap, secrets);
    QFETCH(QString, serviceType);
    QFETCH(uint, timeout);
    QFETCH(QString, userName);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_VPN_DATA), QVariant::fromValue<NMStringMap>(data));
    map.insert(QLatin1String(NM_SETTING_VPN_PERSISTENT), persistent);
    map.insert(QLatin1String(NM_SETTING_VPN_SECRETS), QVariant::fromValue<NMStringMap>(secrets));
    map.insert(QLatin1String(NM_SETTING_VPN_SERVICE_TYPE), serviceType);
    map.insert(QLatin1String(NM_SETTING_VPN_TIMEOUT), timeout);
    map.insert(QLatin1String(NM_SETTING_VPN_USER_NAME), userName);

    NetworkManager::VpnSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    // Will fail if set some default values, because they are skipped in toMap() method
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        QCOMPARE(it.value(), map1.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(VpnSettingTest)
