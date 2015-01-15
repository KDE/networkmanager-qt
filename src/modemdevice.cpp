/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2011-2013 Lamarque Souza <lamarque@kde.org>
    Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>
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

#include "modemdevice.h"
#include "modemdevice_p.h"

#include "manager_p.h"

NetworkManager::ModemDevice::Capabilities convertModemCapabilities(uint theirCaps)
{
    NetworkManager::ModemDevice::Capabilities ourCaps
        = (NetworkManager::ModemDevice::Capabilities) theirCaps;
    return ourCaps;
}

NetworkManager::ModemDevicePrivate::ModemDevicePrivate(const QString &path, ModemDevice *q)
    : DevicePrivate(path, q)
#ifdef NMQT_STATIC
    , modemIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , modemIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
{
}

void NetworkManager::ModemDevicePrivate::initModemProperties()
{
    modemCapabilities = convertModemCapabilities(modemIface.modemCapabilities());
    currentCapabilities = convertModemCapabilities(modemIface.currentCapabilities());
}

NetworkManager::ModemDevice::ModemDevice(const QString &path, QObject *parent)
    : Device(*new ModemDevicePrivate(path, this), parent)
{
    Q_D(ModemDevice);
    d->initModemProperties();
    QObject::connect(&d->modemIface, &OrgFreedesktopNetworkManagerDeviceModemInterface::PropertiesChanged, d, &ModemDevicePrivate::propertiesChanged);
}

NetworkManager::ModemDevice::ModemDevice(NetworkManager::ModemDevicePrivate &dd, QObject *parent)
    : Device(dd, parent)
{
    Q_D(ModemDevice);
    d->initModemProperties();
    QObject::connect(&d->modemIface, &OrgFreedesktopNetworkManagerDeviceModemInterface::PropertiesChanged, d, &ModemDevicePrivate::propertiesChanged);
}

NetworkManager::ModemDevice::~ModemDevice()
{
}

NetworkManager::Device::Type NetworkManager::ModemDevice::type() const
{
    return NetworkManager::Device::Modem;
}

NetworkManager::ModemDevice::Capabilities NetworkManager::ModemDevice::currentCapabilities() const
{
    Q_D(const ModemDevice);
    return d->currentCapabilities;
}

NetworkManager::ModemDevice::Capabilities NetworkManager::ModemDevice::modemCapabilities() const
{
    Q_D(const ModemDevice);
    return d->modemCapabilities;
}

void NetworkManager::ModemDevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(ModemDevice);

    if (property == QLatin1String("CurrentCapabilities")) {
        currentCapabilities = convertModemCapabilities(value.toUInt());
        Q_EMIT q->currentCapabilitiesChanged(currentCapabilities);
    } else {
        DevicePrivate::propertyChanged(property, value);
    }
}
