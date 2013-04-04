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

namespace NetworkManager {
class NetworkManager::IpConfig::Private
{
public:
    Private(const QList<IpAddress> &theAddresses,
        const QList<QHostAddress> &theNameservers,
        const QStringList &theDomains, const QList<IpRoute> &theRoutes)
        : addresses(theAddresses), nameservers(theNameservers),
        domains(theDomains), routes(theRoutes)
    {}
    Private()
    {}
    IpAddresses addresses;
    QList<QHostAddress> nameservers;
    QStringList domains;
    IpRoutes routes;
};


class IpAddress::Private
{
public:
    QHostAddress gateway;
};

class NetworkManager::IpRoute::Private
{
public:
    Private() :
        metric(0) {}
    QHostAddress nextHop;
    quint32 metric;
};
}

NetworkManager::IpAddress::IpAddress()
: d(new Private)
{
}

NetworkManager::IpAddress::~IpAddress()
{
    delete d;
}

NetworkManager::IpAddress::IpAddress(const NetworkManager::IpAddress &other) :
    d(new Private)
{
    *this = other;
}

bool NetworkManager::IpAddress::isValid() const
{
    return !ip().isNull();
}

void NetworkManager::IpAddress::setGateway(const QHostAddress &gateway)
{
    d->gateway = gateway;
}

QHostAddress NetworkManager::IpAddress::gateway() const
{
    return d->gateway;
}

NetworkManager::IpAddress &NetworkManager::IpAddress::operator=(const NetworkManager::IpAddress &other)
{
    if (this == &other)
        return *this;

    QNetworkAddressEntry::operator =(other);
    *d = *other.d;

    return *this;
}

NetworkManager::IpRoute::IpRoute()
: d(new Private)
{
}

NetworkManager::IpRoute::~IpRoute()
{
    delete d;
}

NetworkManager::IpRoute::IpRoute(const NetworkManager::IpRoute &other) :
    d(new Private)
{
    *this = other;
}

void NetworkManager::IpRoute::setNextHop(const QHostAddress &nextHop) const
{
    d->nextHop = nextHop;
}

QHostAddress NetworkManager::IpRoute::nextHop() const
{
    return d->nextHop;
}

void NetworkManager::IpRoute::setMetric(quint32 metric)
{
    d->metric = metric;
}

quint32 NetworkManager::IpRoute::metric() const
{
    return d->metric;
}

NetworkManager::IpRoute &NetworkManager::IpRoute::operator=(const NetworkManager::IpRoute &other)
{
    if (this == &other)
        return *this;

    QNetworkAddressEntry::operator =(other);
    *d = *other.d;

    return *this;
}

bool NetworkManager::IpRoute::isValid() const
{
    return !ip().isNull();
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

NetworkManager::IpConfig::IpConfig(const IpConfig& other) :
    d(new Private)
{
    *this = other;
}

NetworkManager::IpConfig::~IpConfig()
{
    delete d;
}

NetworkManager::IpAddresses NetworkManager::IpConfig::addresses() const
{
    return d->addresses;
}

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

NetworkManager::IpConfig &NetworkManager::IpConfig::operator=(const IpConfig& other)
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

