/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>, based on work by Will Stephenson <wstephenson@kde.org>
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

#include "ipv6config.h"

namespace NetworkManager
{
class IPv6Config::Private
{
public:
    Private(const QList<IPv6Address> &theAddresses,
        const QList<QHostAddress> &theNameservers,
        const QStringList &theDomains, const QList<IPv6Route> &theRoutes)
        : addresses(theAddresses), nameservers(theNameservers),
        domains(theDomains), routes(theRoutes)
    {}
    Private()
    {}
    QList<IPv6Address> addresses;
    QList<QHostAddress> nameservers;
    QStringList domains;
    QList<IPv6Route> routes;
};


class IPv6Address::Private
{
public:
    QHostAddress gateway;
};

class IPv6Route::Private
{
public:
    QHostAddress nextHop;
    quint32 metric;
};
}

NetworkManager::IPv6Address::IPv6Address() :
    d(new Private)
{
}

NetworkManager::IPv6Address::~IPv6Address()
{
    delete d;
}

NetworkManager::IPv6Address::IPv6Address(const IPv6Address &other) :
    d(new Private)
{
    *this = other;
}

void NetworkManager::IPv6Address::setGateway(const QHostAddress &gateway)
{
    d->gateway = gateway;
}

QHostAddress NetworkManager::IPv6Address::gateway() const
{
    return d->gateway;
}

NetworkManager::IPv6Address &NetworkManager::IPv6Address::operator=(const NetworkManager::IPv6Address &other)
{
    QNetworkAddressEntry::operator =(other);
    *d = *other.d;

    return *this;
}

bool NetworkManager::IPv6Address::isValid() const
{
    return !ip().isNull();
}

NetworkManager::IPv6Route::IPv6Route()
: d(new Private())
{
}

NetworkManager::IPv6Route::~IPv6Route()
{
    delete d;
}

NetworkManager::IPv6Route::IPv6Route(const IPv6Route &other) :
    d(new Private)
{
    *this = other;
}

QHostAddress NetworkManager::IPv6Route::nextHop() const
{
    return d->nextHop;
}

void NetworkManager::IPv6Route::setMetric(quint32 metric)
{
    d->metric = metric;
}

quint32 NetworkManager::IPv6Route::metric() const
{
    return d->metric;
}

NetworkManager::IPv6Route &NetworkManager::IPv6Route::operator=(const NetworkManager::IPv6Route &other)
{
    QNetworkAddressEntry::operator =(other);
    *d = *other.d;

    return *this;
}

bool NetworkManager::IPv6Route::isValid() const
{
    return !ip().isNull();
}

void NetworkManager::IPv6Route::setNextHop(const QHostAddress &nextHop)
{
    d->nextHop = nextHop;
}

NetworkManager::IPv6Config::IPv6Config(const QList<IPv6Address> &addresses,
        const QList<QHostAddress> &nameservers,
        const QStringList &domains,
        const QList<IPv6Route> &routes)
: d(new Private(addresses, nameservers, domains, routes))
{
}

NetworkManager::IPv6Config::IPv6Config() :
    d(new Private)
{
}

NetworkManager::IPv6Config::IPv6Config(const NetworkManager::IPv6Config& other) :
    d(new Private)
{
    *this = other;
}

NetworkManager::IPv6Config::~IPv6Config()
{
    delete d;
}

QList<NetworkManager::IPv6Address> NetworkManager::IPv6Config::addresses() const
{
    return d->addresses;
}

QList<QHostAddress> NetworkManager::IPv6Config::nameservers() const
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

