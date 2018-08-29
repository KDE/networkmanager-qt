/*
    Copyright 2018 Aleksander Morgado <aleksander@aleksander.es>

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

#include <arpa/inet.h>

#include "dnsdomain.h"

namespace NetworkManager
{

class NetworkManager::DnsDomain::Private
{
public:
    Private(const QString &theName,
            const QList<QHostAddress> &theServers,
            const QStringList &theOptions)
        : name(theName)
        , servers(theServers)
        , options(theOptions)
    {}
    Private()
    {}
    QString name;
    QList<QHostAddress> servers;
    QStringList options;
};

}

NetworkManager::DnsDomain::DnsDomain(const QString &name,
                                     const QList<QHostAddress> &servers,
                                     const QStringList &options)
    : d(new Private(name, servers, options))
{
}

NetworkManager::DnsDomain::DnsDomain()
    : d(new Private())
{
}

NetworkManager::DnsDomain::DnsDomain(const DnsDomain &other)
    : d(new Private)
{
    *this = other;
}

NetworkManager::DnsDomain::~DnsDomain()
{
    delete d;
}

QString NetworkManager::DnsDomain::name() const
{
    return d->name;
}

void NetworkManager::DnsDomain::setName(const QString &name)
{
    d->name = name;
}

QList<QHostAddress> NetworkManager::DnsDomain::servers() const
{
    return d->servers;
}

void NetworkManager::DnsDomain::setServers(const QList<QHostAddress> &servers)
{
    d->servers = servers;
}

QStringList NetworkManager::DnsDomain::options() const
{
    return d->options;
}

void NetworkManager::DnsDomain::setOptions(const QStringList &options)
{
    d->options = options;
}

NetworkManager::DnsDomain &NetworkManager::DnsDomain::operator=(const DnsDomain &other)
{
    if (this == &other) {
        return *this;
    }

    *d = *other.d;
    return *this;
}
