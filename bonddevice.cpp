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

#include "bonddevice.h"
#include "device_p.h"
#include "manager.h"
#include "manager_p.h"

#include "nm-device-bondinterface.h"

namespace NetworkManager
{
class BondDevicePrivate : public DevicePrivate
{
public:
    BondDevicePrivate(const QString & path, QObject * owner);
    virtual ~BondDevicePrivate();

    OrgFreedesktopNetworkManagerDeviceBondInterface iface;
    bool carrier;
    QString hwAddress;
    QList<QDBusObjectPath> slaves;
};
}

NetworkManager::BondDevicePrivate::BondDevicePrivate(const QString& path, QObject* owner):
    DevicePrivate(path, owner), iface(NetworkManagerPrivate::DBUS_SERVICE, path,QDBusConnection::systemBus()),
    carrier(false), slaves(QList<QDBusObjectPath>())
{
}

NetworkManager::BondDevice::~BondDevice()
{
}

NetworkManager::BondDevice::BondDevice(const QString& path, QObject* parent):
    Device(*new BondDevicePrivate(path, this), parent)
{
    Q_D(BondDevice);

    d->carrier = d->iface.carrier();
    d->hwAddress = d->iface.hwAddress();
    d->slaves = d->iface.slaves();

    connect(&d->iface, SIGNAL(PropertiesChanged(QVariantMap)),
            this, SLOT(onPropertiesChanged(QVariantMap)));
}

NetworkManager::BondDevicePrivate::~BondDevicePrivate()
{
}

NetworkManager::Device::Type NetworkManager::BondDevice::type() const
{
    return NetworkManager::Device::Bond;
}

bool NetworkManager::BondDevice::carrier() const
{
    Q_D(const BondDevice);

    return d->carrier;
}

QString NetworkManager::BondDevice::hwAddress() const
{
    Q_D(const BondDevice);

    return d->hwAddress;
}

QList< QDBusObjectPath > NetworkManager::BondDevice::slaves() const
{
    Q_D(const BondDevice);

    return d->slaves;
}

void NetworkManager::BondDevice::onPropertiesChanged(const QVariantMap& properties)
{
    Q_D(BondDevice);

    if (properties.contains(QLatin1String("Carrier"))) {
        d->carrier = properties.value(QLatin1String("Carrier")).toBool();
        emit carrierChanged(d->carrier);
    }

    if (properties.contains(QLatin1String("HwAddress"))) {
        d->hwAddress = properties.value(QLatin1String("HwAddress")).toString();
        emit hwAddressChanged(d->hwAddress);
    }

    if (properties.contains(QLatin1String("Slaves"))) {
        d->slaves = properties.value(QLatin1String("Slaves")).value<QList<QDBusObjectPath> >();
        emit slavesChanged(d->slaves);
    }
}

#include "bonddevice.moc"
