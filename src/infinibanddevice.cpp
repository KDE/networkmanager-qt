/*
    Copyright 2013 Jan Grulich <jgrulich@redhat.com>
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

#include "infinibanddevice_p.h"
#include "device_p.h"
#include "manager.h"

NetworkManager::InfinibandDevicePrivate::InfinibandDevicePrivate(const QString &path, InfinibandDevice *q)
    : DevicePrivate(path, q)
#ifdef NMQT_STATIC
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , carrier(false)
{
    carrier = iface.carrier();
    hwAddress = iface.hwAddress();
}

NetworkManager::InfinibandDevice::~InfinibandDevice()
{
}

NetworkManager::InfinibandDevice::InfinibandDevice(const QString &path, QObject *parent)
    : Device(*new InfinibandDevicePrivate(path, this), parent)
{
    Q_D(InfinibandDevice);
#if NM_CHECK_VERSION(1, 4, 0)
    QDBusConnection::systemBus().connect(NetworkManagerPrivate::DBUS_SERVICE, d->uni, NetworkManagerPrivate::FDO_DBUS_PROPERTIES,
                                         QLatin1String("PropertiesChanged"), d, SLOT(dbusPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    connect(&d->iface, &OrgFreedesktopNetworkManagerDeviceInfinibandInterface::PropertiesChanged, d, &InfinibandDevicePrivate::propertiesChanged);
#endif
}

NetworkManager::InfinibandDevicePrivate::~InfinibandDevicePrivate()
{
}

NetworkManager::Device::Type NetworkManager::InfinibandDevice::type() const
{
    return NetworkManager::Device::InfiniBand;
}

bool NetworkManager::InfinibandDevice::carrier() const
{
    Q_D(const InfinibandDevice);

    return d->carrier;
}

QString NetworkManager::InfinibandDevice::hwAddress() const
{
    Q_D(const InfinibandDevice);

    return d->hwAddress;
}

void NetworkManager::InfinibandDevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(InfinibandDevice);

    if (property == QLatin1String("Carrier")) {
        carrier = value.toBool();
        Q_EMIT q->carrierChanged(carrier);
    } else if (property == QLatin1String("HwAddress")) {
        hwAddress = value.toString();
        Q_EMIT q->hwAddressChanged(hwAddress);
    } else {
        DevicePrivate::propertyChanged(property, value);
    }
}
