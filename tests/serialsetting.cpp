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

#include "serialsetting.h"

#include "../settings/serialsetting.h"

#include <nm-setting-serial.h>

void SerialSetting::testSetting_data()
{
    QTest::addColumn<quint32>("baud");
    QTest::addColumn<quint32>("bits");
    QTest::addColumn<QChar>("parity");
    QTest::addColumn<quint32>("stopbits");
    QTest::addColumn<quint64>("sendDelay");


    QTest::newRow("setting1")
            << (quint32) 56000    // baud
            << (quint32) 16       // bits
            << QChar('E')         // parity
            << (quint32) 2        // stopbits
            << (quint64) 1000;    // senddelay

}

void SerialSetting::testSetting()
{
    QFETCH(quint32, baud);
    QFETCH(quint32, bits);
    QFETCH(QChar, parity);
    QFETCH(quint32, stopbits);
    QFETCH(quint64, sendDelay);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_SERIAL_BAUD), baud);
    map.insert(QLatin1String(NM_SETTING_SERIAL_BITS), bits);
    map.insert(QLatin1String(NM_SETTING_SERIAL_PARITY), parity);
    map.insert(QLatin1String(NM_SETTING_SERIAL_STOPBITS), stopbits);
    map.insert(QLatin1String(NM_SETTING_SERIAL_SEND_DELAY), sendDelay);

    NetworkManager::SerialSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    foreach (const QString & key, map.keys()) {
        QCOMPARE(map.value(key), map1.value(key));
    }
}

QTEST_MAIN(SerialSetting)
#include "serialsetting.moc"
