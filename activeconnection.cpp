/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "activeconnection.h"
#include "activeconnection_p.h"
#include "connection.h"
#include "settings.h"
#include "device.h"

#include <QDBusObjectPath>

#include "manager_p.h"
#include "nm-active-connectioninterface.h"


NetworkManager::ActiveConnectionPrivate::ActiveConnectionPrivate(const QString & dbusPath)
    : iface(NetworkManagerPrivate::DBUS_SERVICE, dbusPath, QDBusConnection::systemBus())
{
    connection = NetworkManager::Settings::findConnection(iface.connection().path());
    path = dbusPath;
    default4 = iface.default4();
    default6 = iface.default6();
    specificObject = iface.specificObject().path();
    state = NetworkManager::ActiveConnectionPrivate::convertActiveConnectionState(iface.state());
    vpn = iface.vpn();
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

NetworkManager::Settings::Connection* NetworkManager::ActiveConnection::connection() const
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

QStringList NetworkManager::ActiveConnection::deviceUnis() const
{
    Q_D(const ActiveConnection);
    return d->devices;
}

QList<NetworkManager::Device *> NetworkManager::ActiveConnection::devices() const
{
    Q_D(const ActiveConnection);
    QList<Device *> list;
    foreach (const QString &path, d->devices) {
        list.append(NetworkManager::findNetworkInterface(path));
    }
    return list;
}

void NetworkManager::ActiveConnection::propertiesChanged(const QVariantMap & changedProperties)
{
    Q_D(ActiveConnection);
    //nmDebug() << changedProperties.keys();
    QStringList propKeys = changedProperties.keys();
    QLatin1String connectionKey("Connection"),
                  default4Key("Default"),
                  default6Key("Default6"),
                  specificObjectKey("SpecificObject"),
                  stateKey("State"),
                  vpnKey("Vpn"),
                  devicesKey("Devices");
    QVariantMap::const_iterator it = changedProperties.find(connectionKey);
    if (it != changedProperties.end()) {
        d->connection = NetworkManager::Settings::findConnection(qdbus_cast<QDBusObjectPath>(*it).path());
        emit connectionChanged(d->connection);
        propKeys.removeOne(connectionKey);
    }
    it = changedProperties.find(default4Key);
    if (it != changedProperties.end()) {
        d->default4 = it->toBool();
        emit default4Changed(d->default4);
        propKeys.removeOne(default4Key);
    }
    it = changedProperties.find(default6Key);
    if (it != changedProperties.end()) {
        d->default6 = it->toBool();
        emit default6Changed(d->default6);
        propKeys.removeOne(default6Key);
    }
    it = changedProperties.find(specificObjectKey);
    if (it != changedProperties.end()) {
        d->specificObject = qdbus_cast<QDBusObjectPath>(*it).path();
        emit specificObjectChanged(d->specificObject);
        propKeys.removeOne(specificObjectKey);
    }
    it = changedProperties.find(stateKey);
    if (it != changedProperties.end()) {
        d->state = NetworkManager::ActiveConnectionPrivate::convertActiveConnectionState(it->toUInt());
        emit stateChanged(d->state);
        propKeys.removeOne(stateKey);
    }
    it = changedProperties.find(vpnKey);
    if (it != changedProperties.end()) {
        d->vpn = it->toBool();
        emit vpnChanged(d->vpn);
        propKeys.removeOne(vpnKey);
    }
    it = changedProperties.find(devicesKey);
    if (it != changedProperties.end()) {
        QList<QString> list;
        foreach (const QDBusObjectPath &path, it->value<QList<QDBusObjectPath> >()) {
            list.append(path.path());
        }
        d->devices = list;
        emit devicesChanged();
        propKeys.removeOne(devicesKey);
    }
    //if (propKeys.count()) {
    //    nmDebug() << "Unhandled properties: " << propKeys;
    //}
}

