/*
    Copyright 2012  Jan Grulich <jgrulich@redhat.com>

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

#include "pppsetting.h"

#include "../settings/ppp.h"

#include <nm-setting-ppp.h>

void PppSetting::testSetting_data()
{
    QTest::addColumn<bool>("noAuth");
    QTest::addColumn<bool>("refuseEap");
    QTest::addColumn<bool>("refusePap");
    QTest::addColumn<bool>("refuseChap");
    QTest::addColumn<bool>("refuseMschap");
    QTest::addColumn<bool>("refuseMschapv2");
    QTest::addColumn<bool>("noBsdComp");
    QTest::addColumn<bool>("noDeflate");
    QTest::addColumn<bool>("noVjCompare");
    QTest::addColumn<bool>("requireMppe");
    QTest::addColumn<bool>("requireMppe128");
    QTest::addColumn<bool>("mppeStateful");
    QTest::addColumn<bool>("cRtsCts");
    QTest::addColumn<quint32>("baud");
    QTest::addColumn<quint32>("mru");
    QTest::addColumn<quint32>("mtu");
    QTest::addColumn<quint32>("lcpEchoFailure");
    QTest::addColumn<quint32>("lcpEchoInterval");

    QTest::newRow("setting1")
            << false            // noAuth
            << true             // refuseEap
            << true             // refusePap
            << true             // refuseChap
            << true             // refuseMschap
            << true             // refuseMschapv2
            << true             // noBsdComp
            << true             // noDeflate
            << true             // noVjCompare
            << true             // requireMppe
            << true             // requireMppe128
            << true             // mppeStateful
            << true             // cRtsCts
            << (quint32) 1000   // baud
            << (quint32) 128    // mru
            << (quint32) 500    // mtu
            << (quint32) 2      // lcpEchoFailure
            << (quint32) 3;     // lcpEchoInterval

}

void PppSetting::testSetting()
{
    QFETCH(bool, noAuth);
    QFETCH(bool, refuseEap);
    QFETCH(bool, refusePap);
    QFETCH(bool, refuseChap);
    QFETCH(bool, refuseMschap);
    QFETCH(bool, refuseMschapv2);
    QFETCH(bool, noBsdComp);
    QFETCH(bool, noDeflate);
    QFETCH(bool, noVjCompare);
    QFETCH(bool, requireMppe);
    QFETCH(bool, requireMppe128);
    QFETCH(bool, mppeStateful);
    QFETCH(bool, cRtsCts);
    QFETCH(quint32, baud);
    QFETCH(quint32, mru);
    QFETCH(quint32, mtu);
    QFETCH(quint32, lcpEchoFailure);
    QFETCH(quint32, lcpEchoInterval);

    QVariantMap map;

    map.insert(QLatin1String(NM_SETTING_PPP_NOAUTH), noAuth);
    map.insert(QLatin1String(NM_SETTING_PPP_REFUSE_EAP), refuseEap);
    map.insert(QLatin1String(NM_SETTING_PPP_REFUSE_PAP), refusePap);
    map.insert(QLatin1String(NM_SETTING_PPP_REFUSE_CHAP), refuseChap);
    map.insert(QLatin1String(NM_SETTING_PPP_REFUSE_MSCHAP), refuseMschap);
    map.insert(QLatin1String(NM_SETTING_PPP_REFUSE_MSCHAPV2), refuseMschapv2);
    map.insert(QLatin1String(NM_SETTING_PPP_NOBSDCOMP), noBsdComp);
    map.insert(QLatin1String(NM_SETTING_PPP_NODEFLATE), noDeflate);
    map.insert(QLatin1String(NM_SETTING_PPP_NO_VJ_COMP), noVjCompare);
    map.insert(QLatin1String(NM_SETTING_PPP_REQUIRE_MPPE), requireMppe);
    map.insert(QLatin1String(NM_SETTING_PPP_REQUIRE_MPPE_128), requireMppe128);
    map.insert(QLatin1String(NM_SETTING_PPP_MPPE_STATEFUL), mppeStateful);
    map.insert(QLatin1String(NM_SETTING_PPP_CRTSCTS), cRtsCts);
    map.insert(QLatin1String(NM_SETTING_PPP_BAUD), baud);
    map.insert(QLatin1String(NM_SETTING_PPP_MRU), mru);
    map.insert(QLatin1String(NM_SETTING_PPP_MTU), mtu);
    map.insert(QLatin1String(NM_SETTING_PPP_LCP_ECHO_FAILURE), lcpEchoFailure);
    map.insert(QLatin1String(NM_SETTING_PPP_LCP_ECHO_INTERVAL), lcpEchoInterval);

    NetworkManager::Settings::PPPSetting setting;
    setting.fromMap(map);

    QVariantMap map1 = setting.toMap();

    foreach (const QString & key, map.keys()) {
        QCOMPARE(map.value(key), map1.value(key));
    }
}

QTEST_MAIN(PppSetting)
#include "pppsetting.moc"
