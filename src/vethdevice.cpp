/*
    Copyright 2013 Lukáš Tinkl <ltinkl@redhat.com>
    Copyright 2014 Jan Grulich <jgrulich@redhat.com>

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

#include "vethdevice_p.h"
#include "device.h"
#include "manager.h"
#include "manager_p.h"

NetworkManager::VethDevicePrivate::VethDevicePrivate(const QString &path, VethDevice *q)
    : DevicePrivate(path, q)
#ifdef NMQT_STATIC
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
{
    peer = iface.peer().path();
}

NetworkManager::VethDevicePrivate::~VethDevicePrivate()
{
}

NetworkManager::VethDevice::VethDevice(const QString &path, QObject *parent)
    : Device(*new VethDevicePrivate(path, this), parent)
{
    Q_D(VethDevice);
    connect(&d->iface, &OrgFreedesktopNetworkManagerDeviceVethInterface::PropertiesChanged, d, &VethDevicePrivate::propertiesChanged);
}

NetworkManager::VethDevice::~VethDevice()
{
}

NetworkManager::Device::Type NetworkManager::VethDevice::type() const
{
    return NetworkManager::Device::Veth;
}

QString NetworkManager::VethDevice::peer() const
{
    Q_D(const VethDevice);
    return d->peer;
}

void NetworkManager::VethDevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(VethDevice);

    if (property == QLatin1String("Peer")) {
        peer = value.toString();
        emit q->peerChanged(peer);
    } else {
        DevicePrivate::propertyChanged(property, value);
    }
}
