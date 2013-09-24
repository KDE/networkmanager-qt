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

#ifndef NETWORKMANAGERQT_VPNCONNECTION_H
#define NETWORKMANAGERQT_VPNCONNECTION_H

#include "NetworkManagerQt-export.h"

#include "activeconnection.h"

#include <QObject>
#include <QDBusObjectPath>

namespace NetworkManager
{

class Device;
class VpnConnectionPrivate;

/**
 * An active connection
 */
class NETWORKMANAGERQT_EXPORT VpnConnection : public ActiveConnection
{
    Q_OBJECT

public:
    typedef QSharedPointer<VpnConnection> Ptr;
    typedef QList<Ptr> List;
    /**
     * Enum describing the possible VPN connection states
     */
    enum State {
        Unknown = 0, /**< The state of the VPN connection is unknown. */
        Prepare, /**< The VPN connection is preparing to connect. */
        NeedAuth, /**< The VPN connection needs authorization credentials. */
        Connecting, /**< The VPN connection is being established. */
        GettingIpConfig, /**< The VPN connection is getting an IP address. */
        Activated, /**< The VPN connection is active. */
        Failed, /**< The VPN connection failed. */
        Disconnected /**< The VPN connection is disconnected. */
    };

    enum StateChangeReason {
        UnknownReason = 0, /**< The reason for the VPN connection state change is unknown.*/
        NoneReason, /**< No reason was given for the VPN connection state change. */
        UserDisconnectedReason, /**< The VPN connection changed state because the user disconnected it. */
        DeviceDisconnectedReason, /**< The VPN connection changed state because the device it was using was disconnected. */
        ServiceStoppedReason, /**< The service providing the VPN connection was stopped. */
        IpConfigInvalidReason, /**< The IP config of the VPN connection was invalid. */
        ConnectTimeoutReason, /**< The connection attempt to the VPN service timed out. */
        ServiceStartTimeoutReason, /**< A timeout occurred while starting the service providing the VPN connection. */
        ServiceStartFailedReason, /**< Starting the service starting the service providing the VPN connection failed. */
        NoSecretsReason, /**< Necessary secrets for the VPN connection were not provided. */
        LoginFailedReason, /**< Authentication to the VPN server failed. */
        ConnectionRemovedReason /**< The connection was deleted from settings. */
    };

    /**
     * Creates a new VpnConnection object.
     *
     * @param path the DBus path of the device
     */
    explicit VpnConnection(const QString &path, QObject *parent = 0);
    /**
     * Destroys a VpnConnection object.
     */
    virtual ~VpnConnection();
    /**
     * Return the current login banner
     */
    QString banner() const;
    /**
     * returns the current state
     */
    NetworkManager::VpnConnection::State state() const;
    /**
     * operator for casting an ActiveConnection into a VpnConnection. Returns 0 if this
     * object is not a VPN connection. Introduced to make it possible to create a VpnConnection
     * object for every active connection, without creating an ActiveConnection object, checking
     * if it's a VPN connection, deleting the ActiveConnection and creating a VpnConnection
     */
    operator VpnConnection *();

Q_SIGNALS:
    /**
     * This signal is emitted when the connection @p banner has changed
     */
    void bannerChanged(const QString & banner);
    /**
     * This signal is emitted when the VPN connection @p state has changed
     */
    void stateChanged(NetworkManager::VpnConnection::State state, NetworkManager::VpnConnection::StateChangeReason reason);

protected Q_SLOTS:
    void propertiesChanged(const QVariantMap &properties);
    void vpnStateChanged(uint state, uint reason);

private:
    Q_DECLARE_PRIVATE(VpnConnection)
};

} // namespace NetworkManager
#endif //NETWORKMANAGERQT_VPNCONNECTION_H

