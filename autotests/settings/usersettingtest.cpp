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

#include "usersettingtest.h"

#include "settings/usersetting.h"

#include <libnm/NetworkManager.h>

#include <QTest>

#if !NM_CHECK_VERSION(1, 8, 0)
#define NM_SETTING_USER_DATA            "data"
#endif

void UserSettingTest::testSetting_data()
{
    QTest::addColumn<NMStringMap>("data");

    NMStringMap data;
    data.insert("dat", "abc");

    QTest::newRow("setting1")
            << data;              // data
}

void UserSettingTest::testSetting()
{
    QFETCH(NMStringMap, data);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_USER_DATA), QVariant::fromValue<NMStringMap>(data));

    NetworkManager::UserSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    NMStringMap stringMap1 = map.value(QLatin1String(NM_SETTING_USER_DATA)).value<NMStringMap>();
    NMStringMap stringMap2 = map1.value(QLatin1String(NM_SETTING_USER_DATA)).value<NMStringMap>();

    NMStringMap::const_iterator it = stringMap1.constBegin();
    while (it != stringMap1.constEnd()) {
        QCOMPARE(it.value(), stringMap2.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(UserSettingTest)
