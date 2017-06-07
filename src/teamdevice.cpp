/*
    Copyright 2013 Lukáš Tinkl <ltinkl@redhat.com>
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

#include "teamdevice_p.h"
#include "device_p.h"
#include "manager.h"
#include "manager_p.h"

NetworkManager::TeamDevicePrivate::TeamDevicePrivate(const QString &path, TeamDevice *q)
    : DevicePrivate(path, q)
#ifdef NMQT_STATIC
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
{
}

NetworkManager::TeamDevicePrivate::~TeamDevicePrivate()
{
}

NetworkManager::TeamDevice::TeamDevice(const QString &path, QObject *parent)
    : Device(*new TeamDevicePrivate(path, this), parent)
{
    Q_D(TeamDevice);

    QVariantMap initialProperties = NetworkManagerPrivate::retrieveInitialProperties(d->iface.staticInterfaceName(), path);
    if (!initialProperties.isEmpty()) {
        d->propertiesChanged(initialProperties);
    }

#if NM_CHECK_VERSION(1, 4, 0)
    QDBusConnection::systemBus().connect(NetworkManagerPrivate::DBUS_SERVICE, d->uni, NetworkManagerPrivate::FDO_DBUS_PROPERTIES,
                                         QLatin1String("PropertiesChanged"), d, SLOT(dbusPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    connect(&d->iface, &OrgFreedesktopNetworkManagerDeviceTeamInterface::PropertiesChanged, d, &TeamDevicePrivate::propertiesChanged);
#endif
}

NetworkManager::TeamDevice::~TeamDevice()
{
}

NetworkManager::Device::Type NetworkManager::TeamDevice::type() const
{
    return NetworkManager::Device::Team;
}

bool NetworkManager::TeamDevice::carrier() const
{
    Q_D(const TeamDevice);

    return d->carrier;
}

QString NetworkManager::TeamDevice::hwAddress() const
{
    Q_D(const TeamDevice);

    return d->hwAddress;
}

QStringList NetworkManager::TeamDevice::slaves() const
{
    Q_D(const TeamDevice);
    return d->slaves;
}

QString NetworkManager::TeamDevice::config() const
{
    Q_D(const TeamDevice);
    return d->config;
}

void NetworkManager::TeamDevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(TeamDevice);

    if (property == QLatin1String("Carrier")) {
        carrier = value.toBool();
        Q_EMIT q->carrierChanged(carrier);
    } else if (property == QLatin1String("HwAddress")) {
        hwAddress = value.toString();
        Q_EMIT q->hwAddressChanged(hwAddress);
    } else if (property == QLatin1String("Slaves")) {
        QStringList list;
        Q_FOREACH (const QDBusObjectPath & op, qdbus_cast< QList<QDBusObjectPath> >(value)) {
            list << op.path();
        }
        slaves = list;
        Q_EMIT q->slavesChanged(slaves);
    } else if (property == QLatin1String("Config")) {
        config = value.toString();
        Q_EMIT q->configChanged(config);
    } else {
        DevicePrivate::propertyChanged(property, value);
    }
}
