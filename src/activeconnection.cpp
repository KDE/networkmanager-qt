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
#include "device.h"
#include "nmdebug.h"
#include "manager.h"
#include "settings.h"

#include <QDBusObjectPath>

#include "manager_p.h"
#include "nm-active-connectioninterface.h"

NetworkManager::ActiveConnectionPrivate::ActiveConnectionPrivate(const QString &dbusPath, ActiveConnection *q)
#ifdef NMQT_STATIC
    : iface(NetworkManagerPrivate::DBUS_SERVICE, dbusPath, QDBusConnection::sessionBus())
#else
    : iface(NetworkManagerPrivate::DBUS_SERVICE, dbusPath, QDBusConnection::systemBus())

#endif
#if NM_CHECK_VERSION(0, 9, 10)
    , dhcp4Config(0)
    , dhcp6Config(0)
#endif
    , q_ptr(q)
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
    foreach (const QDBusObjectPath & devicePath, iface.devices()) {
        devices.append(devicePath.path());
    }
#if NM_CHECK_VERSION(0, 9, 10)
    id = iface.id();
    type = iface.type();
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
#else
    if (connection && connection->settings()) {
        id = connection->settings()->id();
        type = connection->settings()->typeAsString(connection->settings()->connectionType());
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
    : QObject(parent), d_ptr(new ActiveConnectionPrivate(path, this))
{
    Q_D(ActiveConnection);

    connect(&d->iface, &OrgFreedesktopNetworkManagerConnectionActiveInterface::PropertiesChanged, d, &ActiveConnectionPrivate::propertiesChanged);
}

NetworkManager::ActiveConnection::ActiveConnection(ActiveConnectionPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    Q_D(ActiveConnection);

    connect(&d->iface, &OrgFreedesktopNetworkManagerConnectionActiveInterface::PropertiesChanged, d, &ActiveConnectionPrivate::propertiesChanged);
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

#if NM_CHECK_VERSION(0, 9, 10)
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

QString NetworkManager::ActiveConnection::id() const
{
    Q_D(const ActiveConnection);
    return d->id;
}

NetworkManager::ConnectionSettings::ConnectionType NetworkManager::ActiveConnection::type() const
{
    Q_D(const ActiveConnection);
    return NetworkManager::ConnectionSettings::typeFromString(d->type);
}

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

void NetworkManager::ActiveConnectionPrivate::propertiesChanged(const QVariantMap &properties)
{
    Q_Q(ActiveConnection);

    // qCDebug(NMQT) << Q_FUNC_INFO << properties;

    QVariantMap::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        const QString property = it.key();
        if (property == QLatin1String("Connection")) {
            connection = NetworkManager::findConnection(qdbus_cast<QDBusObjectPath>(*it).path());
            emit q->connectionChanged(connection);
#if !NM_CHECK_VERSION(0, 9, 10)
            const QString tmpId = connection->settings()->id();
            const QString tmpType = connection->settings()->typeAsString(connection->settings()->connectionType());
            if (tmpId != id) {
                id = tmpId;
                emit q->idChanged(id);
            }

            if (tmpType != type) {
                emit q->typeChanged(NetworkManager::ConnectionSettings::typeFromString(type));
            }
#endif
        } else if (property == QLatin1String("Default")) {
            default4 = it->toBool();
            emit q->default4Changed(default4);
        } else if (property == QLatin1String("Default6")) {
            default6 = it->toBool();
            emit q->default6Changed(default6);
#if NM_CHECK_VERSION(0, 9, 10)
        } else if (property == QLatin1String("Dhcp4Config")) {
            QDBusObjectPath dhcp4ConfigPathTmp = (*it).value<QDBusObjectPath>();
            if (dhcp4ConfigPathTmp.path().isNull()) {
                dhcp4Config.clear();
                dhcp4ConfigPath.clear();
            } else if (!dhcp4Config || dhcp4Config->path() != dhcp4ConfigPathTmp.path()) {
                dhcp4Config.clear();
                dhcp4ConfigPath = dhcp4ConfigPathTmp.path();
            }
            emit q->dhcp4ConfigChanged();
        } else if (property == QLatin1String("Dhcp6Config")) {
            QDBusObjectPath dhcp6ConfigPathTmp = (*it).value<QDBusObjectPath>();
            if (dhcp6ConfigPathTmp.path().isNull()) {
                dhcp6Config.clear();
                dhcp6ConfigPath.clear();
            } else if (!dhcp6Config || dhcp6Config->path() != dhcp6ConfigPathTmp.path()) {
                dhcp6Config.clear();
                dhcp6ConfigPath = dhcp6ConfigPathTmp.path();
            }
            emit q->dhcp6ConfigChanged();
        } else if (property == QLatin1String("Ip4Config")) {
            QDBusObjectPath ip4ConfigObjectPathTmp = (*it).value<QDBusObjectPath>();
            if (ip4ConfigObjectPathTmp.path().isNull() || ip4ConfigObjectPathTmp.path() == QLatin1String("/")) {
                ipV4ConfigPath.clear();
            } else {
                ipV4ConfigPath = ip4ConfigObjectPathTmp.path();
            }
            ipV4Config = IpConfig();
            emit q->ipV4ConfigChanged();
        } else if (property == QLatin1String("Ip6Config")) {
            QDBusObjectPath ip6ConfigObjectPathTmp = (*it).value<QDBusObjectPath>();
            if (ip6ConfigObjectPathTmp.path().isNull() || ip6ConfigObjectPathTmp.path() == QLatin1String("/")) {
                ipV6ConfigPath.clear();
            } else {
                ipV6ConfigPath = ip6ConfigObjectPathTmp.path();
            }
            ipV6Config = IpConfig();
            emit q->ipV6ConfigChanged();
        } else if (property == QLatin1String("Id")) {
            id = it->toString();
            emit q->idChanged(id);
        } else if (property == QLatin1String("Type")) {
            type = it->toString();
            emit q->typeChanged(NetworkManager::ConnectionSettings::typeFromString(type));
#endif
        }  else if (property == QLatin1String("Master")) {
            master = qdbus_cast<QDBusObjectPath>(*it).path();
            emit q->masterChanged(master);
        } else if (property == QLatin1String("SpecificObject")) {
            specificObject = qdbus_cast<QDBusObjectPath>(*it).path();
            emit q->specificObjectChanged(specificObject);
        } else if (property == QLatin1String("State")) {
            state = NetworkManager::ActiveConnectionPrivate::convertActiveConnectionState(it->toUInt());
            emit q->stateChanged(state);
        } else if (property == QLatin1String("Vpn")) {
            vpn = it->toBool();
            emit q->vpnChanged(vpn);
        } else if (property == QLatin1String("Uuid")) {
            uuid = it->toString();
            emit q->uuidChanged(uuid);
        } else if (property == QLatin1String("Devices")) {
            devices.clear();
            foreach (const QDBusObjectPath & path, it->value<QList<QDBusObjectPath> >()) {
                devices.append(path.path());
            }
            emit q->devicesChanged();
        } else {
            qCWarning(NMQT) << Q_FUNC_INFO << "Unhandled property" << property;
        }
        ++it;
    }
}
