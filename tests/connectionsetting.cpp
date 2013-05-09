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

#include "connectionsetting.h"

#include "../settings/connectionsettings.h"

#include <nm-setting-connection.h>

void ConnectionSetting::testSetting_data()
{
    QTest::addColumn<QString>("id");
    QTest::addColumn<QString>("uuid");
    QTest::addColumn<QString>("type");
    QTest::addColumn<QStringList>("permissions");
    QTest::addColumn<bool>("autoconnect");
    QTest::addColumn<quint64>("timestamp");
    QTest::addColumn<bool>("readOnly");
    QTest::addColumn<QString>("zone");
    QTest::addColumn<QString>("master");
    QTest::addColumn<QString>("slaveType");

    QStringList permissions;
    permissions << "user:foo:bar";

    QTest::newRow("setting1")
            << QString("test")                                  // id
            << QString("2815492f-7e56-435e-b2e9-246bd7cdc664")  // uuid
            << QString("802-3-ethernet")                        // type
            << permissions                                      // permissions
            << false                                            // autoconnect
            << (quint64) 1355945444                             // timestamp
            << true                                             // readOnly
            << QString("Home")                                  // zone
            << QString("3ba0ba8c-58f3-427d-bce4-3c8200277a5c")  // master
            << QString("bond");                                 // slaveType
}

void ConnectionSetting::testSetting()
{
    QFETCH(QString, id);
    QFETCH(QString, uuid);
    QFETCH(QString, type);
    QFETCH(QStringList, permissions);
    QFETCH(bool, autoconnect);
    QFETCH(quint64, timestamp);
    QFETCH(bool, readOnly);
    QFETCH(QString, zone);
    QFETCH(QString, master);
    QFETCH(QString, slaveType);

    NMVariantMapMap mapmap;
    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_CONNECTION_ID), id);
    map.insert(QLatin1String(NM_SETTING_CONNECTION_UUID), uuid);
    map.insert(QLatin1String(NM_SETTING_CONNECTION_TYPE), type);
    map.insert(QLatin1String(NM_SETTING_CONNECTION_PERMISSIONS), permissions);
    map.insert(QLatin1String(NM_SETTING_CONNECTION_AUTOCONNECT), autoconnect);
    map.insert(QLatin1String(NM_SETTING_CONNECTION_TIMESTAMP), timestamp);
    map.insert(QLatin1String(NM_SETTING_CONNECTION_READ_ONLY), readOnly);
    map.insert(QLatin1String(NM_SETTING_CONNECTION_ZONE), zone);
    map.insert(QLatin1String(NM_SETTING_CONNECTION_MASTER), master);
    map.insert(QLatin1String(NM_SETTING_CONNECTION_SLAVE_TYPE), slaveType);

    mapmap.insert(QLatin1String(NM_SETTING_CONNECTION_SETTING_NAME), map);

    NetworkManager::ConnectionSettings setting;
    setting.fromMap(mapmap);

    NMVariantMapMap mapmap1 = setting.toMap();
    QVariantMap map1 = mapmap1.value(QLatin1String(NM_SETTING_CONNECTION_SETTING_NAME));

    foreach (const QString & key, map.keys()) {
        QCOMPARE(map.value(key), map1.value(key));
    }
}

QTEST_MAIN(ConnectionSetting)
#include "connectionsetting.moc"
