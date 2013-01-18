/*
* Copyright 2011 Ilia Kats <ilia-kats@gmx.net>, based on work by Will Stephenson <wstephenson@kde.org>
* Copyright 2011 Will Stephenson <wstephenson@kde.org>

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

#ifndef NMQT_IPV6CONFIG_H
#define NMQT_IPV6CONFIG_H

#include "QtNetworkManager-export.h"

#include <QtCore/QStringList>
#include <QHostAddress>

namespace NetworkManager
{

class NMQT_EXPORT IPv6Address
{
public:
    IPv6Address(Q_IPV6ADDR address, quint32 netMask, Q_IPV6ADDR gateway);
    IPv6Address();
    ~IPv6Address();
    IPv6Address(const IPv6Address&);
    Q_IPV6ADDR address() const;
    quint32 netMask() const;
    Q_IPV6ADDR gateway() const;
    IPv6Address &operator=(const IPv6Address&);
    bool isValid() const;
private:
    class Private;
    Private * d;
};

class NMQT_EXPORT IPv6Route
{
public:
    IPv6Route(Q_IPV6ADDR route, quint32 prefix, Q_IPV6ADDR nextHop, quint32 metric);
    IPv6Route();
    ~IPv6Route();
    IPv6Route(const IPv6Route&);
    IPv6Route &operator=(const IPv6Route&);
    bool isValid() const;
    Q_IPV6ADDR route() const;
    quint32 prefix() const;
    Q_IPV6ADDR nextHop() const;
    quint32 metric() const;
private:
    class Private;
    Private * d;
};

class NMQT_EXPORT IPv6Config
{
public:
    IPv6Config(const QList<IPv6Address> &addresses,
        const QList<Q_IPV6ADDR> &nameservers,
        const QStringList &domains,
        const QList<IPv6Route> &routes);
    IPv6Config();
    ~IPv6Config();
    IPv6Config(const IPv6Config&);
    /**
     * List of IP addresses related to this configuration.
     */
    QList<IPv6Address> addresses() const;
    QList<Q_IPV6ADDR> nameservers() const;
    QStringList domains() const;
    QList<IPv6Route> routes() const;
    IPv6Config &operator=(const IPv6Config&);
    bool isValid() const;
private:
    class Private;
    Private * d;
};

} // namespace NetworkManager

#endif // NMQT__IPV6CONFIG_H
