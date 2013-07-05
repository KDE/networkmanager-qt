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

#ifndef NMQT_IPCONFIG_H
#define NMQT_IPCONFIG_H

#include "NetworkManagerQt-export.h"
#include "ipaddress.h"
#include "iproute.h"

#include <QtCore/QStringList>
#include <QNetworkAddressEntry>

namespace NetworkManager
{

class NETWORKMANAGERQT_EXPORT IpConfig
{
public:
    /**
     * Constructs a ip config object with a list of address, nameservers, domains and routers.
     */
    IpConfig(const IpAddresses &addresses,
             const QList<QHostAddress> &nameservers,
             const QStringList &domains,
             const IpRoutes &routes);

    /**
     * Constructs an empty IpConfig object.
     */
    IpConfig();

    /**
     * Destroys this IpConfig object.
     */
    ~IpConfig();

    /**
     * Constructs a IpConfig object that is a copy of the object other.
     */
    IpConfig(const IpConfig &other);

    /**
     * Configure this class using the information on the following path
     */
    void setIPv4Path(const QString &path);

    /**
     * Configure this class using the information on the following path
     */
    void setIPv6Path(const QString &path);

    /**
     * Returns a list of IP addresses related to this configuration.
     */
    NetworkManager::IpAddresses addresses() const;

    /**
     * Returns a list of nameservers related to this configuration.
     */
    QList<QHostAddress> nameservers() const;

    /**
     * Returns a list of domains related to this configuration.
     */
    QStringList domains() const;

    /**
     * Returns a list of routes related to this configuration.
     */
    IpRoutes routes() const;

    /**
     * Makes a copy of the IpConfig object other.
     */
    IpConfig &operator=(const IpConfig &other);

    /**
     * Returns false if the list of IP Addresses is empty
     */
    bool isValid() const;

private:
    class Private;
    Private *const d;
};

} // namespace NetworkManager

#endif // NMQT_IPCONFIG_H
