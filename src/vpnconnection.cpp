/*
    Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
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

#include "vpnconnection_p.h"
#include "connection.h"
#include "device.h"
#include "nmdebug.h"
#include "settings.h"

#include <QDBusObjectPath>

NetworkManager::VpnConnectionPrivate::VpnConnectionPrivate(const QString &path, VpnConnection *q)
    : ActiveConnectionPrivate(path, q)
#ifdef NMQT_STATIC
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , q_ptr(q)
{
}

NetworkManager::VpnConnection::State NetworkManager::VpnConnectionPrivate::convertVpnConnectionState(uint state)
{
    return static_cast<NetworkManager::VpnConnection::State>(state);
}

NetworkManager::VpnConnection::StateChangeReason NetworkManager::VpnConnectionPrivate::convertVpnConnectionStateReason(uint reason)
{
    return static_cast<NetworkManager::VpnConnection::StateChangeReason>(reason);
}

NetworkManager::VpnConnection::VpnConnection(const QString &path, QObject *parent)
    : ActiveConnection(*new VpnConnectionPrivate(path, this), parent)
{
    Q_D(VpnConnection);

    // We need to get ActiveConnection's properties, because by default every ActiveConnection
    // is basically a VpnConnection
    QVariantMap initialProperties = NetworkManagerPrivate::retrieveInitialProperties(OrgFreedesktopNetworkManagerConnectionActiveInterface::staticInterfaceName(), path);
    if (!initialProperties.isEmpty()) {
        d->propertiesChanged(initialProperties);
    }

    // Try to retrieve VPN specific properties if this is a VPN connection
    if (vpn()) {
        // Get all VpnConnection's properties at once
        QVariantMap initialProperties = NetworkManagerPrivate::retrieveInitialProperties(d->iface.staticInterfaceName(), path);
        if (!initialProperties.isEmpty()) {
            d->propertiesChanged(initialProperties);
        }
    }

#if NM_CHECK_VERSION(1, 4, 0)
    QDBusConnection::systemBus().connect(NetworkManagerPrivate::DBUS_SERVICE, d->path, NetworkManagerPrivate::FDO_DBUS_PROPERTIES,
                                         QLatin1String("PropertiesChanged"), d, SLOT(dbusPropertiesChanged(QString,QVariantMap,QStringList)));
#else
    connect(&d->iface, &OrgFreedesktopNetworkManagerVPNConnectionInterface::PropertiesChanged, d, &VpnConnectionPrivate::propertiesChanged);
#endif
    connect(&d->iface, &OrgFreedesktopNetworkManagerVPNConnectionInterface::VpnStateChanged, d, &VpnConnectionPrivate::vpnStateChanged);
}

NetworkManager::VpnConnection::~VpnConnection()
{
}

QString NetworkManager::VpnConnection::banner() const
{
    Q_D(const VpnConnection);
    //return d->banner; // FIXME NM doesn't Q_EMIT the Banner property change
    return d->iface.banner();
}

NetworkManager::VpnConnection::State NetworkManager::VpnConnection::state() const
{
    Q_D(const VpnConnection);
    return d->state;
}

void NetworkManager::VpnConnectionPrivate::dbusPropertiesChanged(const QString &interfaceName, const QVariantMap &properties, const QStringList &invalidatedProperties)
{
    Q_UNUSED(invalidatedProperties);

    if (interfaceName == QLatin1String("org.freedesktop.NetworkManager.VPN.Connection")) {
        propertiesChanged(properties);
    } else {
        ActiveConnectionPrivate::propertiesChanged(properties);
    }
}

void NetworkManager::VpnConnectionPrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(VpnConnection);

    if (property == QLatin1String("Banner")) {
        banner = value.toString();
        Q_EMIT q->bannerChanged(banner);
    } else if (property == QLatin1String("VpnState")) {
        //Do not notify about changed VpnState twice, because there is also signal VpnStateChanged() from NetworkManager
        state = NetworkManager::VpnConnectionPrivate::convertVpnConnectionState(value.toUInt());
        //NetworkManager::VpnConnection::StateChangeReason reason = NetworkManager::VpnConnectionPrivate::convertVpnConnectionStateReason(properties.key("Reason").toUInt());
        //Q_EMIT stateChanged(d->state, reason);
    } else {
        ActiveConnectionPrivate::propertyChanged(property, value);
    }
}

void NetworkManager::VpnConnectionPrivate::vpnStateChanged(uint newState, uint reason)
{
    Q_Q(VpnConnection);
    Q_UNUSED(reason);

    state = NetworkManager::VpnConnectionPrivate::convertVpnConnectionState(newState);
    NetworkManager::VpnConnection::StateChangeReason stateChangeReason = NetworkManager::VpnConnectionPrivate::convertVpnConnectionStateReason(reason);
    Q_EMIT q->stateChanged(state, stateChangeReason);
}

NetworkManager::VpnConnection::operator VpnConnection *()
{
    Q_D(VpnConnection);
    if (d->vpn) {
        return this;
    } else {
        return 0;
    }
}
