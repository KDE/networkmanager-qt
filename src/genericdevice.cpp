/*
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

#include "genericdevice_p.h"
#include "manager.h"
#include "nmdebug.h"

NetworkManager::GenericDevicePrivate::GenericDevicePrivate(const QString &path, GenericDevice *q)
    : DevicePrivate(path, q)
#ifdef NMQT_STATIC
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
{
    hwAddress = iface.hwAddress();
    typeDescription = iface.typeDescription();
}

NetworkManager::GenericDevicePrivate::~GenericDevicePrivate()
{
}

NetworkManager::GenericDevice::GenericDevice(const QString &path, QObject *parent)
    : Device(*new NetworkManager::GenericDevicePrivate(path, this), parent)
{
    Q_D(GenericDevice);
    connect(&d->iface, &OrgFreedesktopNetworkManagerDeviceGenericInterface::PropertiesChanged, d, &GenericDevicePrivate::propertiesChanged);
}

NetworkManager::GenericDevice::~GenericDevice()
{
}

NetworkManager::Device::Type NetworkManager::GenericDevice::type() const
{
    return NetworkManager::Device::Generic;
}

QString NetworkManager::GenericDevice::hardwareAddress() const
{
    Q_D(const GenericDevice);
    return d->hwAddress;
}

QString NetworkManager::GenericDevice::typeDescription() const
{
    Q_D(const GenericDevice);
    return d->typeDescription;
}

void NetworkManager::GenericDevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(GenericDevice);

    if (property == QLatin1String("HwAddress")) {
        hwAddress = value.toString();
        Q_EMIT q->hardwareAddressChanged(hwAddress);
    } else if (property == QLatin1String("TypeDescription")) {
        typeDescription = value.toString();
        Q_EMIT q->permanentHardwareAddressChanged(typeDescription);
    } else {
        DevicePrivate::propertyChanged(property, value);
    }
}
