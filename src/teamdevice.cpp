/*
    Copyright 2013 Lukáš Tinkl <ltinkl@redhat.com>

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

#include "teamdevice.h"
#include "device_p.h"
#include "manager.h"
#include "manager_p.h"

#include "nm-device-teaminterface.h"

namespace NetworkManager
{
class TeamDevicePrivate : public DevicePrivate
{
public:
    TeamDevicePrivate(const QString &path, TeamDevice *q);
    virtual ~TeamDevicePrivate();

    OrgFreedesktopNetworkManagerDeviceTeamInterface iface;
};
}

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
    connect(&d->iface, &OrgFreedesktopNetworkManagerDeviceTeamInterface::PropertiesChanged, this, &TeamDevice::propertiesChanged);
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

    return d->iface.carrier();
}

QString NetworkManager::TeamDevice::hwAddress() const
{
    Q_D(const TeamDevice);

    return d->iface.hwAddress();
}

QStringList NetworkManager::TeamDevice::slaves() const
{
    Q_D(const TeamDevice);
    QStringList result;
    foreach (const QDBusObjectPath &op, d->iface.slaves()) {
        result << op.path();
    }

    return result;
}

void NetworkManager::TeamDevice::propertyChanged(const QString &property, const QVariant &value)
{
    if (property == QLatin1String("Carrier")) {
        emit carrierChanged(value.toBool());
    } else if (property == QLatin1String("HwAddress")) {
        emit hwAddressChanged(value.toString());
    } else if (property == QLatin1String("Slaves")) {
        emit slavesChanged();
    } else {
        Device::propertyChanged(property, value);
    }
}

#include "teamdevice.moc"
