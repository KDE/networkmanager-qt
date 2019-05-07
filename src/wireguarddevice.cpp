/*
    Copyright 2019 Jan Grulich <jgrulich@redhat.com>

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

#include "wireguarddevice.h"
#include "wireguarddevice_p.h"
#include "manager_p.h"

NetworkManager::WireGuardDevicePrivate::WireGuardDevicePrivate(const QString &path, WireGuardDevice *q)
    : DevicePrivate(path, q)
#ifdef NMQT_STATIC
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , listenPort(0)
    , fwMark(0)
{
}

NetworkManager::WireGuardDevicePrivate::~WireGuardDevicePrivate()
{
}

NetworkManager::WireGuardDevice::WireGuardDevice(const QString &path, QObject *parent):
    Device(*new WireGuardDevicePrivate(path, this), parent)
{
    Q_D(WireGuardDevice);

    QVariantMap initialProperties = NetworkManagerPrivate::retrieveInitialProperties(d->iface.staticInterfaceName(), path);
    if (!initialProperties.isEmpty()) {
        d->propertiesChanged(initialProperties);
    }

    QDBusConnection::systemBus().connect(NetworkManagerPrivate::DBUS_SERVICE, d->uni, NetworkManagerPrivate::FDO_DBUS_PROPERTIES,
                                         QLatin1String("PropertiesChanged"), d, SLOT(dbusPropertiesChanged(QString,QVariantMap,QStringList)));
}

NetworkManager::WireGuardDevice::~WireGuardDevice()
{
}

NetworkManager::Device::Type NetworkManager::WireGuardDevice::type() const
{
    return NetworkManager::Device::WireGuard;
}

QByteArray NetworkManager::WireGuardDevice::publicKey() const
{
    Q_D(const WireGuardDevice);

    return d->publicKey;
}

uint NetworkManager::WireGuardDevice::listenPort() const
{
    Q_D(const WireGuardDevice);

    return d->listenPort;
}

uint NetworkManager::WireGuardDevice::fwMark() const
{
    Q_D(const WireGuardDevice);

    return d->fwMark;
}

void NetworkManager::WireGuardDevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(WireGuardDevice);

    if (property == QLatin1String("PublicKey")) {
        publicKey = value.toByteArray();
        Q_EMIT q->publicKeyChanged(publicKey);
    } else if (property == QLatin1String("ListenPort")) {
        listenPort = value.toUInt();
        Q_EMIT q->listenPortChanged(listenPort);
    } else if (property == QLatin1String("FwMark")) {
        fwMark = value.toUInt();
        Q_EMIT q->fwMarkChanged(fwMark);
    } else {
        DevicePrivate::propertyChanged(property, value);
    }
}
