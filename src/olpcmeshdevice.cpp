/*
    Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
    Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>

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

#include "olpcmeshdevice.h"
#include "olpcmeshdevice_p.h"
#include "manager_p.h"

#include "wimaxnsp.h"

NetworkManager::OlpcMeshDevicePrivate::OlpcMeshDevicePrivate(const QString &path, OlpcMeshDevice *q)
    : DevicePrivate(path, q)
#ifdef NMQT_STATIC
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
{
    hardwareAddress = iface.hwAddress();
    activeChannel = iface.activeChannel();
    companion = iface.companion().path();
}

NetworkManager::OlpcMeshDevice::OlpcMeshDevice(const QString &path, QObject *parent)
    : Device(*new OlpcMeshDevicePrivate(path, this), parent)
{
    Q_D(OlpcMeshDevice);
    QObject::connect(&d->iface, &OrgFreedesktopNetworkManagerDeviceOlpcMeshInterface::PropertiesChanged, d, &OlpcMeshDevicePrivate::propertiesChanged);
}

NetworkManager::OlpcMeshDevice::~OlpcMeshDevice()
{
}

NetworkManager::Device::Type NetworkManager::OlpcMeshDevice::type() const
{
    return NetworkManager::Device::OlpcMesh;
}

QString NetworkManager::OlpcMeshDevice::hardwareAddress() const
{
    Q_D(const OlpcMeshDevice);
    return d->hardwareAddress;
}

uint NetworkManager::OlpcMeshDevice::activeChannel() const
{
    Q_D(const OlpcMeshDevice);
    return d->activeChannel;
}

NetworkManager::Device::Ptr NetworkManager::OlpcMeshDevice::companionDevice() const
{
    Q_D(const OlpcMeshDevice);
    return NetworkManager::findNetworkInterface(d->companion);
}

void NetworkManager::OlpcMeshDevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(OlpcMeshDevice);

    if (property == QLatin1String("ActiveChannel")) {
        activeChannel = value.toUInt();
        Q_EMIT q->activeChannelChanged(activeChannel);
    } else if (property == QLatin1String("HwAddress")) {
        hardwareAddress = value.toString();
        Q_EMIT q->hardwareAddressChanged(hardwareAddress);
    } else if (property == QLatin1String("Companion")) {
        companion = qdbus_cast<QDBusObjectPath>(value).path();
        Q_EMIT q->companionChanged(NetworkManager::findNetworkInterface(companion));
    } else {
        DevicePrivate::propertyChanged(property, value);
    }
}
