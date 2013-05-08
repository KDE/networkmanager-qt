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

#include "olpcmeshsetting.h"

#include "../settings/olpcmeshsetting.h"

#include <nm-setting-olpc-mesh.h>

void OlpcMeshSetting::testSetting_data()
{
    QTest::addColumn<QByteArray>("ssid");
    QTest::addColumn<quint32>("channel");
    QTest::addColumn<QByteArray>("dhcpAnycastAddress");

    QTest::newRow("setting1")
            << QByteArray("ssid")               // ssid
            << (quint32) 1                      // channel
            << QByteArray("00:A0:C9:14:C8:29"); // dhcpAnycastAddress

}

void OlpcMeshSetting::testSetting()
{
    QFETCH(QByteArray, ssid);
    QFETCH(quint32, channel);
    QFETCH(QByteArray, dhcpAnycastAddress);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_OLPC_MESH_SSID), ssid);
    map.insert(QLatin1String(NM_SETTING_OLPC_MESH_CHANNEL), channel);
    map.insert(QLatin1String(NM_SETTING_OLPC_MESH_DHCP_ANYCAST_ADDRESS), dhcpAnycastAddress);

    NetworkManager::Settings::OlpcMeshSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    foreach (const QString & key, map.keys()) {
        QCOMPARE(map.value(key), map1.value(key));
    }
}

QTEST_MAIN(OlpcMeshSetting)
#include "olpcmeshsetting.moc"
