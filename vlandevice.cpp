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

#include "vlandevice.h"
#include "device_p.h"
#include "manager.h"
#include "manager_p.h"

#include "nm-device-vlaninterface.h"

namespace NetworkManager
{
class VlanDevicePrivate : public DevicePrivate
{
public:
    VlanDevicePrivate(const QString &path);
    virtual ~VlanDevicePrivate();

    OrgFreedesktopNetworkManagerDeviceVlanInterface iface;
    bool carrier;
    QString hwAddress;
    uint vlanId;
};
}

NetworkManager::VlanDevicePrivate::VlanDevicePrivate(const QString &path):
    DevicePrivate(path), iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus()),
    carrier(false)
{
}

NetworkManager::VlanDevice::~VlanDevice()
{
}

NetworkManager::VlanDevice::VlanDevice(const QString &path, QObject *parent):
    Device(*new VlanDevicePrivate(path), parent)
{
    Q_D(VlanDevice);

    d->carrier = d->iface.carrier();
    d->hwAddress = d->iface.hwAddress();
    d->vlanId = d->iface.vlanId();

    connect(&d->iface, SIGNAL(PropertiesChanged(QVariantMap)),
            this, SLOT(propertiesChanged(QVariantMap)));
}

NetworkManager::VlanDevicePrivate::~VlanDevicePrivate()
{
}

NetworkManager::Device::Type NetworkManager::VlanDevice::type() const
{
    return NetworkManager::Device::Vlan;
}

bool NetworkManager::VlanDevice::carrier() const
{
    Q_D(const VlanDevice);

    return d->carrier;
}

QString NetworkManager::VlanDevice::hwAddress() const
{
    Q_D(const VlanDevice);

    return d->hwAddress;
}

uint NetworkManager::VlanDevice::vlanId() const
{
    Q_D(const VlanDevice);

    return d->vlanId;
}

void NetworkManager::VlanDevice::propertyChanged(const QString &property, const QVariant &value)
{
    Q_D(VlanDevice);

    if (property == QLatin1String("Carrier")) {
        d->carrier = value.toBool();
        emit carrierChanged(d->carrier);
    } else if (property == QLatin1String("HwAddress")) {
        d->hwAddress = value.toString();
        emit hwAddressChanged(d->hwAddress);
    } else if (property == QLatin1String("VlanId")) {
        d->vlanId = value.toUInt();
        emit vlanIdChanged(d->vlanId);
    } else {
        Device::propertyChanged(property, value);
    }
}

#include "vlandevice.moc"
