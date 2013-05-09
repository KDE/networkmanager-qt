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

#include "bridgeportsetting.h"

#include "../settings/bridgeportsetting.h"

#include <nm-setting-bridge-port.h>

void BridgePortSetting::testSetting_data()
{
    QTest::addColumn<quint16>("priority");
    QTest::addColumn<quint16>("pathCost");
    QTest::addColumn<bool>("hairpinMode");

    QTest::newRow("setting1")
            << (quint16) 60             // priority
            << (quint16) 30000          // pathCost
            << true;                    // hairpinMode
}

void BridgePortSetting::testSetting()
{
    QFETCH(quint16, priority);
    QFETCH(quint16, pathCost);
    QFETCH(bool, hairpinMode);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_BRIDGE_PORT_PRIORITY), priority);
    map.insert(QLatin1String(NM_SETTING_BRIDGE_PORT_PATH_COST), pathCost);
    map.insert(QLatin1String(NM_SETTING_BRIDGE_PORT_PATH_COST), hairpinMode);

    NetworkManager::BridgePortSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    foreach (const QString & key, map.keys()) {
        QCOMPARE(map.value(key), map1.value(key));
    }
}

QTEST_MAIN(BridgePortSetting)
#include "bridgeportsetting.moc"
