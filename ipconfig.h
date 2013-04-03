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
    IpAddress();
    ~IpAddress();
    IpAddress(const IpAddress&);
    bool isValid() const;
    void setGateway(const QHostAddress &gateway);
    QHostAddress gateway() const;
    IpAddress &operator=(const IpAddress&);
private:
    class Private;
    Private * d;
};
typedef QList<IpAddress> IpAddresses;

class NMQT_EXPORT IpRoute : public QNetworkAddressEntry
{
public:
    IpRoute();
    ~IpRoute();
    IpRoute(const IpRoute&);
    IpRoute &operator=(const IpRoute&);
    bool isValid() const;
    void setNextHop(const QHostAddress &nextHop) const;
    QHostAddress nextHop() const;
    void setMetric(quint32 metric);
    quint32 metric() const;
private:
    class Private;
    Private * d;
};
typedef QList<IpRoute> IpRoutes;

class NMQT_EXPORT IpConfig
{
public:
    IpConfig(const IpAddresses &addresses,
        const QList<QHostAddress> &nameservers,
        const QStringList &domains,
        const IpRoutes &routes);
    IpConfig();
    ~IpConfig();
    IpConfig(const IpConfig&);
    /**
     * List of IP addresses related to this configuration.
     */
    IpAddresses addresses() const;
    QList<QHostAddress> nameservers() const;
    QStringList domains() const;
    IpRoutes routes() const;
    IpConfig &operator=(const IpConfig&);
    bool isValid() const;
private:
    class Private;
    Private * d;
};

} // namespace NetworkManager

#endif // NMQT__IpConfig_H
