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

#include "ipaddress.h"

namespace NetworkManager {

class IpAddress::Private
{
public:
    QHostAddress gateway;
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
