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

#include "dcbsettingtest.h"

#include "settings/dcbsetting.h"

#include <libnm/NetworkManager.h>

#include <QTest>

void DcbSettingTest::testSetting_data()
{
    QTest::addColumn<QString>("appFcoeMode");
    QTest::addColumn<qint32>("appFcoePriority");
    QTest::addColumn<qint32>("appFipPriority");
    QTest::addColumn<qint32>("appIscsiPriority");
    QTest::addColumn<qint32>("appFcoeFlags");
    QTest::addColumn<qint32>("appFipFlags");
    QTest::addColumn<qint32>("appIscsiFlags");
    QTest::addColumn<qint32>("priorityGroupFlags");
    QTest::addColumn<qint32>("priorityFlowControlFlags");
    QTest::addColumn<UIntList>("priorityFlowControl");
    QTest::addColumn<UIntList>("priorityBandwidth");
    QTest::addColumn<UIntList>("priorityGroupBandwidth");
    QTest::addColumn<UIntList>("priorityGroupId");
    QTest::addColumn<UIntList>("priorityStrictBandwidth");
    QTest::addColumn<UIntList>("priorityTrafficClass");

    UIntList priorityFlowControl = {1, 1, 1, 1, 1, 1, 1, 1};
    UIntList priorityBandwidth = {6, 5, 6, 4, 6, 3, 6, 2};
    UIntList priorityGroupBandwidth = {6, 5, 6, 4, 6, 3, 6, 2};
    UIntList priorityGroupId = {6, 5, 6, 4, 6, 3, 6, 2};
    UIntList priorityStrictBandwidth = {1, 1, 1, 1, 1, 1, 1, 1};
    UIntList priorityTrafficClass = {6, 5, 6, 4, 6, 3, 6, 2};

    QTest::newRow("setting1")
            << QString("vn2vn")         // appFcoeMode
            << (qint32)2                // appFcoePriority
            << (qint32)2                // appFipPriority
            << (qint32)2                // appIscsiPriority
            << (qint32)2                // appFcoeFlags
            << (qint32)2                // appFipFlags
            << (qint32)2                // appIscsiFlags
            << (qint32)2                // priorityGroupFlags
            << (qint32)2                // priorityFlowControlFlags
            << priorityFlowControl      // priorityFlowControl
            << priorityBandwidth        // priorityBandwidth
            << priorityGroupBandwidth   // priorityGroupBandwidth
            << priorityGroupId          // priorityGroupId
            << priorityStrictBandwidth  // priorityStrictBandwidth
            << priorityTrafficClass;    // priorityTrafficClass
}

