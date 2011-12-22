/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>

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

NetworkManager::WiredDevicePrivate::WiredDevicePrivate(const QString & path, QObject * owner)
    : DevicePrivate(path, owner), wiredIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus()),
    bitrate(0), carrier(false)
{
}

NetworkManager::WiredDevicePrivate::~WiredDevicePrivate()
{
}

NetworkManager::WiredDevice::WiredDevice(const QString & path, QObject * parent)
    : Device(*new NetworkManager::WiredDevicePrivate(path, this), parent)
{
    Q_D(WiredDevice);
    d->hardwareAddress = d->wiredIface.hwAddress();
    d->permanentHardwareAddress= d->wiredIface.permHwAddress();
    d->bitrate = d->wiredIface.speed() * 1000;
    d->carrier = d->wiredIface.carrier();
    //d->propHelper.registerProperty();
    connect( &d->wiredIface, SIGNAL(PropertiesChanged(QVariantMap)),
                this, SLOT(wiredPropertiesChanged(QVariantMap)));
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

void NetworkManager::WiredDevice::wiredPropertiesChanged(const QVariantMap &properties)
{
    Q_D(WiredDevice);
    QStringList propKeys = properties.keys();
    nmDebug() << properties.keys();
    QLatin1String carrierKey("Carrier"),
                  hwAddressKey("HwAddress"),
                  permHwAddressKey("PermHwAddress"),
                  speedKey("Speed");
    QVariantMap::const_iterator it = properties.find(carrierKey);
    if ( it != properties.end()) {
        d->carrier = it->toBool();
        emit carrierChanged(d->carrier);
        propKeys.removeOne(carrierKey);
    }
    it = properties.find(speedKey);
    if ( it != properties.end()) {
        d->bitrate = it->toUInt() * 1000;
        emit bitRateChanged(d->bitrate);
        propKeys.removeOne(speedKey);
    }
    it = properties.find(hwAddressKey);
    if ( it != properties.end()) {
        d->hardwareAddress = it->toString();
        emit hardwareAddressChanged(d->hardwareAddress);
        propKeys.removeOne(hwAddressKey);
    }
    it = properties.find(permHwAddressKey);
    if ( it != properties.end()) {
        d->permanentHardwareAddress = it->toString();
        emit permanentHardwareAddressChanged(d->permanentHardwareAddress);
        propKeys.removeOne(permHwAddressKey);
    }
    if (propKeys.count()) {
        nmDebug() << "Unhandled properties: ";
        foreach (const QString &key, propKeys) {
            nmDebug() << key << properties.value(key);
        }
    }
}

