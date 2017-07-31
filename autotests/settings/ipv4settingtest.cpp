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

#include "ipv4settingtest.h"

#include "generictypes.h"
#include "settings/ipv4setting.h"

#include <arpa/inet.h>

#include <nm-version.h>
#if NM_CHECK_VERSION(1, 0, 0)
#include <libnm/NetworkManager.h>
#else
#include <nm-setting-ip4-config.h>
#endif

#include <QTest>

void IPv4SettingTest::testSetting_data()
{
    QTest::addColumn<QString>("method");
    QTest::addColumn<UIntList>("dns");
    QTest::addColumn<QStringList>("dnsSearch");
    QTest::addColumn<UIntListList>("addresses");
    QTest::addColumn<UIntListList>("routes");
    QTest::addColumn<int>("routeMetric");
    QTest::addColumn<bool>("ignoreAutoRoutes");
    QTest::addColumn<bool>("ignoreAutoDns");
    QTest::addColumn<QString>("dhcpClientId");
    QTest::addColumn<bool>("dhcpSendHostname");
    QTest::addColumn<QString>("dhcpHostname");
    QTest::addColumn<bool>("neverDefault");
    QTest::addColumn<bool>("mayFail");

    QList<uint> dns;
    dns << inet_addr("10.0.0.1");
    dns << inet_addr("10.0.1.1");

    QStringList dnsSearch;
    dnsSearch << "foo.com";
    dnsSearch << "foo.bar";

    QList<QList<uint> > addresses;
    QList<uint> addr;
    addr << inet_addr("192.168.1.1");
    addr << 25;
    addr << 0;
    addresses << addr;

    QList<QList<uint> > routes;
    QList<uint> routeAddr;
    routeAddr << inet_addr("192.168.1.1");
    routeAddr << 25;
    routeAddr << inet_addr("192.169.1.1");
    routeAddr << 25;
    addresses << routeAddr;

    QTest::newRow("setting1")
            << QString("auto")       // method
            << dns                   // dns
            << dnsSearch             // dnsSearch
            << addresses             // addresses
            << routes                // routes
            << 100                   // routeMetric
            << true                  // ignoreAutoRoutes
            << true                  // ignoreAutoDns
            << QString("home-test")  // dhcpClientId
            << false                 // dhcpSendHostname
            << QString("home-test")  // dhcpHostname
            << true                  // neverDefault
            << false;                // mayFail
}

void IPv4SettingTest::testSetting()
{
    QFETCH(QString, method);
    QFETCH(UIntList, dns);
    QFETCH(QStringList, dnsSearch);
    QFETCH(UIntListList, addresses);
    QFETCH(UIntListList, routes);
    QFETCH(int, routeMetric);
    QFETCH(bool, ignoreAutoRoutes);
    QFETCH(bool, ignoreAutoDns);
    QFETCH(QString, dhcpClientId);
    QFETCH(bool, dhcpSendHostname);
    QFETCH(QString, dhcpHostname);
    QFETCH(bool, neverDefault);
    QFETCH(bool, mayFail);

    QVariantMap map;

    map.insert(QLatin1String(NMQT_SETTING_IP4_CONFIG_METHOD), method);
    map.insert(QLatin1String(NMQT_SETTING_IP4_CONFIG_DNS), QVariant::fromValue(dns));
    map.insert(QLatin1String(NMQT_SETTING_IP4_CONFIG_DNS_SEARCH), dnsSearch);
    map.insert(QLatin1String(NMQT_SETTING_IP4_CONFIG_ADDRESSES), QVariant::fromValue(addresses));
    map.insert(QLatin1String(NMQT_SETTING_IP4_CONFIG_ROUTES), QVariant::fromValue(routes));
    map.insert(QLatin1String(NMQT_SETTING_IP4_CONFIG_ROUTE_METRIC), routeMetric);
    map.insert(QLatin1String(NMQT_SETTING_IP4_CONFIG_IGNORE_AUTO_ROUTES), ignoreAutoRoutes);
    map.insert(QLatin1String(NMQT_SETTING_IP4_CONFIG_IGNORE_AUTO_DNS), ignoreAutoDns);
    map.insert(QLatin1String(NMQT_SETTING_IP4_CONFIG_DHCP_CLIENT_ID), dhcpClientId);
    map.insert(QLatin1String(NMQT_SETTING_IP4_CONFIG_DHCP_SEND_HOSTNAME), dhcpSendHostname);
    map.insert(QLatin1String(NMQT_SETTING_IP4_CONFIG_DHCP_HOSTNAME), dhcpHostname);
    map.insert(QLatin1String(NMQT_SETTING_IP4_CONFIG_NEVER_DEFAULT), neverDefault);
    map.insert(QLatin1String(NMQT_SETTING_IP4_CONFIG_MAY_FAIL), mayFail);

    NetworkManager::Ipv4Setting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    // Will fail if set some default values, because they are skipped in toMap() method
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        if (it.key() != QLatin1String(NMQT_SETTING_IP4_CONFIG_DNS) &&
                it.key() != QLatin1String(NMQT_SETTING_IP4_CONFIG_ADDRESSES) &&
                it.key() != QLatin1String(NMQT_SETTING_IP4_CONFIG_ROUTES)) {
            QCOMPARE(it.value(), map1.value(it.key()));
        }
        ++it;
    }

    QCOMPARE(map.value(QLatin1String(NMQT_SETTING_IP4_CONFIG_DNS)).value<UIntList>(),
             map1.value(QLatin1String(NMQT_SETTING_IP4_CONFIG_DNS)).value<UIntList>());
    QCOMPARE(map.value(QLatin1String(NMQT_SETTING_IP4_CONFIG_ADDRESSES)).value<UIntListList>(),
             map.value(QLatin1String(NMQT_SETTING_IP4_CONFIG_ADDRESSES)).value<UIntListList>());
    QCOMPARE(map.value(QLatin1String(NMQT_SETTING_IP4_CONFIG_ROUTES)).value<UIntListList>(),
             map.value(QLatin1String(NMQT_SETTING_IP4_CONFIG_ROUTES)).value<UIntListList>());
}

QTEST_MAIN(IPv4SettingTest)
