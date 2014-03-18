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

#include "wiredsetting.h"

#include "generic-types.h"
#include "../settings/wiredsetting.h"

#include <nm-setting-wired.h>

#include <QTest>

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
    QTest::addColumn<NMStringMap>("s390Options");

    QStringList macAddressBlacklist;
    macAddressBlacklist << "00:08:C7:1B:8C:02";

    QStringList s390Subchannels;
    s390Subchannels << "0.0.09a0";
    s390Subchannels << "0.0.09a1";
    s390Subchannels << "0.0.09a2";

    QMap<QString, QString> s390Options;
    s390Options.insert("portno", "0");

    QTest::newRow("setting1")
            << QString("tp")                    // port
            << (quint32) 100                    // speed
            << QString("full")                  // duplex
            << false                            // autoNegotiate
            << QByteArray("00-B0-D0-86-BB-F7")  // macAddress
            << QByteArray("00-14-22-01-23-4")   // clonedMacAddress
            << macAddressBlacklist              // macAddressBlacklist
            << (quint32) 200                    // mtu
            << s390Subchannels                  // s390Subchannels
            << QString("qeth")                  // s390NetType
            << s390Options;                     // s390Options
}

void WiredSetting::testSetting()
{
    QFETCH(QString, port);
    QFETCH(quint32, speed);
    QFETCH(QString, duplex);
    QFETCH(bool, autoNegotiate);
    QFETCH(QByteArray, macAddress);
    QFETCH(QByteArray, clonedMacAddress);
    QFETCH(QStringList, macAddressBlacklist);
    QFETCH(quint32, mtu);
    QFETCH(QStringList, s390Subchannels);
    QFETCH(QString, s390NetType);
    QFETCH(NMStringMap, s390Options);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_WIRED_PORT), port);
    map.insert(QLatin1String(NM_SETTING_WIRED_SPEED), speed);
    map.insert(QLatin1String(NM_SETTING_WIRED_DUPLEX), duplex);
    map.insert(QLatin1String(NM_SETTING_WIRED_AUTO_NEGOTIATE), autoNegotiate);
    map.insert(QLatin1String(NM_SETTING_WIRED_MAC_ADDRESS), macAddress);
    map.insert(QLatin1String(NM_SETTING_WIRED_CLONED_MAC_ADDRESS), clonedMacAddress);
    map.insert(QLatin1String(NM_SETTING_WIRED_MAC_ADDRESS_BLACKLIST), macAddressBlacklist);
    map.insert(QLatin1String(NM_SETTING_WIRED_MTU), mtu);
    map.insert(QLatin1String(NM_SETTING_WIRED_S390_SUBCHANNELS), s390Subchannels);
    map.insert(QLatin1String(NM_SETTING_WIRED_S390_NETTYPE), s390NetType);
    map.insert(QLatin1String(NM_SETTING_WIRED_S390_OPTIONS), QVariant::fromValue(s390Options));

    NetworkManager::WiredSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        if (it.key() != QLatin1String(NM_SETTING_WIRED_S390_OPTIONS)) {
            QCOMPARE(it.value(), map1.value(it.key()));
        }
        ++it;
    }

    QCOMPARE(map.value(QLatin1String(NM_SETTING_WIRED_S390_OPTIONS)).value<NMStringMap>(),
             map1.value(QLatin1String(NM_SETTING_WIRED_S390_OPTIONS)).value<NMStringMap>());
}

QTEST_MAIN(WiredSetting)
#include "moc_wiredsetting.cpp"
