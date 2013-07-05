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

#ifndef NMQT_IPROUTE_H
#define NMQT_IPROUTE_H

#include "NetworkManagerQt-export.h"
#include "ipaddress.h"

#include <QtCore/QStringList>
#include <QNetworkAddressEntry>

namespace NetworkManager
{

class NETWORKMANAGERQT_EXPORT IpRoute : public QNetworkAddressEntry
{
public:
    /**
     * Constructs an empty IpRoute object.
     */
    IpRoute();

    /**
     * Constructs an IpRoute object that is a copy of the object @p other.
     */
    IpRoute(const IpRoute &other);

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
     * Makes a copy of the IpRoute object @p other.
     */
    IpRoute &operator=(const IpRoute &other);

private:
    class Private;
    Private *const d;
};
typedef QList<IpRoute> IpRoutes;

} // namespace NetworkManager

#endif // NMQT_IPROUTE_H
