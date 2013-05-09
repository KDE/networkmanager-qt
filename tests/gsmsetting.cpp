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

#include "gsmsetting.h"

#include "../settings/gsmsetting.h"

#include <nm-setting-gsm.h>

void GsmSetting::testSetting_data()
{
    QTest::addColumn<QString>("number");
    QTest::addColumn<QString>("username");
    QTest::addColumn<QString>("password");
    QTest::addColumn<qint32>("passwordFlags");
    QTest::addColumn<QString>("apn");
    QTest::addColumn<QString>("networkId");
    QTest::addColumn<qint32>("networkType");
    QTest::addColumn<QString>("pin");
    QTest::addColumn<qint32>("pinFlags");
    QTest::addColumn<quint32>("allowedBand");
    QTest::addColumn<bool>("homeOnly");

    QTest::newRow("setting1")
            << QString("0123456789")    // number
            << QString("foo")           // username
            << QString("bar")           // password
            << (qint32) 2               // passwordFlags
            << QString("test")          // apn
            << QString("MCC-MNC")       // networkId
            << (qint32) 1               // networkType
            << QString("1010")          // pin
            << (qint32) 4               // pinFlags
            << (quint32) 0              // allowedBand
            << true;                    // homeOnly
}

void GsmSetting::testSetting()
{
    QFETCH(QString, number);
    QFETCH(QString, username);
    QFETCH(QString, password);
    QFETCH(qint32, passwordFlags);
    QFETCH(QString, apn);
    QFETCH(QString, networkId);
    QFETCH(qint32, networkType);
    QFETCH(QString, pin);
    QFETCH(qint32, pinFlags);
    QFETCH(quint32, allowedBand);
    QFETCH(bool, homeOnly);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_GSM_NUMBER), number);
    map.insert(QLatin1String(NM_SETTING_GSM_USERNAME), username);
    map.insert(QLatin1String(NM_SETTING_GSM_PASSWORD), password);
    map.insert(QLatin1String(NM_SETTING_GSM_PASSWORD_FLAGS), passwordFlags);
    map.insert(QLatin1String(NM_SETTING_GSM_APN), apn);
    map.insert(QLatin1String(NM_SETTING_GSM_NETWORK_ID), networkId);
    map.insert(QLatin1String(NM_SETTING_GSM_NETWORK_TYPE), networkType);
    map.insert(QLatin1String(NM_SETTING_GSM_PIN), pin);
    map.insert(QLatin1String(NM_SETTING_GSM_PIN_FLAGS), pinFlags);
    map.insert(QLatin1String(NM_SETTING_GSM_ALLOWED_BANDS), allowedBand);
    map.insert(QLatin1String(NM_SETTING_GSM_HOME_ONLY), homeOnly);

    NetworkManager::GsmSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    // Will fail if set some default values, because they are skipped in toMap() method
    foreach (const QString & key, map.keys()) {
        QCOMPARE(map.value(key), map1.value(key));
    }
}

QTEST_MAIN(GsmSetting)
#include "gsmsetting.moc"
