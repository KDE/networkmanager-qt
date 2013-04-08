/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>

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
#include "nm_active_connectioninterface.h"


NetworkManager::ActiveConnectionPrivate::ActiveConnectionPrivate(const QString & dbusPath)
    : iface(NetworkManagerPrivate::DBUS_SERVICE, dbusPath, QDBusConnection::systemBus())
{
    connection = NetworkManager::Settings::findConnection(iface.connection().path());
    path = dbusPath;

    default4 = qvariant_cast< bool >(iface.property("Default"));
    default6 = iface.default6();
    specificObject = iface.specificObject().path();
    state = NetworkManager::ActiveConnectionPrivate::convertActiveConnectionState(iface.state());
    vpn = iface.vpn();
    uuid = iface.uuid();
    master = iface.master().path();
    foreach (const QDBusObjectPath &devicePath, iface.devices()) {
        devices.append(devicePath.path());
    }
}

NetworkManager::ActiveConnectionPrivate::~ActiveConnectionPrivate()
{
}

NetworkManager::ActiveConnection::State NetworkManager::ActiveConnectionPrivate::convertActiveConnectionState(uint state)
{
    return (NetworkManager::ActiveConnection::State)state;
}

NetworkManager::ActiveConnection::ActiveConnection(const QString & path, QObject * parent)
    : QObject(parent), d_ptr(new ActiveConnectionPrivate(path))
{
    Q_D(ActiveConnection);

    connect( &d->iface, SIGNAL(PropertiesChanged(QVariantMap)),
             this, SLOT(propertiesChanged(QVariantMap)));
}

NetworkManager::ActiveConnection::ActiveConnection(ActiveConnectionPrivate &dd, QObject * parent)
    : QObject(parent), d_ptr(&dd)
{
    Q_D(ActiveConnection);

    connect( &d->iface, SIGNAL(PropertiesChanged(QVariantMap)),
             this, SLOT(propertiesChanged(QVariantMap)));
}

NetworkManager::ActiveConnection::~ActiveConnection()
{
    delete d_ptr;
}

NetworkManager::Settings::Connection::Ptr NetworkManager::ActiveConnection::connection() const
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
        QString property = it.key();
        if (property == QLatin1String("Connection")) {
            d->connection = NetworkManager::Settings::findConnection(qdbus_cast<QDBusObjectPath>(*it).path());
            emit connectionChanged(d->connection);
        } else if (property == QLatin1String("Default")) {
            d->default4 = it->toBool();
            emit default4Changed(d->default4);
        } else if (property == QLatin1String("Default6")) {
            d->default6 = it->toBool();
            emit default6Changed(d->default6);
        } else if (property == QLatin1String("Master")) {
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

#include "activeconnection.moc"
