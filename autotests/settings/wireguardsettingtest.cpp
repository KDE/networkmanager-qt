/*
    Copyright 2019 Jan Grulich <jgrulich@redhat.com>

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

#include "wireguardsettingtest.h"

#include "settings/wireguardsetting.h"

#include <libnm/NetworkManager.h>

#include <QTest>

#if !NM_CHECK_VERSION(1, 16, 0)
#define NM_SETTING_WIREGUARD_FWMARK            "fwmark"
#define NM_SETTING_WIREGUARD_LISTEN_PORT       "listen-port"
#define NM_SETTING_WIREGUARD_PRIVATE_KEY       "private-key"
#define NM_SETTING_WIREGUARD_PRIVATE_KEY_FLAGS "private-key-flags"
#define NM_SETTING_WIREGUARD_PEERS             "peers"
#define NM_SETTING_WIREGUARD_MTU               "mtu"
#define NM_SETTING_WIREGUARD_PEER_ROUTES       "peer-routes"
#endif

void WireguardSettingTest::testSetting_data()
{
    QTest::addColumn<qint32>("fwmark");
    QTest::addColumn<qint32>("listenPort");
    QTest::addColumn<qint32>("mtu");
    QTest::addColumn<bool>("peerRoutes");
    QTest::addColumn<QVariantMap>("peers");
    QTest::addColumn<QString>("privateKey");
    QTest::addColumn<quint32>("privateKeyFlags");

    QTest::newRow("setting1")
            << (qint32) 100                 // fwmark
            << (qint32) 101                 // listenPort
            << (qint32) 102                 // mtu
            << false                        // peerRoutes
            << QVariantMap {{"foo", "bar"}} // peers
            << QString("private-key")       // privateKey
            << (quint32) 2;                 // privateKeyFlags
}

void WireguardSettingTest::testSetting()
{
    QFETCH(qint32, fwmark);
    QFETCH(qint32, listenPort);
    QFETCH(qint32, mtu);
    QFETCH(bool, peerRoutes);
    QFETCH(QVariantMap, peers);
    QFETCH(QString, privateKey);
    QFETCH(quint32, privateKeyFlags);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_WIREGUARD_FWMARK), fwmark);
    map.insert(QLatin1String(NM_SETTING_WIREGUARD_LISTEN_PORT), listenPort);
    map.insert(QLatin1String(NM_SETTING_WIREGUARD_MTU), mtu);
    map.insert(QLatin1String(NM_SETTING_WIREGUARD_PEER_ROUTES), peerRoutes);
    map.insert(QLatin1String(NM_SETTING_WIREGUARD_PEERS), peers);
    map.insert(QLatin1String(NM_SETTING_WIREGUARD_PRIVATE_KEY), privateKey);
    map.insert(QLatin1String(NM_SETTING_WIREGUARD_PRIVATE_KEY_FLAGS), privateKeyFlags);

    NetworkManager::WireguardSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    // Will fail if set some default values, because they are skipped in toMap() method
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        if (it.key() != QLatin1String(NM_SETTING_WIREGUARD_PEERS)) {
            QCOMPARE(it.value(), map1.value(it.key()));
        }
        ++it;
    }

    QVariantMap list = map.value(QLatin1String(NM_SETTING_WIREGUARD_PEERS)).toMap();
    QVariantMap list1 = map1.value(QLatin1String(NM_SETTING_WIREGUARD_PEERS)).toMap();

    QCOMPARE(list.count(), list1.count());

    QVariantMap::const_iterator it2 = list.constBegin();
    while (it2 != list.constEnd()) {
        QCOMPARE(it2.value(), list1.value(it2.key()));
        ++it2;
    }
}

QTEST_MAIN(WireguardSettingTest)

