/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2008 Will Stephenson <wstephenson@kde.org>
    SPDX-FileCopyrightText: 2013 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef GENERIC_TYPES_H
#define GENERIC_TYPES_H

#include <networkmanagerqt/networkmanagerqt_export.h>

#include <QDBusArgument>
#include <QVariantMap>

typedef QList<QList<uint>> UIntListList;

typedef QList<uint> UIntList;

typedef QMap<QString, QVariantMap> NMVariantMapMap;

typedef QList<QVariantMap> NMVariantMapList;

typedef QMap<QString, QString> NMStringMap;
typedef QMapIterator<QString, QString> NMStringMapIterator;

NETWORKMANAGERQT_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const NMStringMap &mydict);
NETWORKMANAGERQT_EXPORT const QDBusArgument &operator>>(const QDBusArgument &argument, NMStringMap &mydict);

typedef struct {
    QByteArray address;
    uint prefix;
    QByteArray gateway;
} IpV6DBusAddress;

Q_DECLARE_METATYPE(IpV6DBusAddress)
typedef QList<IpV6DBusAddress> IpV6DBusAddressList;

NETWORKMANAGERQT_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const IpV6DBusAddress &address);
NETWORKMANAGERQT_EXPORT const QDBusArgument &operator>>(const QDBusArgument &argument, IpV6DBusAddress &address);

typedef struct {
    QByteArray destination;
    uint prefix;
    QByteArray nexthop;
    uint metric;
} IpV6DBusRoute;

Q_DECLARE_METATYPE(IpV6DBusRoute)
typedef QList<IpV6DBusRoute> IpV6DBusRouteList;

NETWORKMANAGERQT_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const IpV6DBusRoute &route);
NETWORKMANAGERQT_EXPORT const QDBusArgument &operator>>(const QDBusArgument &argument, IpV6DBusRoute &route);

// ipv6 nameservers
typedef QList<QByteArray> IpV6DBusNameservers;

typedef struct {
    uint state;
    uint reason;
} DeviceDBusStateReason;
Q_DECLARE_METATYPE(DeviceDBusStateReason)

NETWORKMANAGERQT_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const DeviceDBusStateReason &reason);
NETWORKMANAGERQT_EXPORT const QDBusArgument &operator>>(const QDBusArgument &argument, DeviceDBusStateReason &reason);

#endif // GENERIC_TYPES_H
