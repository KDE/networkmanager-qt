/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
Copyright 2011-2013 Lamarque V. Souza <lamarque@kde.org>

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

#include <QObject>
#include <QDBusObjectPath>
#include <QSharedPointer>

#include "NetworkManagerQt-export.h"
#include "connection.h"

namespace NetworkManager {

class ActiveConnectionPrivate;

/**
 * An active connection
 */
class NETWORKMANAGERQT_EXPORT ActiveConnection : public QObject
{
    Q_OBJECT

public:
    typedef QSharedPointer<ActiveConnection> Ptr;
    typedef QList<Ptr> List;
    enum State {Unknown = 0, Activating, Activated, Deactivating, Deactivated};
    /**
     * Creates a new ActiveConnection object.
     *
     * @param path the DBus path of the device
     */
    explicit ActiveConnection(const QString &path, QObject *parent = 0);
    explicit ActiveConnection(ActiveConnectionPrivate &dd, QObject *parent = 0);
    /**
     * Destroys a ActiveConnection object.
     */
    virtual ~ActiveConnection();


    /**
     * Returns true is this object holds a valid connection
     */
    bool isValid() const;

    /**
     * Return path of the connection object
     */
    QString path() const;
    /**
     * returns a valid NetworkManager::Connection object
     */
    Connection::Ptr connection() const;
    /**
     * Whether this connection has the default IPv4 route
     */
    bool default4() const;
    /**
     * Whether this connection has the default IPv6 route
     */
    bool default6() const;
    /**
     * Returns the uni of master device if the connection is a slave.
     */
    QString master() const;
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
     * list of devices UNIs which are part of this connection.
     */
    QStringList devices() const;

Q_SIGNALS:
    /**
     * This signal is emitted when the connection path has changed
     */
    void connectionChanged(const NetworkManager::Connection::Ptr &connection);
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
    void masterChanged(const QString &uni);
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

protected:
    ActiveConnectionPrivate *d_ptr;

protected Q_SLOTS:
    void propertiesChanged(const QVariantMap &properties);

private:
    Q_DECLARE_PRIVATE(ActiveConnection)
};

} // namespace NetworkManager
#endif // NMQT_ACTIVECONNECTION_H

