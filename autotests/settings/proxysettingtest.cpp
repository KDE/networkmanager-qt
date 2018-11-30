/*
    Copyright 2018 Pranav Gade <pranavgade20@gmail.com>

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

#include "proxysettingtest.h"

#include "settings/proxysetting.h"

#include <libnm/NetworkManager.h>

#include <QTest>

void ProxySettingTest::testSetting_data()
{
    QTest::addColumn<bool>("browserOnly");
    QTest::addColumn<quint32>("method");
    QTest::addColumn<QString>("pacScript");
    QTest::addColumn<QString>("pacUrl");

    QTest::newRow("setting1")
            << false                     // browserOnly
            << (quint32)1                // method
            << QString("script")         // pacScript
            << QString("url");           // pacUrl
}

void ProxySettingTest::testSetting()
{
    QFETCH(bool, browserOnly);
    QFETCH(quint32, method);
    QFETCH(QString, pacScript);
    QFETCH(QString, pacUrl);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_PROXY_BROWSER_ONLY), browserOnly);
    map.insert(QLatin1String(NM_SETTING_PROXY_METHOD), method);
    map.insert(QLatin1String(NM_SETTING_PROXY_PAC_SCRIPT), pacScript);
    map.insert(QLatin1String(NM_SETTING_PROXY_PAC_URL), pacUrl);

    NetworkManager::ProxySetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    // Will fail if set some default values, because they are skipped in toMap() method
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        QCOMPARE(it.value(), map1.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(ProxySettingTest)
