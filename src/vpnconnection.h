/*
    SPDX-FileCopyrightText: 2011 Ilia Kats <ilia-kats@gmx.net>
    SPDX-FileCopyrightText: 2011-2013 Lamarque V. Souza <lamarque@kde.org>
    SPDX-FileCopyrightText: 2013 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_VPNCONNECTION_H
#define NETWORKMANAGERQT_VPNCONNECTION_H

#include <networkmanagerqt/networkmanagerqt_export.h>

#include "activeconnection.h"

#include <QObject>

namespace NetworkManager
{
class Device;
class VpnConnectionPrivate;

/*!
 * \class NetworkManager::VpnConnection
 * \inheaderfile NetworkManagerQt/VpnConnection
 * \inmodule NetworkManagerQt
 *
 * \brief An active VPN connection.
 */
class NETWORKMANAGERQT_EXPORT VpnConnection : public ActiveConnection
{
    Q_OBJECT

public:
    /*!
     * \typedef NetworkManager::VpnConnection::Ptr
     */
    typedef QSharedPointer<VpnConnection> Ptr;
    /*!
     * \typedef NetworkManager::VpnConnection::List
     */
    typedef QList<Ptr> List;
    /*!
     * Enum describing the possible VPN connection states
     *
     * \value Unknown
     *        The state of the VPN connection is unknown.
     * \value Prepare
     *        The VPN connection is preparing to connect.
     * \value NeedAuth
     *        The VPN connection needs authorization credentials.
     * \value Connecting
     *        The VPN connection is being established.
     * \value GettingIpConfig
     *        The VPN connection is getting an IP address.
     * \value Activated
     *        The VPN connection is active.
     * \value Failed
     *        The VPN connection failed.
     * \value Disconnected
     *        The VPN connection is disconnected.
     */
    enum State {
        Unknown = 0,
        Prepare,
        NeedAuth,
        Connecting,
        GettingIpConfig,
        Activated,
        Failed,
        Disconnected,
    };
    Q_ENUM(State)

    /*!
     *
     * \value UnknownReason
     *        The reason for the VPN connection state change is unknown.
     * \value NoneReason
     *        No reason was given for the VPN connection state change.
     * \value UserDisconnectedReason
     *        The VPN connection changed state because the user disconnected it.
     * \value DeviceDisconnectedReason
     *        The VPN connection changed state because the device it was using was disconnected.
     * \value ServiceStoppedReason
     *        The service providing the VPN connection was stopped.
     * \value IpConfigInvalidReason
     *        The IP config of the VPN connection was invalid.
     * \value ConnectTimeoutReason
     *        The connection attempt to the VPN service timed out.
     * \value ServiceStartTimeoutReason
     *        A timeout occurred while starting the service providing the VPN connection.
     * \value ServiceStartFailedReason
     *        Starting the service starting the service providing the VPN connection failed.
     * \value NoSecretsReason
     *        Necessary secrets for the VPN connection were not provided.
     * \value LoginFailedReason
     *        Authentication to the VPN server failed.
     * \value ConnectionRemovedReason
     *        The connection was deleted from settings.
     */
    enum StateChangeReason {
        UnknownReason = 0,
        NoneReason,
        UserDisconnectedReason,
        DeviceDisconnectedReason,
        ServiceStoppedReason,
        IpConfigInvalidReason,
        ConnectTimeoutReason,
        ServiceStartTimeoutReason,
        ServiceStartFailedReason,
        NoSecretsReason,
        LoginFailedReason,
        ConnectionRemovedReason,
    };
    Q_ENUM(StateChangeReason)

    /*!
     * Creates a new VpnConnection object.
     *
     * \a path the DBus path of the device
     */
    explicit VpnConnection(const QString &path, QObject *parent = nullptr);
    /*!
     * Destroys a VpnConnection object.
     */
    ~VpnConnection() override;
    /*!
     * Return the current login banner
     */
    QString banner() const;
    /*!
     * returns the current state
     */
    NetworkManager::VpnConnection::State state() const;
    /*!
     * operator for casting an ActiveConnection into a VpnConnection. Returns 0 if this
     * object is not a VPN connection. Introduced to make it possible to create a VpnConnection
     * object for every active connection, without creating an ActiveConnection object, checking
     * if it's a VPN connection, deleting the ActiveConnection and creating a VpnConnection
     */
    operator VpnConnection *();

Q_SIGNALS:
    /*!
     * This signal is emitted when the connection \a banner has changed
     */
    void bannerChanged(const QString &banner);
    /*!
     * This signal is emitted when the VPN connection \a state has changed
     */
    void stateChanged(NetworkManager::VpnConnection::State state, NetworkManager::VpnConnection::StateChangeReason reason);

private:
    Q_DECLARE_PRIVATE(VpnConnection)
};

} // namespace NetworkManager
#endif // NETWORKMANAGERQT_VPNCONNECTION_H
