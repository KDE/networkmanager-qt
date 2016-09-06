/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2013 Jan Grulich <jgrulich@redhat.com>

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

#include "wireddevice.h"
#include "wireddevice_p.h"
#include "manager.h"
#include "manager_p.h"

#include "nmdebug.h"

NetworkManager::WiredDevicePrivate::WiredDevicePrivate(const QString &path, WiredDevice *q)
    : DevicePrivate(path, q)
#ifdef NMQT_STATIC
    , wiredIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , wiredIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , bitrate(0)
    , carrier(false)
{
    hardwareAddress = wiredIface.hwAddress();
    permanentHardwareAddress = wiredIface.permHwAddress();
    bitrate = wiredIface.speed() * 1000;
    carrier = wiredIface.carrier();
}

NetworkManager::WiredDevicePrivate::~WiredDevicePrivate()
{
}

NetworkManager::WiredDevice::WiredDevice(const QString &path, QObject *parent)
    : Device(*new NetworkManager::WiredDevicePrivate(path, this), parent)
{
    Q_D(WiredDevice);
#if NM_CHECK_VERSION(1, 4, 0)
    QDBusConnection::systemBus().connect(NetworkManagerPrivate::DBUS_SERVICE, d->uni, NetworkManagerPrivate::FDO_DBUS_PROPERTIES,
                                         QLatin1String("PropertiesChanged"), d, SLOT(dbusPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    connect(&d->wiredIface, &OrgFreedesktopNetworkManagerDeviceWiredInterface::PropertiesChanged, d, &WiredDevicePrivate::propertiesChanged);
#endif
}

NetworkManager::WiredDevice::~WiredDevice()
{
}

NetworkManager::Device::Type NetworkManager::WiredDevice::type() const
{
    return NetworkManager::Device::Ethernet;
}

QString NetworkManager::WiredDevice::hardwareAddress() const
{
    Q_D(const NetworkManager::WiredDevice);
    return d->hardwareAddress;
}

QString NetworkManager::WiredDevice::permanentHardwareAddress() const
{
    Q_D(const NetworkManager::WiredDevice);
    return d->permanentHardwareAddress;
}

int NetworkManager::WiredDevice::bitRate() const
{
    Q_D(const NetworkManager::WiredDevice);
    return d->bitrate;
}

bool NetworkManager::WiredDevice::carrier() const
{
    Q_D(const NetworkManager::WiredDevice);
    return d->carrier;
}

void NetworkManager::WiredDevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(NetworkManager::WiredDevice);

    if (property == QLatin1String("Carrier")) {
        carrier = value.toBool();
        Q_EMIT q->carrierChanged(carrier);
    } else if (property == QLatin1String("HwAddress")) {
        hardwareAddress = value.toString();
        Q_EMIT q->hardwareAddressChanged(hardwareAddress);
    } else if (property == QLatin1String("PermHwAddress")) {
        permanentHardwareAddress = value.toString();
        Q_EMIT q->permanentHardwareAddressChanged(permanentHardwareAddress);
    } else if (property == QLatin1String("Speed")) {
        bitrate = value.toUInt() * 1000;
        Q_EMIT q->bitRateChanged(bitrate);
    } else {
        DevicePrivate::propertyChanged(property, value);
    }
}
