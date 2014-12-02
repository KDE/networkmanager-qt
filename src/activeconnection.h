/*
    Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
    Copyright 2011-2013 Lamarque V. Souza <lamarque@kde.org>
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

#ifndef NETWORKMANAGERQT_ACTIVECONNECTION_H
#define NETWORKMANAGERQT_ACTIVECONNECTION_H

#include <QObject>
#include <QDBusObjectPath>
#include <QSharedPointer>

#include <networkmanagerqt_export.h>
#include "connection.h"
#include "dhcp4config.h"
#include "dhcp6config.h"
#include "ipconfig.h"

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
    /**
     * Enum describing possible active connection states
     */
    enum State {
        Unknown = 0, /**< The active connection is in an unknown state */
        Activating, /**< The connection is activating */
        Activated, /**< The connection is activated */
        Deactivating, /**< The connection is being torn down and cleaned up */
        Deactivated /**< The connection is no longer active */
    };
    /**
     * Creates a new ActiveConnection object.
     *
     * @param path the DBus path of the device
     */
    explicit ActiveConnection(const QString &path, QObject *parent = 0);
    explicit ActiveConnection(ActiveConnectionPrivate &dd, QObject *parent = 0);
    /**
     * Destroys an ActiveConnection object.
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
     * Returns a valid NetworkManager::Connection object
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
#if NM_CHECK_VERSION(0, 9, 10)
   /**
     * The Ip4Config object describing the configuration of the
     * connection. Only valid when the connection is in the
     * NM_ACTIVE_CONNECTION_STATE_ACTIVATED state
     */
    IpConfig ipV4Config() const;
   /**
     * The Ip6Config object describing the configuration of the
     * connection. Only valid when the connection is in the
     * NM_ACTIVE_CONNECTION_STATE_ACTIVATED state
     */
    IpConfig ipV6Config() const;
    /**
     * The Dhcp4Config object describing the DHCP options
     * returned by the DHCP server (assuming the connection used DHCP). Only
     * valid when the connection is in the NM_ACTIVE_CONNECTION_STATE_ACTIVATED
     * state
     */
    Dhcp4Config::Ptr dhcp4Config() const;
    /**
     * The Dhcp6Config object describing the DHCP options
     * returned by the DHCP server (assuming the connection used DHCP). Only
     * valid when the connection is in the NM_ACTIVE_CONNECTION_STATE_ACTIVATED
     * state
     */
    Dhcp6Config::Ptr dhcp6Config() const;
    /**
     * The Id of the connection
     */
    QString id() const;
    /**
     * The type of the connection
     */
    NetworkManager::ConnectionSettings::ConnectionType type() const;
#endif
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
     * List of devices UNIs which are part of this connection.
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
    void default4Changed(bool isDefault);
    /**
     * The state of the default IPv6 route changed
     */
    void default6Changed(bool isDefault);
#if NM_CHECK_VERSION(0, 9, 10)
    /**
     * Emitted when the DHCP configuration for IPv4 of this network has changed.
     */
    void dhcp4ConfigChanged();
    /**
     * Emitted when the DHCP configuration for IPv6 of this network has changed.
     */
    void dhcp6ConfigChanged();
    /**
     * Emitted when the IPv4 configuration of this network has changed.
     */
    void ipV4ConfigChanged();
    /**
     * Emitted when the IPv6 configuration of this network has changed.
     */
    void ipV6ConfigChanged();
    /**
     * The @p id changed
     */
    void idChanged(const QString & id);
    /**
     * The @p type changed
     */
    void typeChanged(NetworkManager::ConnectionSettings::ConnectionType type);
#endif
    /**
     * The master device changed.
     */
    void masterChanged(const QString &uni);
    /**
     * The @p path to the specific object changed
     */
    void specificObjectChanged(const QString & path);
    /**
     * The @p state changed
     */
    void stateChanged(NetworkManager::ActiveConnection::State state);
    /**
     * The VPN property changed.
     */
    void vpnChanged(bool isVpn);
    /**
     * The @p uuid changed.
     */
    void uuidChanged(const QString & uuid);
    /**
     * The list of devices changed.
     */
    void devicesChanged();

protected:
    ActiveConnectionPrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(ActiveConnection)
};

} // namespace NetworkManager
#endif // NETWORKMANAGERQT_ACTIVECONNECTION_H

