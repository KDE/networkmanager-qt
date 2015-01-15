/*
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

#include "macvlandevice_p.h"
#include "device_p.h"
#include "manager.h"

NetworkManager::MacVlanDevicePrivate::MacVlanDevicePrivate(const QString &path, MacVlanDevice *q)
    : DevicePrivate(path, q)
#ifdef NMQT_STATIC
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
{
    mode = iface.mode();
    noPromisc = iface.noPromisc();
    parent = iface.parent().path();
}

NetworkManager::MacVlanDevicePrivate::~MacVlanDevicePrivate()
{
}

NetworkManager::MacVlanDevice::MacVlanDevice(const QString &path, QObject *parent):
    Device(*new MacVlanDevicePrivate(path, this), parent)
{
    Q_D(MacVlanDevice);
    connect(&d->iface, &OrgFreedesktopNetworkManagerDeviceMacvlanInterface::PropertiesChanged, d, &MacVlanDevicePrivate::propertiesChanged);
}

NetworkManager::MacVlanDevice::~MacVlanDevice()
{
}

NetworkManager::Device::Type NetworkManager::MacVlanDevice::type() const
{
    return NetworkManager::Device::MacVlan;
}

QString NetworkManager::MacVlanDevice::mode() const
{
    Q_D(const MacVlanDevice);
    return d->mode;
}

bool NetworkManager::MacVlanDevice::noPromisc() const
{
    Q_D(const MacVlanDevice);
    return d->noPromisc;
}

QString NetworkManager::MacVlanDevice::parent() const
{
    Q_D(const MacVlanDevice);
    return d->parent;
}

void NetworkManager::MacVlanDevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(MacVlanDevice);

    if (property == QLatin1String("Mode")) {
        mode = value.toString();
        Q_EMIT q->modeChanged(mode);
    } else if (property == QLatin1String("NoPromisc")) {
        noPromisc = value.toBool();
        Q_EMIT q->noPromiscChanged(noPromisc);
    } else if (property == QLatin1String("Parent")) {
        parent = value.toString();
        Q_EMIT q->parentChanged(parent);
    } else {
        DevicePrivate::propertyChanged(property, value);
    }
}
