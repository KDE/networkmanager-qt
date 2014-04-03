/*
    Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
    Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>
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

#include "activeconnection.h"
#include "activeconnection_p.h"
#include "connection.h"
#include "settings.h"
#include "device.h"
#include "manager.h"

#include <QDBusObjectPath>

#include "manager_p.h"
#include "nm-active-connectioninterface.h"


NetworkManager::ActiveConnectionPrivate::ActiveConnectionPrivate(const QString &dbusPath)
    : iface(NetworkManagerPrivate::DBUS_SERVICE, dbusPath, QDBusConnection::systemBus())
#if NM_CHECK_VERSION(0, 9, 9)
    , dhcp4Config(0)
    , dhcp6Config(0)
#endif
{
    connection = NetworkManager::findConnection(iface.connection().path());
    path = dbusPath;
    default4 = iface.default4();
    default6 = iface.default6();
    specificObject = iface.specificObject().path();
    state = NetworkManager::ActiveConnectionPrivate::convertActiveConnectionState(iface.state());
    vpn = iface.vpn();
    uuid = iface.uuid();
    master = iface.master().path();
    foreach (const QDBusObjectPath &devicePath, iface.devices()) {
        devices.append(devicePath.path());
    }
#if NM_CHECK_VERSION(0, 9, 9)
    QDBusObjectPath ip4ConfigObjectPath = iface.ip4Config();
    if (!ip4ConfigObjectPath.path().isNull() || ip4ConfigObjectPath.path() != QLatin1String("/")) {
        ipV4ConfigPath = ip4ConfigObjectPath.path();
    }

    QDBusObjectPath ip6ConfigObjectPath = iface.ip6Config();
    if (!ip6ConfigObjectPath.path().isNull() || ip6ConfigObjectPath.path() != QLatin1String("/")) {
        ipV6ConfigPath = ip6ConfigObjectPath.path();
    }

    QDBusObjectPath dhcp4ConfigObjectPath = iface.dhcp4Config();
    if (!dhcp4ConfigObjectPath.path().isNull() && dhcp4ConfigObjectPath.path() != QLatin1String("/")) {
        dhcp4ConfigPath = dhcp4ConfigObjectPath.path();
    }

    QDBusObjectPath dhcp6ConfigObjectPath = iface.dhcp6Config();
    if (!dhcp6ConfigObjectPath.path().isNull() && dhcp6ConfigObjectPath.path() != QLatin1String("/")) {
        dhcp6ConfigPath = dhcp6ConfigObjectPath.path();
    }
#endif
}

NetworkManager::ActiveConnectionPrivate::~ActiveConnectionPrivate()
{
}

NetworkManager::ActiveConnection::State NetworkManager::ActiveConnectionPrivate::convertActiveConnectionState(uint state)
{
    return (NetworkManager::ActiveConnection::State)state;
}

NetworkManager::ActiveConnection::ActiveConnection(const QString &path, QObject *parent)
    : QObject(parent), d_ptr(new ActiveConnectionPrivate(path))
{
    Q_D(ActiveConnection);

    connect(&d->iface, SIGNAL(PropertiesChanged(QVariantMap)),
            this, SLOT(propertiesChanged(QVariantMap)));
}

NetworkManager::ActiveConnection::ActiveConnection(ActiveConnectionPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    Q_D(ActiveConnection);

    connect(&d->iface, SIGNAL(PropertiesChanged(QVariantMap)),
            this, SLOT(propertiesChanged(QVariantMap)));
}

NetworkManager::ActiveConnection::~ActiveConnection()
{
    delete d_ptr;
}

bool NetworkManager::ActiveConnection::isValid() const
{
    Q_D(const ActiveConnection);
    return !d->connection.isNull();
}

NetworkManager::Connection::Ptr NetworkManager::ActiveConnection::connection() const
{
    Q_D(const ActiveConnection);
    return d->connection;
}

QString NetworkManager::ActiveConnection::path() const
{
    Q_D(const ActiveConnection);
    return d->path;
}

bool NetworkManager::ActiveConnection::default4() const
{
    Q_D(const ActiveConnection);
    return d->default4;
}

bool NetworkManager::ActiveConnection::default6() const
{
    Q_D(const ActiveConnection);
    return d->default6;
}
#if NM_CHECK_VERSION(0, 9, 9)
NetworkManager::Dhcp4Config::Ptr NetworkManager::ActiveConnection::dhcp4Config() const
{
    Q_D(const ActiveConnection);
    if (!d->dhcp4Config && !d->dhcp4ConfigPath.isNull()) {
        d->dhcp4Config = NetworkManager::Dhcp4Config::Ptr(new Dhcp4Config(d->dhcp4ConfigPath), &QObject::deleteLater);
    }
    return d->dhcp4Config;
}

NetworkManager::Dhcp6Config::Ptr NetworkManager::ActiveConnection::dhcp6Config() const
{
    Q_D(const ActiveConnection);
    if (!d->dhcp6Config && !d->dhcp6ConfigPath.isNull()) {
        d->dhcp6Config = NetworkManager::Dhcp6Config::Ptr(new Dhcp6Config(d->dhcp6ConfigPath), &QObject::deleteLater);
    }
    return d->dhcp6Config;
}

NetworkManager::IpConfig NetworkManager::ActiveConnection::ipV4Config() const
{
    Q_D(const ActiveConnection);
    if (!d->ipV4Config.isValid() && !d->ipV4ConfigPath.isNull()) {
        d->ipV4Config.setIPv4Path(d->ipV4ConfigPath);
    }
    return d->ipV4Config;
}

NetworkManager::IpConfig NetworkManager::ActiveConnection::ipV6Config() const
{
    Q_D(const ActiveConnection);
    if (!d->ipV6Config.isValid() && !d->ipV6ConfigPath.isNull()) {
        d->ipV6Config.setIPv6Path(d->ipV6ConfigPath);
    }
    return d->ipV6Config;
}
#endif
QString NetworkManager::ActiveConnection::master() const
{
    Q_D(const ActiveConnection);
    return d->master;
}

QString NetworkManager::ActiveConnection::specificObject() const
{
    Q_D(const ActiveConnection);
    return d->specificObject;
}

NetworkManager::ActiveConnection::State NetworkManager::ActiveConnection::state() const
{
    Q_D(const ActiveConnection);
    return d->state;
}

bool NetworkManager::ActiveConnection::vpn() const
{
    Q_D(const ActiveConnection);
    return d->vpn;
}

QString NetworkManager::ActiveConnection::uuid() const
{
    Q_D(const ActiveConnection);
    return d->uuid;
}

QStringList NetworkManager::ActiveConnection::devices() const
{
    Q_D(const ActiveConnection);
    return d->devices;
}

void NetworkManager::ActiveConnection::propertiesChanged(const QVariantMap &properties)
{
    Q_D(ActiveConnection);

    QVariantMap::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        const QString property = it.key();
        if (property == QLatin1String("Connection")) {
            d->connection = NetworkManager::findConnection(qdbus_cast<QDBusObjectPath>(*it).path());
            emit connectionChanged(d->connection);
        } else if (property == QLatin1String("Default")) {
            d->default4 = it->toBool();
            emit default4Changed(d->default4);
        } else if (property == QLatin1String("Default6")) {
            d->default6 = it->toBool();
            emit default6Changed(d->default6);
#if NM_CHECK_VERSION(0, 9, 9)
        } else if (property == QLatin1String("Dhcp4Config")) {
            QDBusObjectPath dhcp4ConfigPath = (*it).value<QDBusObjectPath>();
            if (dhcp4ConfigPath.path().isNull()) {
                d->dhcp4Config.clear();
                d->dhcp4ConfigPath.clear();
            } else if (!d->dhcp4Config || d->dhcp4Config->path() != dhcp4ConfigPath.path()) {
                d->dhcp4Config.clear();
                d->dhcp4ConfigPath = dhcp4ConfigPath.path();
            }
            emit dhcp4ConfigChanged();
        } else if (property == QLatin1String("Dhcp6Config")) {
            QDBusObjectPath dhcp6ConfigPath = (*it).value<QDBusObjectPath>();
            if (dhcp6ConfigPath.path().isNull()) {
                d->dhcp6Config.clear();
                d->dhcp6ConfigPath.clear();
            } else if (!d->dhcp6Config || d->dhcp6Config->path() != dhcp6ConfigPath.path()) {
                d->dhcp6Config.clear();
                d->dhcp6ConfigPath = dhcp6ConfigPath.path();
            }
            emit dhcp6ConfigChanged();
        } else if (property == QLatin1String("Ip4Config")) {
            QDBusObjectPath ip4ConfigObjectPath = (*it).value<QDBusObjectPath>();
            if (ip4ConfigObjectPath.path().isNull() || ip4ConfigObjectPath.path() == QLatin1String("/")) {
                d->ipV4ConfigPath.clear();
            } else {
                d->ipV4ConfigPath = ip4ConfigObjectPath.path();
            }
            d->ipV4Config = IpConfig();
            emit ipV4ConfigChanged();
        } else if (property == QLatin1String("Ip6Config")) {
            QDBusObjectPath ip6ConfigObjectPath = (*it).value<QDBusObjectPath>();
            if (ip6ConfigObjectPath.path().isNull() || ip6ConfigObjectPath.path() == QLatin1String("/")) {
                d->ipV6ConfigPath.clear();
            } else {
                d->ipV6ConfigPath = ip6ConfigObjectPath.path();
            }
            d->ipV6Config = IpConfig();
            emit ipV6ConfigChanged();
#endif
        }  else if (property == QLatin1String("Master")) {
            d->master = qdbus_cast<QDBusObjectPath>(*it).path();
            emit masterChanged(d->master);
        } else if (property == QLatin1String("SpecificObject")) {
            d->specificObject = qdbus_cast<QDBusObjectPath>(*it).path();
            emit specificObjectChanged(d->specificObject);
        } else if (property == QLatin1String("State")) {
            d->state = NetworkManager::ActiveConnectionPrivate::convertActiveConnectionState(it->toUInt());
            emit stateChanged(d->state);
        } else if (property == QLatin1String("Vpn")) {
            d->vpn = it->toBool();
            emit vpnChanged(d->vpn);
        } else if (property == QLatin1String("Uuid")) {
            d->uuid = it->toString();
            emit uuidChanged(d->uuid);
        } else if (property == QLatin1String("Devices")) {
            d->devices.clear();
            foreach (const QDBusObjectPath &path, it->value<QList<QDBusObjectPath> >()) {
                d->devices.append(path.path());
            }
            emit devicesChanged();
        } else {
            qWarning() << Q_FUNC_INFO << "Unhandled property" << property;
        }
        ++it;
    }
}
