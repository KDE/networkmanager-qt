/*
* Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
* Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>

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

#ifndef NMQT_IPADDRESS_H
#define NMQT_IPADDRESS_H

#include "NetworkManagerQt-export.h"

#include <QtCore/QStringList>
#include <QNetworkAddressEntry>

namespace NetworkManager
{

class NETWORKMANAGERQT_EXPORT IpAddress : public QNetworkAddressEntry
{
public:
    /**
     * Constructs an empty IpAddress object.
     */
    IpAddress();

    /**
     * Constructs a IpAddress object that is a copy of the object other.
     */
    IpAddress(const IpAddress &other);

    /**
     * Destroys this IpRoute object.
     */
    ~IpAddress();

    /**
     * Return if the IP address is defined.
     */
    bool isValid() const;

    /**
     * Defines the default gateway of this object.
     */
    void setGateway(const QHostAddress &gateway);

    /**
     * Returns the default gateway of this object.
     */
    QHostAddress gateway() const;

    /**
     * Makes a copy of the IpAddress object other.
     */
    IpAddress &operator=(const IpAddress &other);

private:
    class Private;
    Private *const d;
};
typedef QList<IpAddress> IpAddresses;

} // namespace NetworkManager

#endif // NMQT_IPADDRESS_H
