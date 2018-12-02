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

#include "ovsportsettingtest.h"

#include "settings/ovsportsetting.h"

#include <libnm/NetworkManager.h>

#include <QTest>

#if !NM_CHECK_VERSION(1, 10, 0)
#define NM_SETTING_OVS_PORT_VLAN_MODE           "vlan-mode"
#define NM_SETTING_OVS_PORT_TAG                 "tag"
#define NM_SETTING_OVS_PORT_LACP                "lacp"
#define NM_SETTING_OVS_PORT_BOND_MODE           "bond-mode"
#define NM_SETTING_OVS_PORT_BOND_UPDELAY        "bond-updelay"
#define NM_SETTING_OVS_PORT_BOND_DOWNDELAY      "bond-downdelay"
#endif

void OvsPortSettingTest::testSetting_data()
{
    QTest::addColumn<quint32>("bondDowndelay");
    QTest::addColumn<quint32>("bondUpdelay");
    QTest::addColumn<quint32>("tag");
    QTest::addColumn<QString>("lacp");
    QTest::addColumn<QString>("bondMode");
    QTest::addColumn<QString>("vlanMode");

    QTest::newRow("setting1")
            << (quint32)1                // bondDowndelay
            << (quint32)1                // bondUpdelay
            << (quint32)1                // tag
            << QString("off")            // lacp
            << QString("active-backup")  // bondMode
            << QString("trunk");           // vlanMode
}

void OvsPortSettingTest::testSetting()
{
    QFETCH(quint32, bondDowndelay);
    QFETCH(quint32, bondUpdelay);
    QFETCH(quint32, tag);
    QFETCH(QString, lacp);
    QFETCH(QString, bondMode);
    QFETCH(QString, vlanMode);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_OVS_PORT_BOND_DOWNDELAY), bondDowndelay);
    map.insert(QLatin1String(NM_SETTING_OVS_PORT_BOND_UPDELAY), bondUpdelay);
    map.insert(QLatin1String(NM_SETTING_OVS_PORT_TAG), tag);
    map.insert(QLatin1String(NM_SETTING_OVS_PORT_LACP), lacp);
    map.insert(QLatin1String(NM_SETTING_OVS_PORT_BOND_MODE), bondMode);
    map.insert(QLatin1String(NM_SETTING_OVS_PORT_VLAN_MODE), vlanMode);

    NetworkManager::OvsPortSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    // Will fail if set some default values, because they are skipped in toMap() method
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        QCOMPARE(it.value(), map1.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(OvsPortSettingTest)
