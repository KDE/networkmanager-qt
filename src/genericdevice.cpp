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

#include "genericdevice.h"
#include "device_p.h"
#include "manager.h"
#include "manager_p.h"
#include "nmdebug.h"

#include "nm-device-genericinterface.h"

namespace NetworkManager
{
class GenericDevicePrivate : public DevicePrivate
{
public:
    GenericDevicePrivate(const QString &path, GenericDevice *q);
    virtual ~GenericDevicePrivate();

    OrgFreedesktopNetworkManagerDeviceGenericInterface iface;
    QString hwAddress;
    QString typeDescription;
};
}

NetworkManager::GenericDevicePrivate::GenericDevicePrivate(const QString &path, GenericDevice *q)
    : DevicePrivate(path, q)
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
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
    connect(&d->iface, &OrgFreedesktopNetworkManagerDeviceGenericInterface::PropertiesChanged, this, &GenericDevice::propertiesChanged);
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
    Q_D(const NetworkManager::GenericDevice);
    return d->hwAddress;
}

QString NetworkManager::GenericDevice::typeDescription() const
{
    Q_D(const NetworkManager::GenericDevice);
    return d->typeDescription;
}

void NetworkManager::GenericDevice::propertyChanged(const QString &property, const QVariant &value)
{
    Q_D(NetworkManager::GenericDevice);

    if (property == QLatin1String("HwAddress")) {
        d->hwAddress = value.toString();
        emit hardwareAddressChanged(d->hwAddress);
    } else if (property == QLatin1String("TypeDescription")) {
        d->typeDescription = value.toString();
        emit permanentHardwareAddressChanged(d->typeDescription);
    } else {
        Device::propertyChanged(property, value);
    }
}

#include "genericdevice.moc"
