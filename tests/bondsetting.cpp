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

#include "bondsetting.h"

#include "../settings/bondsetting.h"

#include <nm-setting-bond.h>

#include <QTest>

void BondSetting::testSetting_data()
{
    QTest::addColumn<QString>("interfaceName");
    QTest::addColumn<NMStringMap>("options");

    NMStringMap options;
    options.insert("mode", "802.3ad");

    QTest::newRow("setting1")
            << QString("eth0")       // interfaceName
            << options;              // options
}

void BondSetting::testSetting()
{
    QFETCH(QString, interfaceName);
    QFETCH(NMStringMap, options);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_BOND_INTERFACE_NAME), interfaceName);
    map.insert(QLatin1String(NM_SETTING_BOND_OPTIONS), QVariant::fromValue<NMStringMap>(options));

    NetworkManager::BondSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    QCOMPARE(map.value(QLatin1String(NM_SETTING_BOND_INTERFACE_NAME)), map1.value(QLatin1String(NM_SETTING_BOND_INTERFACE_NAME)));

    NMStringMap stringMap1 = map.value(QLatin1String(NM_SETTING_BOND_OPTIONS)).value<NMStringMap>();
    NMStringMap stringMap2 = map1.value(QLatin1String(NM_SETTING_BOND_OPTIONS)).value<NMStringMap>();

    NMStringMap::const_iterator it = stringMap1.constBegin();
    while (it != stringMap1.constEnd()) {
        QCOMPARE(it.value(), stringMap2.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(BondSetting)
#include "bondsetting.moc"
