/* This file is part of the KDE project
   Copyright 2008 Will Stephenson <wstephenson@kde.org>

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

#ifndef GENERIC_TYPES_H
#define GENERIC_TYPES_H

#include <QtCore/QVariantMap>
#include <QDBusArgument>


typedef QList<QList<uint> > UIntListList;
Q_DECLARE_METATYPE(UIntListList)
typedef QList<uint> UIntList;
Q_DECLARE_METATYPE(UIntList)
Q_DECLARE_METATYPE(QVariantMap)
typedef QMap<QString,QVariantMap> QVariantMapMap;
Q_DECLARE_METATYPE(QVariantMapMap)
typedef QMap<QString,QString> QStringMap;
typedef QMapIterator<QString, QString> QStringMapIterator;
Q_DECLARE_METATYPE(QStringMap)
typedef enum {Unknown = 0, Adhoc, Infra, Ap} WirelessDeviceMode;

QDBusArgument &operator<<(QDBusArgument &argument, const QStringMap & mydict);
const QDBusArgument &operator>>(const QDBusArgument &argument, QStringMap & mydict);

typedef struct
{
    QByteArray address;
    uint netMask;
    QByteArray gateway;
} IpV6DBusAddress;

Q_DECLARE_METATYPE(IpV6DBusAddress)
typedef QList<IpV6DBusAddress> IpV6DBusAddressList;
Q_DECLARE_METATYPE(IpV6DBusAddressList)

QDBusArgument &operator<<(QDBusArgument &argument, const IpV6DBusAddress &address);
const QDBusArgument &operator>>(const QDBusArgument &argument, IpV6DBusAddress &address);

typedef struct
{
    QByteArray destination;
    uint prefix;
    QByteArray nexthop;
    uint metric;
} IpV6DBusRoute;

Q_DECLARE_METATYPE(IpV6DBusRoute)
typedef QList<IpV6DBusRoute> IpV6DBusRouteList;
Q_DECLARE_METATYPE(IpV6DBusRouteList)

QDBusArgument &operator<<(QDBusArgument &argument, const IpV6DBusRoute &route);
const QDBusArgument &operator>>(const QDBusArgument &argument, IpV6DBusRoute &route);

// ipv6 nameservers
typedef QList<QByteArray> IpV6DBusNameservers;
Q_DECLARE_METATYPE(IpV6DBusNameservers)

typedef struct
{
    uint state;
    uint reason;
} DeviceDBusStateReason;
Q_DECLARE_METATYPE(DeviceDBusStateReason)

QDBusArgument &operator<<(QDBusArgument &argument, const DeviceDBusStateReason &reason);
const QDBusArgument &operator>>(const QDBusArgument &argument, DeviceDBusStateReason &reason);

#endif // GENERIC_TYPES_H
