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

#include "macsecsettingtest.h"

#include "settings/macsecsetting.h"

#include <libnm/NetworkManager.h>

#include <QTest>

#if !NM_CHECK_VERSION(1, 6, 0)
#define NM_SETTING_MACSEC_PARENT               "parent"
#define NM_SETTING_MACSEC_MODE                 "mode"
#define NM_SETTING_MACSEC_ENCRYPT              "encrypt"
#define NM_SETTING_MACSEC_MKA_CAK              "mka-cak"
#define NM_SETTING_MACSEC_MKA_CAK_FLAGS        "mka-cak-flags"
#define NM_SETTING_MACSEC_MKA_CKN              "mka-ckn"
#define NM_SETTING_MACSEC_PORT                 "port"
#define NM_SETTING_MACSEC_VALIDATION           "validation"
#endif

#if !NM_CHECK_VERSION(1, 12, 0)
#define NM_SETTING_MACSEC_SEND_SCI             "send-sci"
#endif

void MacsecSettingTest::testSetting_data()
{
    QTest::addColumn<bool>("encrypt");
    QTest::addColumn<QString>("mkaCak");
    QTest::addColumn<QString>("mkaCkn");
    QTest::addColumn<qint32>("mode");
    QTest::addColumn<QString>("parent");
    QTest::addColumn<qint32>("port");
    QTest::addColumn<bool>("sendSci");
    QTest::addColumn<qint32>("validation");
    QTest::addColumn<quint32>("mkaCakFlags");

    QTest::newRow("setting1")
            << false             // encrypt
            << QString("abc")    // mkaCak
            << QString("abc")    // mkaCkn
            << (qint32) 1        // mode
            << QString("abc")    // parent
            << (qint32) 666      // port
            << false             // sendSci
            << (qint32) 4        // validation
            << (quint32) 6;      // mkaCakFlags

}

void MacsecSettingTest::testSetting()
{
    QFETCH(bool, encrypt);
    QFETCH(QString, mkaCak);
    QFETCH(QString, mkaCkn);
    QFETCH(qint32, mode);
    QFETCH(QString, parent);
    QFETCH(qint32, port);
    QFETCH(bool, sendSci);
    QFETCH(qint32, validation);
    QFETCH(quint32, mkaCakFlags);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_MACSEC_ENCRYPT), encrypt);
    map.insert(QLatin1String(NM_SETTING_MACSEC_MKA_CAK), mkaCak);
    map.insert(QLatin1String(NM_SETTING_MACSEC_MKA_CKN), mkaCkn);
    map.insert(QLatin1String(NM_SETTING_MACSEC_MODE), mode);
    map.insert(QLatin1String(NM_SETTING_MACSEC_PARENT), parent);
    map.insert(QLatin1String(NM_SETTING_MACSEC_PORT), port);
    map.insert(QLatin1String(NM_SETTING_MACSEC_SEND_SCI), sendSci);
    map.insert(QLatin1String(NM_SETTING_MACSEC_VALIDATION), validation);
    map.insert(QLatin1String(NM_SETTING_MACSEC_MKA_CAK_FLAGS), mkaCakFlags);

    NetworkManager::MacsecSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    // Will fail if set some default values, because they are skipped in toMap() method
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        QCOMPARE(it.value(), map1.value(it.key()));
        ++it;
    }
}

QTEST_MAIN(MacsecSettingTest)
