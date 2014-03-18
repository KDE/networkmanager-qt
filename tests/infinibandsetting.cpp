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

#include "infinibandsetting.h"

#include "../settings/infinibandsetting.h"

#include <nm-setting-infiniband.h>

#include <QTest>

void InfinibandSetting::testSetting_data()
{
    QTest::addColumn<QByteArray>("macAddress");
    QTest::addColumn<quint32>("mtu");
    QTest::addColumn<QString>("transportMode");

    QTest::newRow("setting1")
            << QByteArray("eth0")       // macAddress
            << (quint32) 1000           // mtu
            << "connected";             // transportMode
}

void InfinibandSetting::testSetting()
{
    QFETCH(QByteArray, macAddress);
    QFETCH(quint32, mtu);
    QFETCH(QString, transportMode);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_INFINIBAND_MAC_ADDRESS), macAddress);
    map.insert(QLatin1String(NM_SETTING_INFINIBAND_MTU), mtu);
    map.insert(QLatin1String(NM_SETTING_INFINIBAND_TRANSPORT_MODE), transportMode);

    NetworkManager::InfinibandSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        QCOMPARE(it.value(), map1.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(InfinibandSetting)
#include "moc_infinibandsetting.cpp"
