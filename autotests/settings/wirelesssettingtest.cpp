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

#include "wirelesssettingtest.h"

#include "settings/wirelesssetting.h"

#include <nm-version.h>
#if NM_CHECK_VERSION(1, 0, 0)
#include <libnm/NetworkManager.h>
#else
#include <nm-setting-wireless.h>
#endif

#if !NM_CHECK_VERSION(1, 2, 0)
#define NM_SETTING_WIRELESS_MAC_ADDRESS_RANDOMIZATION "mac-address-randomization"
#define NM_SETTING_WIRELESS_POWERSAVE "powersave"
#endif

#if !NM_CHECK_VERSION(1, 4, 0)
#define NM_SETTING_WIRELESS_GENERATE_MAC_ADDRESS_MASK "generate-mac-address-mask"
#endif


#include <QTest>

void WirelessSettingTest::testSetting_data()
{
    QTest::addColumn<QByteArray>("ssid");
    QTest::addColumn<QString>("mode");
    QTest::addColumn<QString>("band");
    QTest::addColumn<quint32>("channel");
    QTest::addColumn<QByteArray>("bssid");
    QTest::addColumn<quint32>("rate");
    QTest::addColumn<quint32>("txPower");
    QTest::addColumn<QByteArray>("macAddress");
    QTest::addColumn<QByteArray>("clonedMacAddress");
    QTest::addColumn<QString>("generateMacAddressMask");
    QTest::addColumn<QStringList>("macAddressBlacklist");
    QTest::addColumn<uint>("macAddressRandomization");
    QTest::addColumn<quint32>("mtu");
    QTest::addColumn<QStringList>("seenBssids");
    QTest::addColumn<uint>("powerSave");
    QTest::addColumn<QString>("security");
    QTest::addColumn<bool>("hidden");

    QStringList macAddressBlacklist;
    macAddressBlacklist << "00:08:C7:1B:8C:02";

    QStringList seenBssids;
    seenBssids << "0005.9a3e.7c0f";

    QTest::newRow("setting1")
            << QByteArray("test")                             // ssid
            << QString("adhoc")                               // mode
            << QString("bg")                                  // band
            << (quint32)1                                     // channel
            << QByteArray("0011.2161.b7c0")                   // bssid
            << (quint32)2                                     // rate
            << (quint32)3                                     // txPower
            << QByteArray("00-B0-D0-86-BB-F7")                // macAddress
            << QByteArray("00-14-22-01-23-4")                 // clonedMacAddress
            << QString("FE:FF:FF:00:00:00 68:F7:28:00:00:00") // generateMacAddressMask
            << macAddressBlacklist                            // macAddressBlacklist
            << (uint) 1                                       // macAddressRandomization
            << (quint32)4                                     // mtu
            << seenBssids                                     // seenBssids
            << (uint) 2                                       // powerSave
            << QString("802-11-wireless-security")            // security
            << true;                                          // hidden
}

void WirelessSettingTest::testSetting()
{
    QFETCH(QByteArray, ssid);
    QFETCH(QString, mode);
    QFETCH(QString, band);
    QFETCH(quint32, channel);
    QFETCH(QByteArray, bssid);
    QFETCH(quint32, rate);
    QFETCH(quint32, txPower);
    QFETCH(QByteArray, macAddress);
    QFETCH(QByteArray, clonedMacAddress);
    QFETCH(QString, generateMacAddressMask);
    QFETCH(QStringList, macAddressBlacklist);
    QFETCH(uint, macAddressRandomization);
    QFETCH(quint32, mtu);
    QFETCH(QStringList, seenBssids);
    QFETCH(uint, powerSave);
    QFETCH(QString, security);
    QFETCH(bool, hidden);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_WIRELESS_SSID), ssid);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_MODE), mode);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_BAND), band);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_CHANNEL), channel);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_BSSID), bssid);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_RATE), rate);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_TX_POWER), txPower);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_MAC_ADDRESS), macAddress);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_CLONED_MAC_ADDRESS), clonedMacAddress);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_GENERATE_MAC_ADDRESS_MASK), generateMacAddressMask);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_MAC_ADDRESS_BLACKLIST), macAddressBlacklist);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_MAC_ADDRESS_RANDOMIZATION), macAddressRandomization);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_MTU), mtu);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SEEN_BSSIDS), seenBssids);
    map.insert(QLatin1String(NM_SETTING_WIRELESS_POWERSAVE), powerSave);
#if NM_CHECK_VERSION(1, 0, 0)
    map.insert(QLatin1String("security"), security);
#else
    map.insert(QLatin1String(NM_SETTING_WIRELESS_SEC), security);
#endif
    map.insert(QLatin1String(NM_SETTING_WIRELESS_HIDDEN), hidden);

    NetworkManager::WirelessSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    // Will fail if set some default values, because they are skipped in toMap() method
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        QCOMPARE(it.value(), map1.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(WirelessSettingTest)
