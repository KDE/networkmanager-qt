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

#include "bridgedevice.h"
#include "device_p.h"
#include "manager.h"
#include "manager_p.h"

#include "nm-device-bridgeinterface.h"

namespace NetworkManager
{
class BridgeDevicePrivate : public DevicePrivate
{
public:
    BridgeDevicePrivate(const QString & path, QObject * owner);
    virtual ~BridgeDevicePrivate();

    OrgFreedesktopNetworkManagerDeviceBridgeInterface iface;
    bool carrier;
    QString hwAddress;
    QList<QDBusObjectPath> slaves;
};
}

NetworkManager::BridgeDevicePrivate::BridgeDevicePrivate(const QString& path, QObject* owner):
    DevicePrivate(path, owner), iface(NetworkManagerPrivate::DBUS_SERVICE, path,QDBusConnection::systemBus()),
    carrier(false), slaves(QList<QDBusObjectPath>())
{
}

NetworkManager::BridgeDevice::~BridgeDevice()
{
}

NetworkManager::BridgeDevice::BridgeDevice(const QString& path, QObject* parent):
    Device(*new BridgeDevicePrivate(path, this), parent)
{
    Q_D(BridgeDevice);

    d->carrier = d->iface.carrier();
    d->hwAddress = d->iface.hwAddress();
    d->slaves = d->iface.slaves();

    connect(&d->iface, SIGNAL(PropertiesChanged(QVariantMap)),
            this, SLOT(onPropertiesChanged(QVariantMap)));
}

NetworkManager::BridgeDevicePrivate::~BridgeDevicePrivate()
{
}

NetworkManager::Device::Type NetworkManager::BridgeDevice::type() const
{
    return NetworkManager::Device::Bridge;
}

bool NetworkManager::BridgeDevice::carrier() const
{
    Q_D(const BridgeDevice);

    return d->carrier;
}

QString NetworkManager::BridgeDevice::hwAddress() const
{
    Q_D(const BridgeDevice);

    return d->hwAddress;
}

QList< QDBusObjectPath > NetworkManager::BridgeDevice::slaves() const
{
    Q_D(const BridgeDevice);

    return d->slaves;
}

void NetworkManager::BridgeDevice::onPropertiesChanged(const QVariantMap& properties)
{
    Q_D(BridgeDevice);

    QVariantMap::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        QString property = it.key();
        if (property == QLatin1String("Carrier")) {
            d->carrier = it->toBool();
            emit carrierChanged(d->carrier);
        } else if (property == QLatin1String("HwAddress")) {
            d->hwAddress = it->toString();
            emit hwAddressChanged(d->hwAddress);
        } else if (property == QLatin1String("Slaves")) {
            d->slaves = it->value<QList<QDBusObjectPath> >();
            emit slavesChanged(d->slaves);
        } else {
            qWarning() << Q_FUNC_INFO << "Unhandled property" << property;
        }
        ++it;
    }
}

#include "bridgedevice.moc"
