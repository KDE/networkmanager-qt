/*
    Copyright 2013 Lukáš Tinkl <ltinkl@redhat.com>

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

#include "tundevice.h"
#include "device_p.h"
#include "manager.h"
#include "manager_p.h"

#include "nm-device-tuninterface.h"

namespace NetworkManager
{
class TunDevicePrivate: public DevicePrivate
{
public:
    TunDevicePrivate(const QString &path, TunDevice *q);
    virtual ~TunDevicePrivate();

    OrgFreedesktopNetworkManagerDeviceTunInterface iface;
};
}

NetworkManager::TunDevicePrivate::TunDevicePrivate(const QString &path, TunDevice *q)
    : DevicePrivate(path, q)
#ifdef NMQT_STATIC
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
{
}

NetworkManager::TunDevicePrivate::~TunDevicePrivate()
{
}

NetworkManager::TunDevice::TunDevice(const QString &path, QObject *parent)
    : Device(*new TunDevicePrivate(path, this), parent)
{
    Q_D(TunDevice);
    connect(&d->iface, &OrgFreedesktopNetworkManagerDeviceTunInterface::PropertiesChanged, this, &TunDevice::propertiesChanged);
}

NetworkManager::TunDevice::~TunDevice()
{
}

NetworkManager::Device::Type NetworkManager::TunDevice::type() const
{
    return NetworkManager::Device::Tun;
}

qlonglong NetworkManager::TunDevice::owner() const
{
    Q_D(const TunDevice);
    return d->iface.owner();
}

qlonglong NetworkManager::TunDevice::group() const
{
    Q_D(const TunDevice);
    return d->iface.group();
}

QString NetworkManager::TunDevice::mode() const
{
    Q_D(const TunDevice);
    return d->iface.mode();
}

bool NetworkManager::TunDevice::multiQueue() const
{
    Q_D(const TunDevice);
    return d->iface.multiQueue();
}

bool NetworkManager::TunDevice::noPi() const
{
    Q_D(const TunDevice);
    return d->iface.noPi();
}

bool NetworkManager::TunDevice::vnetHdr() const
{
    Q_D(const TunDevice);
    return d->iface.vnetHdr();
}

void NetworkManager::TunDevice::propertyChanged(const QString &property, const QVariant &value)
{
    if (property == QLatin1String("Owner")) {
        emit ownerChanged();
    } else if (property == QLatin1String("Group")) {
        emit groupChanged();
    } else if (property == QLatin1String("Mode")) {
        emit modeChanged();
    } else if (property == QLatin1String("MultiQueue")) {
        emit multiQueueChanged();
    } else if (property == QLatin1String("NoPi")) {
        emit noPiChanged();
    } else if (property == QLatin1String("VnetHdr")) {
        emit vnetHdrChanged();
    } else {
        Device::propertyChanged(property, value);
    }
}

#include "tundevice.moc"
