/*
    Copyright 2016 Jan Grulich <jgrulich@redhat.com>

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

#include "tunsettingtest.h"

#include "settings/tunsetting.h"

#include <nm-version.h>
#if NM_CHECK_VERSION(1, 0, 0)
#include <libnm/NetworkManager.h>
#else
#include <nm-setting-tun.h>
#endif

#include <QTest>

void TunSettingTest::testSetting_data()
{
    QTest::addColumn<QString>("group");
    QTest::addColumn<quint32>("mode");
    QTest::addColumn<bool>("multiQueue");
    QTest::addColumn<QString>("owner");
    QTest::addColumn<bool>("pi");
    QTest::addColumn<bool>("vnetHdr");

    QTest::newRow("setting1")
            << QString("randomGroup")    // group
            << (quint32)2                // mode
            << true                      // multi-queue
            << QString("randomOwner")    // owner
            << true                      // pi
            << true;                     // vnet-hdr
}

void TunSettingTest::testSetting()
{
    QFETCH(QString, group);
    QFETCH(quint32, mode);
    QFETCH(bool, multiQueue);
    QFETCH(QString, owner);
    QFETCH(bool, pi);
    QFETCH(bool, vnetHdr);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_TUN_GROUP), group);
    map.insert(QLatin1String(NM_SETTING_TUN_MODE), mode);
    map.insert(QLatin1String(NM_SETTING_TUN_MULTI_QUEUE), multiQueue);
    map.insert(QLatin1String(NM_SETTING_TUN_OWNER), owner);
    map.insert(QLatin1String(NM_SETTING_TUN_PI), pi);
    map.insert(QLatin1String(NM_SETTING_TUN_VNET_HDR), vnetHdr);


    NetworkManager::TunSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    // Will fail if set some default values, because they are skipped in toMap() method
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        QCOMPARE(it.value(), map1.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(TunSettingTest)

