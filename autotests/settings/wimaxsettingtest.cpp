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

#include "wimaxsettingtest.h"

#include "settings/wimaxsetting.h"

#include <libnm/NetworkManager.h>

#include <QTest>

void WimaxSettingTest::testSetting_data()
{
    QTest::addColumn<QString>("networkName");
    QTest::addColumn<QByteArray>("macAddress");

    QTest::newRow("setting1")
            << QString("networkname")           // networkName
            << QByteArray("01:23:45:67:89:ab"); // macAddress

}

void WimaxSettingTest::testSetting()
{
    QFETCH(QString, networkName);
    QFETCH(QByteArray, macAddress);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_WIMAX_NETWORK_NAME), networkName);
    map.insert(QLatin1String(NM_SETTING_WIMAX_MAC_ADDRESS), macAddress);

    NetworkManager::WimaxSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        QCOMPARE(it.value(), map1.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(WimaxSettingTest)
