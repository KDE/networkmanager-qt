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

#include "managertest.h"

#include "manager.h"
#include "wireddevice.h"

#include "fakenetwork/wireddevice.h"

#include <QTest>
#include <QSignalSpy>
#include <QtTest/qsignalspy.h>

void ManagerTest::initTestCase()
{
    fakeNetwork = new FakeNetwork();
}

void ManagerTest::testDevices()
{
    WiredDevice *device = new WiredDevice();
    /* Device properties */
    device->setAutoconnect(true);
    device->setCapabilities(3);
    device->setDeviceType(1);
    device->setDriver(QLatin1Literal("e1000e"));
    device->setDriverVersion(QLatin1Literal("2.3.2-k"));
    device->setFirmwareMissing(false);
    device->setFirmwareVersion(QLatin1Literal("0.13-3"));
    device->setInterface(QLatin1Literal("em1"));
    device->setManaged(true);
    device->setUdi(QLatin1Literal("/sys/devices/pci0000:00/0000:00:19.0/net/em1"));

    /* Wired device properties */
    device->setCarrier(true);
    device->setHwAddress(QLatin1Literal("F0:DE:F1:FB:30:C1"));
    device->setPermanentHwAddress(QLatin1Literal("F0:DE:F1:FB:30:C1"));

    QSignalSpy addDeviceSpy(NetworkManager::notifier(), SIGNAL(deviceAdded(QString)));
    connect(NetworkManager::notifier(), &NetworkManager::Notifier::deviceAdded, this, &ManagerTest::testDeviceAdded);
    fakeNetwork->addDevice(device);
    QVERIFY(addDeviceSpy.wait());
    QCOMPARE(NetworkManager::networkInterfaces().count(), 1);
    QCOMPARE(NetworkManager::networkInterfaces().first()->uni(), addDeviceSpy.at(0).at(0).toString());
    const QString addedDevicePath = NetworkManager::networkInterfaces().first()->uni();

    QSignalSpy removeDeviceSpy(NetworkManager::notifier(), SIGNAL(deviceRemoved(QString)));
    fakeNetwork->removeDevice(device);
    QVERIFY(removeDeviceSpy.wait());
    QVERIFY(NetworkManager::networkInterfaces().isEmpty());
    QCOMPARE(removeDeviceSpy.at(0).at(0).toString(), addedDevicePath);

    delete device;
}

void ManagerTest::testDeviceAdded(const QString &dev)
{
    NetworkManager::Device::Ptr device = NetworkManager::findNetworkInterface(dev);

    QCOMPARE(device->autoconnect(), true);
    QCOMPARE(device->capabilities(), 3);
    QCOMPARE(device->type(), NetworkManager::Device::Ethernet);
    QCOMPARE(device->driver(), QLatin1Literal("e1000e"));
    QCOMPARE(device->driverVersion(), QLatin1Literal("2.3.2-k"));
    QCOMPARE(device->firmwareMissing(), false);
    QCOMPARE(device->firmwareVersion(), QLatin1Literal("0.13-3"));
    QCOMPARE(device->interfaceName(), QLatin1Literal("em1"));
    QCOMPARE(device->managed(), true);
    QCOMPARE(device->udi(), QLatin1Literal("/sys/devices/pci0000:00/0000:00:19.0/net/em1"));

    NetworkManager::WiredDevice::Ptr wiredDevice = device.objectCast<NetworkManager::WiredDevice>();

    QCOMPARE(wiredDevice->carrier(), true);
    QCOMPARE(wiredDevice->hardwareAddress(), QLatin1Literal("F0:DE:F1:FB:30:C1"));
    QCOMPARE(wiredDevice->permanentHardwareAddress(), QLatin1Literal("F0:DE:F1:FB:30:C1"));
}

void ManagerTest::testManager()
{
    QCOMPARE(NetworkManager::connectivity(), NetworkManager::NoConnectivity);
//     QCOMPARE(NetworkManager::status(), NetworkManager::Disconnected);
    QCOMPARE(NetworkManager::version(), QLatin1Literal("0.9.10.0"));

    QSignalSpy wirelessEnabledChanged(NetworkManager::notifier(), SIGNAL(wirelessEnabledChanged(bool)));
    QCOMPARE(NetworkManager::isWirelessEnabled(), true);
    fakeNetwork->setWirelessEnabled(false);
    QVERIFY(wirelessEnabledChanged.wait());
    QCOMPARE(wirelessEnabledChanged.count(), 1);
    QVERIFY(wirelessEnabledChanged.at(0).at(0).toBool() == false);
    QCOMPARE(NetworkManager::isWirelessEnabled(), false);

    QSignalSpy wirelessHwEnabledChanged(NetworkManager::notifier(), SIGNAL(wirelessHardwareEnabledChanged(bool)));
    QCOMPARE(NetworkManager::isWirelessHardwareEnabled(), true);
    fakeNetwork->setWirelessHardwareEnabled(false);
    QVERIFY(wirelessHwEnabledChanged.wait());
    QCOMPARE(wirelessHwEnabledChanged.count(), 1);
    QVERIFY(wirelessHwEnabledChanged.at(0).at(0).toBool() == false);
    QCOMPARE(NetworkManager::isWirelessHardwareEnabled(), false);

#if !NM_CHECK_VERSION(1, 2, 0)
    QSignalSpy wimaxEnabledChanged(NetworkManager::notifier(), SIGNAL(wimaxEnabledChanged(bool)));
    QCOMPARE(NetworkManager::isWimaxEnabled(), true);
    fakeNetwork->setWimaxEnabled(false);
    QVERIFY(wimaxEnabledChanged.wait());
    QCOMPARE(wimaxEnabledChanged.count(), 1);
    QVERIFY(wimaxEnabledChanged.at(0).at(0).toBool() == false);
    QCOMPARE(NetworkManager::isWimaxEnabled(), false);

    QSignalSpy wimaxHwEnabledChanged(NetworkManager::notifier(), SIGNAL(wimaxHardwareEnabledChanged(bool)));
    QCOMPARE(NetworkManager::isWimaxHardwareEnabled(), true);
    fakeNetwork->setWimaxHardwareEnabled(false);
    QVERIFY(wimaxHwEnabledChanged.wait());
    QCOMPARE(wimaxHwEnabledChanged.count(), 1);
    QVERIFY(wimaxHwEnabledChanged.at(0).at(0).toBool() == false);
    QCOMPARE(NetworkManager::isWimaxHardwareEnabled(), false);
#endif

    QSignalSpy wwanEnabledChanged(NetworkManager::notifier(), SIGNAL(wwanEnabledChanged(bool)));
    QCOMPARE(NetworkManager::isWwanEnabled(), true);
    fakeNetwork->setWwanEnabled(false);
    QVERIFY(wwanEnabledChanged.wait());
    QCOMPARE(wwanEnabledChanged.count(), 1);
    QVERIFY(wwanEnabledChanged.at(0).at(0).toBool() == false);
    QCOMPARE(NetworkManager::isWwanEnabled(), false);

    // We make it here, so we can set all values back for futher testing
    fakeNetwork->setWirelessEnabled(true);
    fakeNetwork->setWirelessHardwareEnabled(true);
#if !NM_CHECK_VERSION(1, 2, 0)
    fakeNetwork->setWimaxEnabled(true);
    fakeNetwork->setWimaxHardwareEnabled(true);
#endif
    fakeNetwork->setWwanEnabled(true);
}

QTEST_MAIN(ManagerTest)
