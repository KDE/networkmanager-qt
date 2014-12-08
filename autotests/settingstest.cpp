/*
    Copyright 2014  Jan Grulich <jgrulich@redhat.com>

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

#include "settingstest.h"

#include "settings.h"
#include "settings/ipv4setting.h"
#include "settings/ipv6setting.h"
#include "settings/wiredsetting.h"

#include "fakenetwork/settings.h"

#include <QTest>
#include <QSignalSpy>
#include <QtTest/qsignalspy.h>
#include <QDebug>

void SettingsTest::initTestCase()
{
    fakeNetwork = new FakeNetwork();
}

void SettingsTest::testConnections()
{
    NetworkManager::ConnectionSettings::Ptr connectionSettings = NetworkManager::ConnectionSettings::Ptr(new NetworkManager::ConnectionSettings(NetworkManager::ConnectionSettings::Wired));
    connectionSettings->setId("Wired connection");
    connectionSettings->setUuid(QLatin1Literal("39af79a5-b053-4893-9378-7342a5a30d06"));
    NetworkManager::Ipv4Setting::Ptr ipv4Setting = connectionSettings->setting(NetworkManager::Setting::Ipv4).dynamicCast<NetworkManager::Ipv4Setting>();
    ipv4Setting->setInitialized(true);
    ipv4Setting->setMethod(NetworkManager::Ipv4Setting::Automatic);
    NetworkManager::Ipv6Setting::Ptr ipv6Setting = connectionSettings->setting(NetworkManager::Setting::Ipv6).dynamicCast<NetworkManager::Ipv6Setting>();
    ipv6Setting->setInitialized(true);
    ipv6Setting->setMethod(NetworkManager::Ipv6Setting::Automatic);
    NetworkManager::WiredSetting::Ptr wiredSetting = connectionSettings->setting(NetworkManager::Setting::Wired).dynamicCast<NetworkManager::WiredSetting>();
    wiredSetting->setInitialized(true);
    // Something needs to be set to not use default values, when using default values we get an empty map
    wiredSetting->setSpeed(100);

    NetworkManager::addConnection(connectionSettings->toMap());
    connect(NetworkManager::settingsNotifier(), &NetworkManager::SettingsNotifier::connectionAdded, this, &SettingsTest::testConnectionAdded);
    QSignalSpy connectionAddedSpy(NetworkManager::settingsNotifier(), SIGNAL(connectionAdded(QString)));
    QVERIFY(connectionAddedSpy.wait());
    QCOMPARE(NetworkManager::listConnections().count(), 1);
    QCOMPARE(NetworkManager::listConnections().first()->path(), connectionAddedSpy.at(0).at(0).toString());

    NetworkManager::Connection::Ptr connection = NetworkManager::listConnections().first();
    QSignalSpy connectionRemovedSpy(NetworkManager::settingsNotifier(), SIGNAL(connectionRemoved(QString)));
    connection->remove();
    QVERIFY(connectionRemovedSpy.wait());
    QCOMPARE(NetworkManager::listConnections().count(), 0);
    QCOMPARE(connection->path(), connectionRemovedSpy.at(0).at(0).toString());

    // Compare hostname we set in fake network
    QCOMPARE(NetworkManager::hostname(), QLatin1Literal("fake-hostname"));
}

void SettingsTest::testConnectionAdded(const QString &connection)
{
    NetworkManager::Connection::Ptr addedConnection = NetworkManager::findConnection(connection);
    QCOMPARE(addedConnection->uuid(), QLatin1Literal("39af79a5-b053-4893-9378-7342a5a30d06"));
    QCOMPARE(addedConnection->path(), connection);

    NetworkManager::ConnectionSettings::Ptr connectionSettings = addedConnection->settings();
    NetworkManager::Ipv4Setting::Ptr ipv4Setting = connectionSettings->setting(NetworkManager::Setting::Ipv4).dynamicCast<NetworkManager::Ipv4Setting>();
    QCOMPARE(ipv4Setting->method(), NetworkManager::Ipv4Setting::Automatic);
    NetworkManager::Ipv6Setting::Ptr ipv6Setting = connectionSettings->setting(NetworkManager::Setting::Ipv6).dynamicCast<NetworkManager::Ipv6Setting>();
    QCOMPARE(ipv6Setting->method(), NetworkManager::Ipv6Setting::Automatic);
    NetworkManager::WiredSetting::Ptr wiredSetting = connectionSettings->setting(NetworkManager::Setting::Wired).dynamicCast<NetworkManager::WiredSetting>();
    QVERIFY(wiredSetting->speed() == 100);
    wiredSetting->setSpeed(10);

    addedConnection->update(connectionSettings->toMap());
    QSignalSpy connectionUpdatedSpy(addedConnection.data(), SIGNAL(updated()));
    QVERIFY(connectionUpdatedSpy.wait());
    QVERIFY(wiredSetting->speed() == 10);
}

QTEST_MAIN(SettingsTest)
