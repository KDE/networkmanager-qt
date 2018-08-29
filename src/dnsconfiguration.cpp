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

#include "dnsconfiguration.h"

namespace NetworkManager
{

class NetworkManager::DnsConfiguration::Private
{
public:
    Private(const QStringList &theSearches,
            const QStringList &theOptions,
            const QList<DnsDomain> theDomains)
        : searches(theSearches)
        , options(theOptions)
        , domains(theDomains)
    {}
    Private()
    {}
    QStringList searches;
    QStringList options;
    QList<DnsDomain> domains;
};

}

NetworkManager::DnsConfiguration::DnsConfiguration(const QStringList &searches,
                                                   const QStringList &options,
                                                   const QList<DnsDomain> domains)
    : d(new Private(searches, options, domains))
{
}

NetworkManager::DnsConfiguration::DnsConfiguration()
    : d(new Private())
{
}

NetworkManager::DnsConfiguration::DnsConfiguration(const DnsConfiguration &other)
    : d(new Private)
{
    *this = other;
}

NetworkManager::DnsConfiguration::~DnsConfiguration()
{
    delete d;
}

QStringList NetworkManager::DnsConfiguration::searches() const
{
    return d->searches;
}

void NetworkManager::DnsConfiguration::setSearches(const QStringList &searches)
{
    d->searches = searches;
}

QStringList NetworkManager::DnsConfiguration::options() const
{
    return d->options;
}

void NetworkManager::DnsConfiguration::setOptions(const QStringList &options)
{
    d->options = options;
}

QList<NetworkManager::DnsDomain> NetworkManager::DnsConfiguration::domains() const
{
    return d->domains;
}

void NetworkManager::DnsConfiguration::setDomains(const QList<NetworkManager::DnsDomain> &domains)
{
    d->domains = domains;
}

QVariantMap NetworkManager::DnsConfiguration::toMap() const
{
    QVariantMap map;

    map["searches"] = d->searches;
    map["options"] = d->options;

    QVariantMap domains;
    Q_FOREACH (const NetworkManager::DnsDomain &domain, d->domains) {
        QVariantMap contents;
        QStringList serversList;
        Q_FOREACH (const QHostAddress &address, domain.servers())
            serversList.append(address.toString());
        contents["servers"] = serversList;
        contents["options"] = domain.options();
        domains[domain.name()] = contents;
    }
    map["domains"] = domains;

    return map;
}

void NetworkManager::DnsConfiguration::fromMap (const QVariantMap &map)
{
    d->searches = map["searches"].toStringList();
    d->options = map["options"].toStringList();
    d->domains = QList<NetworkManager::DnsDomain>();

    QVariantMap domains = map["domains"].toMap();
    QVariantMap::const_iterator i = domains.constBegin();
    while (i != domains.constEnd()) {
        QVariantMap contents = i.value().toMap();
        QList<QHostAddress> addressList;
        Q_FOREACH (const QString &server, contents["servers"].toStringList())
            addressList.append(QHostAddress(server));
        NetworkManager::DnsDomain domain(i.key(),
                                         addressList,
                                         contents["options"].toStringList());
        d->domains.append(domain);
        ++i;
    }
}

NetworkManager::DnsConfiguration &NetworkManager::DnsConfiguration::operator=(const DnsConfiguration &other)
{
    if (this == &other) {
        return *this;
    }

    *d = *other.d;
    return *this;
}
