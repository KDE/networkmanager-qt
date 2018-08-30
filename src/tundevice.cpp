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

#include "tundevice_p.h"
#include "manager_p.h"

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

    QVariantMap initialProperties = NetworkManagerPrivate::retrieveInitialProperties(d->iface.staticInterfaceName(), path);
    if (!initialProperties.isEmpty()) {
        d->propertiesChanged(initialProperties);
    }

    QDBusConnection::systemBus().connect(NetworkManagerPrivate::DBUS_SERVICE, d->uni, NetworkManagerPrivate::FDO_DBUS_PROPERTIES,
                                         QLatin1String("PropertiesChanged"), d, SLOT(dbusPropertiesChanged(QString,QVariantMap,QStringList)));
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
    return d->owner;
}

qlonglong NetworkManager::TunDevice::group() const
{
    Q_D(const TunDevice);
    return d->group;
}

QString NetworkManager::TunDevice::mode() const
{
    Q_D(const TunDevice);
    return d->mode;
}

bool NetworkManager::TunDevice::multiQueue() const
{
    Q_D(const TunDevice);
    return d->multiQueue;
}

bool NetworkManager::TunDevice::noPi() const
{
    Q_D(const TunDevice);
    return d->noPi;
}

bool NetworkManager::TunDevice::vnetHdr() const
{
    Q_D(const TunDevice);
    return d->vnetHdr;
}

QString NetworkManager::TunDevice::hwAddress() const
{
    Q_D(const TunDevice);
    return d->hwAddress;
}

void NetworkManager::TunDevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(TunDevice);

    if (property == QLatin1String("Owner")) {
        owner = value.toLongLong();
        Q_EMIT q->ownerChanged(owner);
    } else if (property == QLatin1String("Group")) {
        group = value.toLongLong();
        Q_EMIT q->groupChanged(group);
    } else if (property == QLatin1String("Mode")) {
        mode = value.toString();
        Q_EMIT q->modeChanged(mode);
    } else if (property == QLatin1String("MultiQueue")) {
        multiQueue = value.toBool();
        Q_EMIT q->multiQueueChanged(multiQueue);
    } else if (property == QLatin1String("NoPi")) {
        noPi = value.toBool();
        Q_EMIT q->noPiChanged(noPi);
    } else if (property == QLatin1String("VnetHdr")) {
        vnetHdr = value.toBool();
        Q_EMIT q->vnetHdrChanged(vnetHdr);
    } else if (property == QLatin1String("HwAddress")) {
        hwAddress = value.toString();
        Q_EMIT q->hwAddressChanged(hwAddress);
    } else {
        DevicePrivate::propertyChanged(property, value);
    }
}
