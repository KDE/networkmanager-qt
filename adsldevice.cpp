/*
    Copyright 2012-2013  Jan Grulich <jgrulich@redhat.com>
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

#include "adsldevice.h"
#include "device_p.h"
#include "manager.h"
#include "manager_p.h"

#include "nm-device-adslinterface.h"

namespace NetworkManager
{
class AdslDevicePrivate : public DevicePrivate
{
public:
    AdslDevicePrivate(const QString &path, AdslDevice *q);
    virtual ~AdslDevicePrivate();

    OrgFreedesktopNetworkManagerDeviceAdslInterface iface;
    bool carrier;
};
}

NetworkManager::AdslDevicePrivate::AdslDevicePrivate(const QString &path, AdslDevice *q)
    : DevicePrivate(path, q)
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
    , carrier(false)
{
}

NetworkManager::AdslDevice::~AdslDevice()
{
}

NetworkManager::AdslDevice::AdslDevice(const QString &path, QObject *parent)
    : Device(*new AdslDevicePrivate(path, this), parent)
{
    Q_D(AdslDevice);

    d->carrier = d->iface.carrier();

    connect(&d->iface, SIGNAL(PropertiesChanged(QVariantMap)),
            this, SLOT(propertiesChanged(QVariantMap)));
}

NetworkManager::AdslDevicePrivate::~AdslDevicePrivate()
{
}

NetworkManager::Device::Type NetworkManager::AdslDevice::type() const
{
    return NetworkManager::Device::Adsl;
}

bool NetworkManager::AdslDevice::carrier() const
{
    Q_D(const AdslDevice);

    return d->carrier;
}

void NetworkManager::AdslDevice::propertyChanged(const QString &property, const QVariant &value)
{
    Q_D(AdslDevice);

    if (property == QLatin1String("Carrier")) {
        d->carrier = value.toBool();
        emit carrierChanged(d->carrier);
    } else {
        Device::propertyChanged(property, value);
    }
}

#include "adsldevice.moc"