void DcbSettingTest::testSetting()
{
    QFETCH(QString, appFcoeMode);
    QFETCH(qint32, appFcoePriority);
    QFETCH(qint32, appFipPriority);
    QFETCH(qint32, appIscsiPriority);
    QFETCH(qint32, appFcoeFlags);
    QFETCH(qint32, appFipFlags);
    QFETCH(qint32, appIscsiFlags);
    QFETCH(qint32, priorityGroupFlags);
    QFETCH(qint32, priorityFlowControlFlags);

    QFETCH(UIntList, priorityFlowControl);
    QFETCH(UIntList, priorityBandwidth);
    QFETCH(UIntList, priorityGroupBandwidth);
    QFETCH(UIntList, priorityGroupId);
    QFETCH(UIntList, priorityStrictBandwidth);
    QFETCH(UIntList, priorityTrafficClass);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_DCB_APP_FCOE_MODE), appFcoeMode);
    map.insert(QLatin1String(NM_SETTING_DCB_APP_FCOE_PRIORITY), appFcoePriority);
    map.insert(QLatin1String(NM_SETTING_DCB_APP_FIP_PRIORITY), appFipPriority);
    map.insert(QLatin1String(NM_SETTING_DCB_APP_ISCSI_PRIORITY), appIscsiPriority);
    map.insert(QLatin1String(NM_SETTING_DCB_APP_FCOE_FLAGS), appFcoeFlags);
    map.insert(QLatin1String(NM_SETTING_DCB_APP_FIP_FLAGS), appFipFlags);
    map.insert(QLatin1String(NM_SETTING_DCB_APP_ISCSI_FLAGS), appIscsiFlags);
    map.insert(QLatin1String(NM_SETTING_DCB_PRIORITY_GROUP_FLAGS), priorityGroupFlags);
    map.insert(QLatin1String(NM_SETTING_DCB_PRIORITY_FLOW_CONTROL_FLAGS), priorityFlowControlFlags);

    map.insert(QLatin1String(NM_SETTING_DCB_PRIORITY_FLOW_CONTROL), QVariant::fromValue(priorityFlowControl));
    map.insert(QLatin1String(NM_SETTING_DCB_PRIORITY_BANDWIDTH), QVariant::fromValue(priorityBandwidth));
    map.insert(QLatin1String(NM_SETTING_DCB_PRIORITY_GROUP_BANDWIDTH), QVariant::fromValue(priorityGroupBandwidth));
    map.insert(QLatin1String(NM_SETTING_DCB_PRIORITY_GROUP_ID), QVariant::fromValue(priorityGroupId));
    map.insert(QLatin1String(NM_SETTING_DCB_PRIORITY_STRICT_BANDWIDTH), QVariant::fromValue(priorityStrictBandwidth));
    map.insert(QLatin1String(NM_SETTING_DCB_PRIORITY_TRAFFIC_CLASS), QVariant::fromValue(priorityTrafficClass));

    NetworkManager::DcbSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    // Will fail if set some default values, because they are skipped in toMap() method
    QVariantMap::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        if (it.key() != QLatin1String(NM_SETTING_DCB_PRIORITY_FLOW_CONTROL) &&
            it.key() != QLatin1String(NM_SETTING_DCB_PRIORITY_BANDWIDTH) &&
            it.key() != QLatin1String(NM_SETTING_DCB_PRIORITY_GROUP_BANDWIDTH) &&
            it.key() != QLatin1String(NM_SETTING_DCB_PRIORITY_GROUP_ID) &&
            it.key() != QLatin1String(NM_SETTING_DCB_PRIORITY_STRICT_BANDWIDTH) &&
            it.key() != QLatin1String(NM_SETTING_DCB_PRIORITY_TRAFFIC_CLASS))
        {
            QCOMPARE(it.value(), map1.value(it.key()));
        }
        ++it;
    }

    QCOMPARE(map.value(QLatin1String(NM_SETTING_DCB_PRIORITY_FLOW_CONTROL)).value<UIntList>(),
             map1.value(QLatin1String(NM_SETTING_DCB_PRIORITY_FLOW_CONTROL)).value<UIntList>());
    QCOMPARE(map.value(QLatin1String(NM_SETTING_DCB_PRIORITY_BANDWIDTH)).value<UIntList>(),
             map1.value(QLatin1String(NM_SETTING_DCB_PRIORITY_BANDWIDTH)).value<UIntList>());
    QCOMPARE(map.value(QLatin1String(NM_SETTING_DCB_PRIORITY_GROUP_BANDWIDTH)).value<UIntList>(),
             map1.value(QLatin1String(NM_SETTING_DCB_PRIORITY_GROUP_BANDWIDTH)).value<UIntList>());
    QCOMPARE(map.value(QLatin1String(NM_SETTING_DCB_PRIORITY_GROUP_ID)).value<UIntList>(),
             map1.value(QLatin1String(NM_SETTING_DCB_PRIORITY_GROUP_ID)).value<UIntList>());
    QCOMPARE(map.value(QLatin1String(NM_SETTING_DCB_PRIORITY_STRICT_BANDWIDTH)).value<UIntList>(),
             map1.value(QLatin1String(NM_SETTING_DCB_PRIORITY_STRICT_BANDWIDTH)).value<UIntList>());
    QCOMPARE(map.value(QLatin1String(NM_SETTING_DCB_PRIORITY_TRAFFIC_CLASS)).value<UIntList>(),
             map1.value(QLatin1String(NM_SETTING_DCB_PRIORITY_TRAFFIC_CLASS)).value<UIntList>());
}

QTEST_MAIN(DcbSettingTest)
