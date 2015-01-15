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

#include "gredevice_p.h"
#include "device_p.h"
#include "manager.h"

NetworkManager::GreDevicePrivate::GreDevicePrivate(const QString &path, GreDevice *q)
    : DevicePrivate(path, q)
#ifdef NMQT_STATIC
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
{
    inputFlags = iface.inputFlags();
    outputFlags = iface.outputFlags();
    inputKey = iface.inputKey();
    outputKey = iface.outputKey();
    localEnd = iface.local();
    remoteEnd = iface.remote();
    parent = iface.parent().path();
    pathMtuDiscovery = iface.pathMtuDiscovery();
    tos = iface.tos();
    ttl = iface.ttl();
}

NetworkManager::GreDevicePrivate::~GreDevicePrivate()
{
}

NetworkManager::GreDevice::GreDevice(const QString &path, QObject *parent):
    Device(*new GreDevicePrivate(path, this), parent)
{
    Q_D(GreDevice);
    QObject::connect(&d->iface, &OrgFreedesktopNetworkManagerDeviceGreInterface::PropertiesChanged, d, &GreDevicePrivate::propertiesChanged);
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
    return d->inputFlags;
}

ushort NetworkManager::GreDevice::outputFlags() const
{
    Q_D(const GreDevice);
    return d->outputFlags;
}

uint NetworkManager::GreDevice::inputKey() const
{
    Q_D(const GreDevice);
    return d->inputKey;
}

uint NetworkManager::GreDevice::outputKey() const
{
    Q_D(const GreDevice);
    return d->outputKey;
}

QString NetworkManager::GreDevice::localEnd() const
{
    Q_D(const GreDevice);
    return d->localEnd;
}

QString NetworkManager::GreDevice::remoteEnd() const
{
    Q_D(const GreDevice);
    return d->remoteEnd;
}

QString NetworkManager::GreDevice::parent() const
{
    Q_D(const GreDevice);
    return d->parent;
}

bool NetworkManager::GreDevice::pathMtuDiscovery() const
{
    Q_D(const GreDevice);
    return d->pathMtuDiscovery;
}

uchar NetworkManager::GreDevice::tos() const
{
    Q_D(const GreDevice);
    return d->tos;
}

uchar NetworkManager::GreDevice::ttl() const
{
    Q_D(const GreDevice);
    return d->ttl;
}

void NetworkManager::GreDevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(GreDevice);

    if (property == QLatin1String("InputFlags")) {
        inputFlags = static_cast<ushort>(value.toUInt());
        Q_EMIT q->inputFlagsChanged(inputFlags);
    } else if (property == QLatin1String("OutputFlags")) {
        outputFlags = static_cast<ushort>(value.toUInt());
        Q_EMIT q->outputFlagsChanged(outputFlags);
    } else if (property == QLatin1String("InputKey")) {
        inputKey = value.toUInt();
        Q_EMIT q->inputKeyChanged(inputKey);
    } else if (property == QLatin1String("OutputKey")) {
        outputKey = value.toUInt();
        Q_EMIT q->outputKeyChanged(outputKey);
    } else if (property == QLatin1String("Local")) {
        localEnd = value.toString();
        Q_EMIT q->localEndChanged(localEnd);
    } else if (property == QLatin1String("Remote")) {
        remoteEnd = value.toString();
        Q_EMIT q->remoteEndChanged(remoteEnd);
    } else if (property == QLatin1String("Parent")) {
        parent = value.toString();
        Q_EMIT q->parentChanged(parent);
    } else if (property == QLatin1String("PathMtuDiscovery")) {
        pathMtuDiscovery = value.toBool();
        Q_EMIT q->pathMtuDiscoveryChanged(pathMtuDiscovery);
    } else if (property == QLatin1String("Tos")) {
        tos = static_cast<uchar>(value.toUInt());
        Q_EMIT q->tosChanged(tos);
    } else if (property == QLatin1String("Ttl")) {
        ttl = static_cast<uchar>(value.toUInt());
        Q_EMIT q->ttlChanged(ttl);
    } else {
        DevicePrivate::propertyChanged(property, value);
    }
}
