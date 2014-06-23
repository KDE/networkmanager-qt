/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
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

#include "ipconfig.h"

#include "manager.h"
#include "manager_p.h"

#include <arpa/inet.h>

#include "dbus/nm-ip4-configinterface.h"
#include "dbus/nm-ip6-configinterface.h"

namespace NetworkManager {

class NetworkManager::IpConfig::Private
{
public:
    Private(const QList<IpAddress> &theAddresses,
            const QList<QHostAddress> &theNameservers,
            const QStringList &theDomains, const QList<IpRoute> &theRoutes)
        : addresses(theAddresses)
        , nameservers(theNameservers)
        , domains(theDomains)
        , routes(theRoutes)
    {}
    Private()
    {}
    IpAddresses addresses;
#if NM_CHECK_VERSION(0, 9, 10)
    QString gateway;
    QStringList searches;
#endif
    QList<QHostAddress> nameservers;
    QStringList domains;
    IpRoutes routes;
};

}

NetworkManager::IpConfig::IpConfig(const IpAddresses &addresses,
                                   const QList<QHostAddress> &nameservers,
                                   const QStringList &domains,
                                   const IpRoutes &routes)
    : d(new Private(addresses, nameservers, domains, routes))
{
}

NetworkManager::IpConfig::IpConfig()
    : d(new Private())
{
}

NetworkManager::IpConfig::IpConfig(const IpConfig &other) :
    d(new Private)
{
    *this = other;
}

void NetworkManager::IpConfig::setIPv4Path(const QString &path)
{
    OrgFreedesktopNetworkManagerIP4ConfigInterface iface(NetworkManagerPrivate::DBUS_SERVICE,
                                                         path,
                                                         QDBusConnection::systemBus());
    // TODO - watch propertiesChanged signal

    //convert ipaddresses into object
    UIntListList addresses = iface.addresses();
    QList<NetworkManager::IpAddress> addressObjects;
    foreach (const UIntList &addressList, addresses) {
        if (addressList.count() == 3) {
            NetworkManager::IpAddress address;
            address.setIp(QHostAddress(ntohl(addressList[0])));
            address.setPrefixLength(addressList[1]);
            address.setGateway(QHostAddress(ntohl(addressList[2])));
            addressObjects << address;
        }
    }
    //convert routes into objects
    UIntListList routes = iface.routes();
    QList<NetworkManager::IpRoute> routeObjects;
    foreach (const UIntList &routeList, routes) {
        if (routeList.count() == 4) {
            NetworkManager::IpRoute route;
            route.setIp(QHostAddress(ntohl(routeList[0])));
            route.setPrefixLength(routeList[1]);
            route.setNextHop(QHostAddress(ntohl(routeList[2])));
            route.setMetric(ntohl(routeList[3]));
            routeObjects << route;
        }
    }
    // nameservers' IP addresses are always in network byte order
    QList<QHostAddress> nameservers;
    foreach (uint nameserver, iface.nameservers()) {
        nameservers << QHostAddress(ntohl(nameserver));
    }

    d->addresses = addressObjects;
#if NM_CHECK_VERSION(0, 9, 10)
    d->gateway = iface.gateway();
    d->searches = iface.searches();
#endif
    d->nameservers = nameservers;
    d->domains = iface.domains();
    d->routes = routeObjects;
}

void NetworkManager::IpConfig::setIPv6Path(const QString &path)
{
    // ask the daemon for the details
    OrgFreedesktopNetworkManagerIP6ConfigInterface iface(NetworkManagerPrivate::DBUS_SERVICE,
            path,
            QDBusConnection::systemBus());

    // TODO - watch propertiesChanged signal

    IpV6DBusAddressList addresses = iface.addresses();
    QList<NetworkManager::IpAddress> addressObjects;
    foreach (const IpV6DBusAddress &address, addresses) {
        Q_IPV6ADDR addr;
        Q_IPV6ADDR gateway;
        for (int i = 0; i < 16; i++) {
            addr[i] = address.address[i];
        }
        for (int i = 0; i < 16; i++) {
            gateway[i] = address.gateway[i];
        }
        NetworkManager::IpAddress addressEntry;
        addressEntry.setIp(QHostAddress(addr));
        addressEntry.setPrefixLength(address.prefix);
        addressEntry.setGateway(QHostAddress(gateway));
        addressObjects << addressEntry;
    }

    IpV6DBusRouteList routes = iface.routes();
    QList<NetworkManager::IpRoute> routeObjects;
    foreach (const IpV6DBusRoute &route, routes) {
        Q_IPV6ADDR dest;
        Q_IPV6ADDR nexthop;
        for (int i = 0; i < 16; i++) {
            dest[i] = route.destination[i];
        }
        for (int i = 0; i < 16; i++) {
            nexthop[i] = route.nexthop[i];
        }
        NetworkManager::IpRoute routeEntry;
        routeEntry.setIp(QHostAddress(dest));
        routeEntry.setPrefixLength(route.prefix);
        routeEntry.setNextHop(QHostAddress(nexthop));
        routeEntry.setMetric(route.metric);
        routeObjects << routeEntry;
    }

    QList<QHostAddress> nameservers;
    foreach (const QByteArray &nameserver, iface.nameservers()) {
        Q_IPV6ADDR address;
        for (int i = 0; i < 16; i++) {
            address[i] = static_cast<quint8>(nameserver[i]);
        }
        nameservers << QHostAddress(address);
    }

    d->addresses = addressObjects;
#if NM_CHECK_VERSION(0, 9, 10)
    d->gateway = iface.gateway();
    d->searches = iface.searches();
#endif
    d->nameservers = nameservers;
    d->domains = iface.domains();
    d->routes = routeObjects;
}

NetworkManager::IpConfig::~IpConfig()
{
    delete d;
}

NetworkManager::IpAddresses NetworkManager::IpConfig::addresses() const
{
    return d->addresses;
}

#if NM_CHECK_VERSION(0, 9, 10)
QString NetworkManager::IpConfig::gateway() const
{
    return d->gateway;
}
#endif

QList<QHostAddress> NetworkManager::IpConfig::nameservers() const
{
    return d->nameservers;
}

QStringList NetworkManager::IpConfig::domains() const
{
    return d->domains;
}

QList<NetworkManager::IpRoute> NetworkManager::IpConfig::routes() const
{
    return d->routes;
}

#if NM_CHECK_VERSION(0, 9, 10)
QStringList NetworkManager::IpConfig::searches() const
{
    return d->searches;
}
#endif

NetworkManager::IpConfig &NetworkManager::IpConfig::operator=(const IpConfig &other)
{
    if (this == &other)
        return *this;

    *d = *other.d;
    return *this;
}

bool NetworkManager::IpConfig::isValid() const
{
    return !d->addresses.isEmpty();
}
