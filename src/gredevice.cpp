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

#include "gredevice.h"
#include "device_p.h"
#include "manager.h"
#include "manager_p.h"

#include "nm-device-greinterface.h"

namespace NetworkManager
{
class GreDevicePrivate : public DevicePrivate
{
public:
    GreDevicePrivate(const QString &path, GreDevice *q);
    virtual ~GreDevicePrivate();

    OrgFreedesktopNetworkManagerDeviceGreInterface iface;
};
}

NetworkManager::GreDevicePrivate::GreDevicePrivate(const QString &path, GreDevice *q)
    : DevicePrivate(path, q)
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

NetworkManager::GreDevicePrivate::~GreDevicePrivate()
{
}

NetworkManager::GreDevice::GreDevice(const QString &path, QObject *parent):
    Device(*new GreDevicePrivate(path, this), parent)
{
    Q_D(GreDevice);
    QObject::connect(&d->iface, &OrgFreedesktopNetworkManagerDeviceGreInterface::PropertiesChanged, this, &GreDevice::propertiesChanged);
}

NetworkManager::GreDevice::~GreDevice()
{
}

NetworkManager::Device::Type NetworkManager::GreDevice::type() const
{
    return NetworkManager::Device::Gre;
}

ushort NetworkManager::GreDevice::inputFlags() const
{
    Q_D(const GreDevice);
    return d->iface.inputFlags();
}

ushort NetworkManager::GreDevice::outputFlags() const
{
    Q_D(const GreDevice);
    return d->iface.outputFlags();
}

uint NetworkManager::GreDevice::inputKey() const
{
    Q_D(const GreDevice);
    return d->iface.inputKey();
}

uint NetworkManager::GreDevice::outputKey() const
{
    Q_D(const GreDevice);
    return d->iface.outputKey();
}

QString NetworkManager::GreDevice::localEnd() const
{
    Q_D(const GreDevice);
    return d->iface.local();
}

QString NetworkManager::GreDevice::remoteEnd() const
{
    Q_D(const GreDevice);
    return d->iface.remote();
}

QString NetworkManager::GreDevice::parent() const
{
    Q_D(const GreDevice);
    return d->iface.parent().path();
}

bool NetworkManager::GreDevice::pathMtuDiscovery() const
{
    Q_D(const GreDevice);
    return d->iface.pathMtuDiscovery();
}

uchar NetworkManager::GreDevice::tos() const
{
    Q_D(const GreDevice);
    return d->iface.tos();
}

uchar NetworkManager::GreDevice::ttl() const
{
    Q_D(const GreDevice);
    return d->iface.ttl();
}

void NetworkManager::GreDevice::propertyChanged(const QString &property, const QVariant &value)
{
    if (property == QLatin1String("InputFlags")) {
        emit inputFlagsChanged();
    } else if (property == QLatin1String("OutputFlags")) {
        emit outputFlagsChanged();
    } else if (property == QLatin1String("InputKey")) {
        emit inputKeyChanged();
    } else if (property == QLatin1String("OutputKey")) {
        emit outputKeyChanged();
    } else if (property == QLatin1String("Local")) {
        emit localEndChanged();
    } else if (property == QLatin1String("Remote")) {
        emit remoteEndChanged();
    } else if (property == QLatin1String("Parent")) {
        emit parentChanged();
    } else if (property == QLatin1String("Parent")) {
        emit parentChanged();
    } else if (property == QLatin1String("PathMtuDiscovery")) {
        emit pathMtuDiscoveryChanged();
    } else if (property == QLatin1String("Tos")) {
        emit tosChanged();
    } else if (property == QLatin1String("Ttl")) {
        emit ttlChanged();
    } else {
        Device::propertyChanged(property, value);
    }
}

#include "gredevice.moc"
