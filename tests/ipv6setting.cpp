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

#include "ipv6setting.h"

#include <arpa/inet.h>
#include <nm-setting-ip6-config.h>
#include <QtNetworkManager/generic-types.h>
#include <QtNetworkManager/ipv6.h>

//TODO: Test DNS,IPv6Addresses and IPv6Routes
void IPv6Setting::testSetting_data()
{
    QTest::addColumn<QString>("method");
    //QTest::addColumn<IpV6DBusNameservers>("dns");
    QTest::addColumn<QStringList>("dnsSearch");
    //QTest::addColumn<IpV6DBusAddressList>("addresses");
    //QTest::addColumn<IpV6DBusRouteList>("routes");
    QTest::addColumn<bool>("ignoreAutoRoutes");
    QTest::addColumn<bool>("ignoreAutoDns");
    QTest::addColumn<bool>("neverDefault");
    QTest::addColumn<bool>("mayFail");
    QTest::addColumn<quint32>("privacy");

    QStringList dnsSearch;
    dnsSearch << "foo.com";
    dnsSearch << "foo.bar";

    QTest::newRow("setting1")
            << QString("auto")       // method
    //        << dns                   // dns
            << dnsSearch             // dnsSearch
    //        << addresses             // addresses
    //        << routes                // routes
            << true                  // ignoreAutoRoutes
            << true                  // ignoreAutoDns
            << true                  // neverDefault
            << false                 // mayFail
            << (quint32) 0;                    // privacy
}

void IPv6Setting::testSetting()
{
    QFETCH(QString, method);
    //QFETCH(IpV6DBusNameservers, dns);
    QFETCH(QStringList, dnsSearch);
    //QFETCH(IpV6DBusAddressList, addresses);
    //QFETCH(IpV6DBusRouteList, routes);
    QFETCH(bool, ignoreAutoRoutes);
    QFETCH(bool, ignoreAutoDns);
    QFETCH(bool, neverDefault);
    QFETCH(bool, mayFail);
    QFETCH(quint32, privacy);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_IP6_CONFIG_METHOD), method);
    //map.insert(QLatin1String(NM_SETTING_IP6_CONFIG_DNS), QVariant::fromValue(dns));
    map.insert(QLatin1String(NM_SETTING_IP6_CONFIG_DNS_SEARCH), dnsSearch);
    //map.insert(QLatin1String(NM_SETTING_IP6_CONFIG_ADDRESSES), QVariant::fromValue(addresses));
    //map.insert(QLatin1String(NM_SETTING_IP6_CONFIG_ROUTES), QVariant::fromValue(routes));
    map.insert(QLatin1String(NM_SETTING_IP6_CONFIG_IGNORE_AUTO_ROUTES), ignoreAutoRoutes);
    map.insert(QLatin1String(NM_SETTING_IP6_CONFIG_IGNORE_AUTO_DNS), ignoreAutoDns);
    map.insert(QLatin1String(NM_SETTING_IP6_CONFIG_NEVER_DEFAULT), neverDefault);
    map.insert(QLatin1String(NM_SETTING_IP6_CONFIG_MAY_FAIL), mayFail);
    map.insert(QLatin1String(NM_SETTING_IP6_CONFIG_IP6_PRIVACY), privacy);

    NetworkManager::Settings::Ipv6Setting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    // Will fail if set some default values, because they are skipped in toMap() method
    foreach (const QString & key, map.keys()) {
        if (key != QLatin1String(NM_SETTING_IP6_CONFIG_DNS) &&
            key != QLatin1String(NM_SETTING_IP6_CONFIG_ADDRESSES) &&
            key != QLatin1String(NM_SETTING_IP6_CONFIG_ROUTES)) {
                QCOMPARE(map.value(key), map1.value(key));
        }
    }

    /*QCOMPARE(map.value(QLatin1String(NM_SETTING_IP6_CONFIG_DNS)).value<IpV6DBusNameservers>(),
             map1.value(QLatin1String(NM_SETTING_IP6_CONFIG_DNS)).value<IpV6DBusNameservers>());
    QCOMPARE(map.value(QLatin1String(NM_SETTING_IP6_CONFIG_ADDRESSES)).value<IpV6DBusAddressList>(),
             map.value(QLatin1String(NM_SETTING_IP6_CONFIG_ADDRESSES)).value<IpV6DBusAddressList>());
    QCOMPARE(map.value(QLatin1String(NM_SETTING_IP6_CONFIG_ROUTES)).value<IpV6DBusRouteList>(),
             map.value(QLatin1String(NM_SETTING_IP6_CONFIG_ROUTES)).value<IpV6DBusRouteList>());*/
}

QTEST_MAIN(IPv6Setting)
#include "ipv6setting.moc"
