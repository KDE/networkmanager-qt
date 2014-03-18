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

#include "bluetoothsetting.h"

#include "../settings/bluetoothsetting.h"

#include <nm-setting-bluetooth.h>

#include <QTest>

void BluetoothSetting::testSetting_data()
{
    QTest::addColumn<QByteArray>("address");
    QTest::addColumn<QString>("type");

    QTest::newRow("setting1")
            << QByteArray("00:02:72:00:d4:1a")  // address
            << QString("panu");                 // type

}

void BluetoothSetting::testSetting()
{
    QFETCH(QByteArray, address);
    QFETCH(QString, type);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_BLUETOOTH_BDADDR), address);
    map.insert(QLatin1String(NM_SETTING_BLUETOOTH_TYPE), type);

    NetworkManager::BluetoothSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        QCOMPARE(it.value(), map1.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(BluetoothSetting)
#include "moc_bluetoothsetting.cpp"
