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

#ifndef NMQT_VPNCONNECTION_H
#define NMQT_VPNCONNECTION_H

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
    enum State {Unknown = 0, Prepare, NeedAuth, Connecting, GettingIpConfig, Activated, Failed, Disconnected};

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
     * This signal is emitted when the connection banner has changed
     */
    void bannerChanged(const QString &);
    /**
     * The state changed
     */
    void stateChanged(NetworkManager::VpnConnection::State);

protected Q_SLOTS:
    void propertiesChanged(const QVariantMap &properties);
    void vpnStateChanged(uint state, uint reason);

private:
    Q_DECLARE_PRIVATE(VpnConnection)
};

} // namespace NetworkManager
#endif //NMQT_VPNCONNECTION_H

