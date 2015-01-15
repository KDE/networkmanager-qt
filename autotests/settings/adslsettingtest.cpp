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

#include "adslsettingtest.h"

#include "settings/adslsetting.h"

#include <nm-version.h>
#if NM_CHECK_VERSION(1, 0, 0)
#include <libnm/NetworkManager.h>
#else
#include <nm-setting-adsl.h>
#endif

#include <QTest>

void AdslSettingTest::testSetting_data()
{
    QTest::addColumn<QString>("username");
    QTest::addColumn<QString>("password");
    QTest::addColumn<quint32>("passwordFlags");
    QTest::addColumn<QString>("protocol");
    QTest::addColumn<QString>("encapsulation");
    QTest::addColumn<quint32>("vpi");
    QTest::addColumn<quint32>("vci");

    QTest::newRow("setting1")
            << QString("username")   // username
            << QString("password")   // options
            << (quint32) 2           // passwordFlags
            << "pppoa"               // protocol
            << "llc"                 // encapsulation
            << (quint32) 8           // vpi
            << (quint32) 35;         // vci
}

void AdslSettingTest::testSetting()
{
    QFETCH(QString, username);
    QFETCH(QString, password);
    QFETCH(quint32, passwordFlags);
    QFETCH(QString, protocol);
    QFETCH(QString, encapsulation);
    QFETCH(quint32, vpi);
    QFETCH(quint32, vci);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_ADSL_USERNAME), username);
    map.insert(QLatin1String(NM_SETTING_ADSL_PASSWORD), password);
    map.insert(QLatin1String(NM_SETTING_ADSL_PASSWORD_FLAGS), passwordFlags);
    map.insert(QLatin1String(NM_SETTING_ADSL_PROTOCOL), protocol);
    map.insert(QLatin1String(NM_SETTING_ADSL_ENCAPSULATION), encapsulation);
    map.insert(QLatin1String(NM_SETTING_ADSL_VPI), vpi);
    map.insert(QLatin1String(NM_SETTING_ADSL_VCI), vci);

    NetworkManager::AdslSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        QCOMPARE(it.value(), map1.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(AdslSettingTest)
