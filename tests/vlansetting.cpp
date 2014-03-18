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

#include "vlansetting.h"

#include "../settings/vlansetting.h"

#include <nm-setting-vlan.h>

#include <QTest>

void VlanSetting::testSetting_data()
{
    QTest::addColumn<QString>("interfaceName");
    QTest::addColumn<QString>("parent");
    QTest::addColumn<quint32>("id");
    QTest::addColumn<quint32>("flags");
    QTest::addColumn<QStringList>("ingressPriorityMap");
    QTest::addColumn<QStringList>("egressPriorityMap");

    QStringList mapping;
    mapping << "7:3";
    mapping << "8:4";

    QTest::newRow("setting1")
            << QString("eth2.1")                                // interfaceName
            << QString("2815492f-7e56-435e-b2e9-246bd7cdc668")  // parent
            << (quint32) 2                                      // id
            << (quint32) 0x02                                   // flags
            << mapping                                          // ingressPriorityMap
            << mapping;                                         // egressPriorityMap
}

void VlanSetting::testSetting()
{
    QFETCH(QString, interfaceName);
    QFETCH(QString, parent);
    QFETCH(quint32, id);
    QFETCH(quint32, flags);
    QFETCH(QStringList, ingressPriorityMap);
    QFETCH(QStringList, egressPriorityMap);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_VLAN_INTERFACE_NAME), interfaceName);
    map.insert(QLatin1String(NM_SETTING_VLAN_PARENT), parent);
    map.insert(QLatin1String(NM_SETTING_VLAN_ID), id);
    map.insert(QLatin1String(NM_SETTING_VLAN_FLAGS), flags);
    map.insert(QLatin1String(NM_SETTING_VLAN_INGRESS_PRIORITY_MAP), ingressPriorityMap);
    map.insert(QLatin1String(NM_SETTING_VLAN_EGRESS_PRIORITY_MAP), egressPriorityMap);

    NetworkManager::VlanSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        QCOMPARE(it.value(), map1.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(VlanSetting)
#include "moc_vlansetting.cpp"
