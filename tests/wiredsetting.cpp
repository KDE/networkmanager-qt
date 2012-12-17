/*
    Copyright 2012  Jan Grulich <jgrulich@redhat.com>

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

#include "wiredsetting.h"

#include <nm-setting-wired.h>
#include <QtNetworkManager/802-3-ethernet.h>

typedef QMap<QString,QString> MapStringString;
Q_DECLARE_METATYPE(MapStringString)

void WiredSetting::testSetting_data()
{
    QTest::addColumn<QString>("port");
    QTest::addColumn<quint32>("speed");
    QTest::addColumn<QString>("duplex");
    QTest::addColumn<bool>("autoNegotiate");
    QTest::addColumn<QByteArray>("macAddress");
    QTest::addColumn<QByteArray>("clonedMacAddress");
    QTest::addColumn<QStringList>("macAddressBlacklist");
    QTest::addColumn<quint32>("mtu");
    QTest::addColumn<QStringList>("s390Subchannels");
    QTest::addColumn<QString>("s390NetType");
    QTest::addColumn<MapStringString>("s390Options");

    QTest::newRow("setting1")
            << QString("tp") // port
            << (quint32) 100 // speed
            << QString("full") // duplex
            << false // autoNegotiate
            << QByteArray("00-B0-D0-86-BB-F7") // macAddress
            << QByteArray()  // clonedMacAddress
            << QStringList() // macAddressBlacklist
            << (quint32) 200 // mtu
            << QStringList() // s390Subchannels
            << QString("qeth") // s390NetType
            << MapStringString(); // s390Options
}

void WiredSetting::testSetting()
{
    QFETCH(QString, port);
    QFETCH(quint32, speed);
    QFETCH(QString, duplex);
    QFETCH(bool, autoNegotiate);
    QFETCH(QByteArray, macAddress);
    //QFETCH(QByteArray, clonedMacAddress);
    //QFETCH(QStringList, macAddressBlacklist);
    QFETCH(quint32, mtu);
    //QFETCH(QStringList, s390Subchannels);
    QFETCH(QString, s390NetType);
    //QFETCH(MapStringString, s390Options);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_WIRED_PORT), port);
    map.insert(QLatin1String(NM_SETTING_WIRED_SPEED), speed);
    map.insert(QLatin1String(NM_SETTING_WIRED_DUPLEX), duplex);
    map.insert(QLatin1String(NM_SETTING_WIRED_AUTO_NEGOTIATE), autoNegotiate);
    map.insert(QLatin1String(NM_SETTING_WIRED_MAC_ADDRESS), macAddress);
    //map.insert(QLatin1String(NM_SETTING_WIRED_CLONED_MAC_ADDRESS), clonedMacAddress);
    //map.insert(QLatin1String(NM_SETTING_WIRED_MAC_ADDRESS_BLACKLIST), macAddressBlacklist);
    map.insert(QLatin1String(NM_SETTING_WIRED_MTU), mtu);
    //map.insert(QLatin1String(NM_SETTING_WIRED_S390_SUBCHANNELS), s390Subchannels);
    map.insert(QLatin1String(NM_SETTING_WIRED_S390_NETTYPE), s390NetType);
    //map.insert(QLatin1String(NM_SETTING_WIRED_S390_OPTIONS), s390Options);

    NetworkManager::Settings::WiredSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    foreach (const QString & key, map.keys()) {
        QCOMPARE(map.value(key), map1.value(key));
    }
}

QTEST_MAIN(WiredSetting)
#include "wiredsetting.moc"
