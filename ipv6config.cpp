/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>, based on work by Will Stephenson <wstephenson@kde.org>

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

#include "ipv6config.h"

namespace NetworkManager
{
class IPv6Config::Private
{
public:
    Private(const QList<IPv6Address> &theAddresses,
        const QList<Q_IPV6ADDR> &theNameservers,
        const QStringList &theDomains, const QList<IPv6Route> &theRoutes)
        : addresses(theAddresses), nameservers(theNameservers),
        domains(theDomains), routes(theRoutes)
    {}
    Private()
    {}
    QList<IPv6Address> addresses;
    QList<Q_IPV6ADDR> nameservers;
    QStringList domains;
    QList<IPv6Route> routes;
};


class IPv6Address::Private
{
public:
    Private(Q_IPV6ADDR theAddress, quint32 theNetMask, Q_IPV6ADDR theGateway)
        : address(theAddress), netMask(theNetMask), gateway(theGateway)
    {}
    Private()
        : address(), netMask(0), gateway()
    {}
    Q_IPV6ADDR address;
    quint32 netMask;
    Q_IPV6ADDR gateway;
};

class IPv6Route::Private
{
public:
    Private(Q_IPV6ADDR theRoute, quint32 thePrefix, Q_IPV6ADDR theNextHop, quint32 theMetric)
        : route(theRoute), prefix(thePrefix), nextHop(theNextHop), metric(theMetric)
    {}
    Private()
        : route(), prefix(0), nextHop(), metric(0)
    {}
    Q_IPV6ADDR route;
    quint32 prefix;
    Q_IPV6ADDR nextHop;
    quint32 metric;
};
}

NetworkManager::IPv6Address::IPv6Address(Q_IPV6ADDR address, quint32 netMask, Q_IPV6ADDR gateway)
: d(new Private(address, netMask, gateway))
{
}

NetworkManager::IPv6Address::IPv6Address()
: d(new Private())
{
}

NetworkManager::IPv6Address::~IPv6Address()
{
    delete d;
}

NetworkManager::IPv6Address::IPv6Address(const IPv6Address &other)
: d(new Private(*other.d))
{
}

Q_IPV6ADDR NetworkManager::IPv6Address::address() const
{
    return d->address;
}

quint32 NetworkManager::IPv6Address::netMask() const
{
    return d->netMask;
}

Q_IPV6ADDR NetworkManager::IPv6Address::gateway() const
{
    return d->gateway;
}

NetworkManager::IPv6Address &NetworkManager::IPv6Address::operator=(const NetworkManager::IPv6Address &other)
{
    *d = *other.d;
    return *this;
}

bool NetworkManager::IPv6Address::isValid() const
{
    return !QHostAddress(d->address).isNull();
}

NetworkManager::IPv6Route::IPv6Route(Q_IPV6ADDR route, quint32 prefix, Q_IPV6ADDR nextHop, quint32 metric)
: d(new Private(route, prefix, nextHop, metric))
{
}

NetworkManager::IPv6Route::IPv6Route()
: d(new Private())
{
}

NetworkManager::IPv6Route::~IPv6Route()
{
    delete d;
}

NetworkManager::IPv6Route::IPv6Route(const IPv6Route &other)
: d(new Private(*other.d))
{
}

Q_IPV6ADDR NetworkManager::IPv6Route::route() const
{
    return d->route;
}

quint32 NetworkManager::IPv6Route::prefix() const
{
    return d->prefix;
}

Q_IPV6ADDR NetworkManager::IPv6Route::nextHop() const
{
    return d->nextHop;
}

quint32 NetworkManager::IPv6Route::metric() const
{
    return d->metric;
}

NetworkManager::IPv6Route &NetworkManager::IPv6Route::operator=(const NetworkManager::IPv6Route &other)
{
    *d = *other.d;
    return *this;
}

bool NetworkManager::IPv6Route::isValid() const
{
    return !QHostAddress(d->route).isNull();
}


NetworkManager::IPv6Config::IPv6Config(const QList<IPv6Address> &addresses,
        const QList<Q_IPV6ADDR> &nameservers,
        const QStringList &domains,
        const QList<IPv6Route> &routes)
: d(new Private(addresses, nameservers, domains, routes))
{
}

NetworkManager::IPv6Config::IPv6Config()
: d(new Private())
{
}

NetworkManager::IPv6Config::IPv6Config(const NetworkManager::IPv6Config& other)
{
    d = new Private(*other.d);
}

NetworkManager::IPv6Config::~IPv6Config()
{
    delete d;
}

QList<NetworkManager::IPv6Address> NetworkManager::IPv6Config::addresses() const
{
    return d->addresses;
}

QList<Q_IPV6ADDR> NetworkManager::IPv6Config::nameservers() const
{
    return d->nameservers;
}

QStringList NetworkManager::IPv6Config::domains() const
{
    return d->domains;
}

QList<NetworkManager::IPv6Route> NetworkManager::IPv6Config::routes() const
{
    return d->routes;
}

NetworkManager::IPv6Config &NetworkManager::IPv6Config::operator=(const NetworkManager::IPv6Config& other)
{
    *d = *other.d;
    return *this;
}

bool NetworkManager::IPv6Config::isValid() const
{
    return !d->addresses.isEmpty();
}

