/*
    Copyright 2018 Pranav Gade <pranavgade20@gmail.com>

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

#include "ovsbridgesettingtest.h"

#include "settings/ovsbridgesetting.h"

#include <libnm/NetworkManager.h>

#include <QTest>

#if !NM_CHECK_VERSION(1, 10, 0)
#define NM_SETTING_OVS_BRIDGE_SETTING_NAME          "ovs-bridge"
#define NM_SETTING_OVS_BRIDGE_FAIL_MODE             "fail-mode"
#define NM_SETTING_OVS_BRIDGE_MCAST_SNOOPING_ENABLE "mcast-snooping-enable"
#define NM_SETTING_OVS_BRIDGE_RSTP_ENABLE           "rstp-enable"
#define NM_SETTING_OVS_BRIDGE_STP_ENABLE            "stp-enable"
#endif


void OvsBridgeSettingTest::testSetting_data()
{
    QTest::addColumn<bool>("mcastSnoopingEnable");
    QTest::addColumn<bool>("rstpEnable");
    QTest::addColumn<bool>("stpEnable");
    QTest::addColumn<QString>("failMode");

    QTest::newRow("setting1")
            << true                     // mcastSnoopingEnable
            << true                     // rstpEnable
            << true                     // stpEnable
            << QString("secure");       // failMode
}

void OvsBridgeSettingTest::testSetting()
{
    QFETCH(bool, mcastSnoopingEnable);
    QFETCH(bool, rstpEnable);
    QFETCH(bool, stpEnable);
    QFETCH(QString, failMode);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_OVS_BRIDGE_MCAST_SNOOPING_ENABLE), mcastSnoopingEnable);
    map.insert(QLatin1String(NM_SETTING_OVS_BRIDGE_RSTP_ENABLE), rstpEnable);
    map.insert(QLatin1String(NM_SETTING_OVS_BRIDGE_STP_ENABLE), stpEnable);
    map.insert(QLatin1String(NM_SETTING_OVS_BRIDGE_FAIL_MODE), failMode);

    NetworkManager::OvsBridgeSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    // Will fail if set some default values, because they are skipped in toMap() method
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        QCOMPARE(it.value(), map1.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(OvsBridgeSettingTest)
