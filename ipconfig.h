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

#include "QtNetworkManager-export.h"

#include <QtCore/QStringList>
#include <QNetworkAddressEntry>

namespace NetworkManager
{

class NMQT_EXPORT IpAddress : public QNetworkAddressEntry
{
public:
    /**
     * Constructs an empty IpAddress object.
     */
    IpAddress();

    /**
     * Constructs a IpAddress object that is a copy of the object other.
     */
    IpAddress(const IpAddress&);

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
    IpAddress &operator=(const IpAddress&);

private:
    class Private;
    Private * d;
};
typedef QList<IpAddress> IpAddresses;

class NMQT_EXPORT IpRoute : public QNetworkAddressEntry
{
public:
    /**
     * Constructs an empty IpRoute object.
     */
    IpRoute();

    /**
     * Constructs a IpRoute object that is a copy of the object other.
     */
    IpRoute(const IpRoute&);

    /**
     * Destroys this IpRoute object.
     */
    ~IpRoute();

    /**
     * Returns true if the route IP is defined.
     */
    bool isValid() const;

    /**
     * Defines the next hop of the given route.
     */
    void setNextHop(const QHostAddress &nextHop) const;

    /**
     * Returns the next hop of the given route.
     */
    QHostAddress nextHop() const;

    /**
     * Defines the metric of the given route,
     * lower values have higher priority.
     */
    void setMetric(quint32 metric);

    /**
     * Returns the route metric number of the given route.
     */
    quint32 metric() const;

    /**
     * Makes a copy of the IpRoute object other.
     */
    IpRoute &operator=(const IpRoute&);

private:
    class Private;
    Private * d;
};
typedef QList<IpRoute> IpRoutes;

class NMQT_EXPORT IpConfig
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
    IpConfig(const IpConfig&);

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
    IpConfig &operator=(const IpConfig&);

    /**
     * Returns false if the list of IP Addresses is empty
     */
    bool isValid() const;

private:
    class Private;
    Private * d;
};

} // namespace NetworkManager

#endif // NMQT__IpConfig_H
