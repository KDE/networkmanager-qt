/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>

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

#ifndef NMQT_ACTIVECONNECTION_H
#define NMQT_ACTIVECONNECTION_H

#include "QtNetworkManager-export.h"

#include <QObject>
#include <QDBusObjectPath>

namespace NetworkManager
{
namespace Settings
{
    class Connection;
}
class Device;
class ActiveConnectionPrivate;

/**
 * An active connection
 */
class NMQT_EXPORT ActiveConnection : public QObject
{
Q_OBJECT
Q_DECLARE_PRIVATE(ActiveConnection)

public:
    enum State {Unknown = 0, Activating, Activated, Deactivating, Deactivated};
    /**
     * Creates a new ActiveConnection object.
     *
     * @param path the DBus path of the device
     */
    explicit ActiveConnection(const QString & path, QObject * parent = 0);
    explicit ActiveConnection(ActiveConnectionPrivate &dd, QObject * parent = 0);
    /**
     * Destroys a ActiveConnection object.
     */
    virtual ~ActiveConnection();
    /**
     * Return path of the connection object
     */
    QString path() const;
    /**
     * returns a valid NetworkManager::Settings::Connection object
     */
    Settings::Connection* connection() const;
    /**
     * Whether this connection has the default IPv4 route
     */
    bool default4() const;
    /**
     * Whether this connection has the default IPv6 route
     */
    bool default6() const;
    /**
     * Returns the master device if the connection is a slave.
     */
    NetworkManager::Device* master() const;
    /**
     * The path of the specific object associated with the connection.
     */
    QString specificObject() const;
    /**
     * The current state of the connection
     */
    NetworkManager::ActiveConnection::State state() const;
    /**
     * Whether this is a VPN connection
     */
    bool vpn() const;
    /**
     * The UUID of the connection.
     */
    QString uuid() const;
    /**
     * list of device UNIs which are part of this connection.
     */
    QStringList deviceUnis() const;
    /**
     * list of devices which are part of this connection.
     */
    QList<Device *> devices() const;

protected Q_SLOTS:
    void propertiesChanged(const QVariantMap &properties);

protected:
    ActiveConnectionPrivate *d_ptr;

Q_SIGNALS:
    /**
     * This signal is emitted when the connection path has changed
     */
    void connectionChanged(Settings::Connection*);
    /**
     * The state of the default IPv4 route changed
     */
    void default4Changed(bool);
    /**
     * The state of the default IPv6 route changed
     */
    void default6Changed(bool);
    /**
     * The master device changed.
     */
    void masterChanged(NetworkManager::Device*);
    /**
     * The specific object changed
     */
    void specificObjectChanged(const QString &);
    /**
     * The state changed
     */
    void stateChanged(NetworkManager::ActiveConnection::State);
    /**
     * The VPN property changed.
     */
    void vpnChanged(bool);
    /**
     * The uuid changed.
     */
    void uuidChanged(const QString &);
    /**
     * The list of devices changed.
     */
    void devicesChanged();
};

} // namespace NetworkManager
#endif // NMQT_ACTIVECONNECTION_H

