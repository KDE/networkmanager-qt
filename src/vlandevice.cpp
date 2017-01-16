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

#include "vlandevice_p.h"
#include "device_p.h"
#include "manager.h"

NetworkManager::VlanDevicePrivate::VlanDevicePrivate(const QString &path, VlanDevice *q)
    : DevicePrivate(path, q)
#ifdef NMQT_STATIC
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , carrier(false)
{
}

NetworkManager::VlanDevice::~VlanDevice()
{
}

NetworkManager::VlanDevice::VlanDevice(const QString &path, QObject *parent)
    : Device(*new VlanDevicePrivate(path, this), parent)
{
    Q_D(VlanDevice);

    QVariantMap initialProperties = NetworkManagerPrivate::retrieveInitialProperties(d->iface.staticInterfaceName(), path);
    if (!initialProperties.isEmpty()) {
        d->propertiesChanged(initialProperties);
    }

#if NM_CHECK_VERSION(1, 4, 0)
    QDBusConnection::systemBus().connect(NetworkManagerPrivate::DBUS_SERVICE, d->uni, NetworkManagerPrivate::FDO_DBUS_PROPERTIES,
                                         QLatin1String("PropertiesChanged"), d, SLOT(dbusPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    connect(&d->iface, &OrgFreedesktopNetworkManagerDeviceVlanInterface::PropertiesChanged, d, &VlanDevicePrivate::propertiesChanged);
#endif
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

NetworkManager::Device::Ptr NetworkManager::VlanDevice::parent() const
{
    if (NetworkManager::checkVersion(1, 0, 0)) {
        Q_D(const VlanDevice);

        return NetworkManager::findNetworkInterface(d->parent);
    } else {
        return NetworkManager::Device::Ptr(nullptr);
    }
}

uint NetworkManager::VlanDevice::vlanId() const
{
    Q_D(const VlanDevice);

    return d->vlanId;
}

void NetworkManager::VlanDevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(VlanDevice);

    if (property == QLatin1String("Carrier")) {
        carrier = value.toBool();
        Q_EMIT q->carrierChanged(carrier);
    } else if (property == QLatin1String("HwAddress")) {
        hwAddress = value.toString();
        Q_EMIT q->hwAddressChanged(hwAddress);
    } else if (property == QLatin1String("Parent")) {
        parent = value.value<QDBusObjectPath>().path();
        Q_EMIT q->parentChanged(parent);
    } else if (property == QLatin1String("VlanId")) {
        vlanId = value.toUInt();
        Q_EMIT q->vlanIdChanged(vlanId);
    } else {
        DevicePrivate::propertyChanged(property, value);
    }
}
