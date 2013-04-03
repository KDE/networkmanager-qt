/*
* Copyright 2011 Ilia Kats <ilia-kats@gmx.net>, based on work by Will Stephenson <wstephenson@kde.org>
* Copyright 2011 Will Stephenson <wstephenson@kde.org>
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

#ifndef NMQT_IPV6CONFIG_H
#define NMQT_IPV6CONFIG_H

#include "QtNetworkManager-export.h"

#include <QtCore/QStringList>
#include <QNetworkAddressEntry>

namespace NetworkManager
{

class NMQT_EXPORT IPv6Address : public QNetworkAddressEntry
{
public:
    IPv6Address();
    ~IPv6Address();
    IPv6Address(const IPv6Address&);
    void setGateway(const QHostAddress &gateway);
    QHostAddress gateway() const;
    IPv6Address &operator=(const IPv6Address&);
    bool isValid() const;
private:
    class Private;
    Private * d;
};

class NMQT_EXPORT IPv6Route : public QNetworkAddressEntry
{
public:
    IPv6Route();
    ~IPv6Route();
    IPv6Route(const IPv6Route&);
    IPv6Route &operator=(const IPv6Route&);
    bool isValid() const;
    void setNextHop(const QHostAddress &nextHop);
    QHostAddress nextHop() const;
    void setMetric(quint32 metric);
    quint32 metric() const;
private:
    class Private;
    Private * d;
};

class NMQT_EXPORT IPv6Config
{
public:
    IPv6Config(const QList<IPv6Address> &addresses,
        const QList<QHostAddress> &nameservers,
        const QStringList &domains,
        const QList<IPv6Route> &routes);
    IPv6Config();
    ~IPv6Config();
    IPv6Config(const IPv6Config&);
    /**
     * List of IP addresses related to this configuration.
     */
    QList<IPv6Address> addresses() const;
    QList<QHostAddress> nameservers() const;
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
