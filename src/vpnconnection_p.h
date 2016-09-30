/*
    Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
    Copyright 2013-2014 Jan Grulich <jgrulich@redhat.com>

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

#ifndef NETWORKMANAGERQT_VPNCONNECTION_P_H
#define NETWORKMANAGERQT_VPNCONNECTION_P_H

#include "activeconnection_p.h"
#include "manager_p.h"
#include "vpnconnection.h"

#include "nm-vpn-connectioninterface.h"

namespace NetworkManager
{

class VpnConnectionPrivate : public ActiveConnectionPrivate
{
    Q_OBJECT
public:
    VpnConnectionPrivate(const QString &path, VpnConnection *q);

    static NetworkManager::VpnConnection::State convertVpnConnectionState(uint state);
    static NetworkManager::VpnConnection::StateChangeReason convertVpnConnectionStateReason(uint reason);

    QString banner;
    NetworkManager::VpnConnection::State state;
    OrgFreedesktopNetworkManagerVPNConnectionInterface iface;

    Q_DECLARE_PUBLIC(VpnConnection)
    VpnConnection *q_ptr;

protected:
    /**
     * When subclassing make sure to call the parent class method
     * if the property was not useful to your new class
     */
    virtual void propertyChanged(const QString &property, const QVariant &value) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void dbusPropertiesChanged(const QString &interfaceName, const QVariantMap &properties, const QStringList &invalidatedProperties);
    void vpnStateChanged(uint new_state, uint reason);
};

}

#endif // NETWORKMANAGERQT_VPNCONNECTION_P_H
