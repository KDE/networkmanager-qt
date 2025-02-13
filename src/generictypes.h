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

/*!
 * \typedef UIntListList
 */
typedef QList<QList<uint>> UIntListList;
Q_DECLARE_METATYPE(UIntListList)

/*!
 * \typedef UIntList
 */
typedef QList<uint> UIntList;
Q_DECLARE_METATYPE(UIntList)

/*!
 * \typedef NMVariantMapMap
 */
typedef QMap<QString, QVariantMap> NMVariantMapMap;
Q_DECLARE_METATYPE(NMVariantMapMap)

/*!
 * \typedef NMVariantMapList
 */
typedef QList<QVariantMap> NMVariantMapList;
Q_DECLARE_METATYPE(NMVariantMapList)

/*!
 * \typedef NMStringMap
 */
typedef QMap<QString, QString> NMStringMap;
/*!
 * \typedef NMStringMapIterator
 */
typedef QMapIterator<QString, QString> NMStringMapIterator;
Q_DECLARE_METATYPE(NMStringMap)

NETWORKMANAGERQT_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const NMStringMap &mydict);
NETWORKMANAGERQT_EXPORT const QDBusArgument &operator>>(const QDBusArgument &argument, NMStringMap &mydict);

/*!
 * \typedef IpV6DBusAddress
 */
typedef struct {
    QByteArray address;
    uint prefix;
    QByteArray gateway;
} IpV6DBusAddress;

Q_DECLARE_METATYPE(IpV6DBusAddress)
/*!
 * \typedef IpV6DBusAddressList
 */
typedef QList<IpV6DBusAddress> IpV6DBusAddressList;
Q_DECLARE_METATYPE(IpV6DBusAddressList)

NETWORKMANAGERQT_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const IpV6DBusAddress &address);
NETWORKMANAGERQT_EXPORT const QDBusArgument &operator>>(const QDBusArgument &argument, IpV6DBusAddress &address);

/*!
 * \typedef IpV6DBusRoute
 */
typedef struct {
    QByteArray destination;
    uint prefix;
    QByteArray nexthop;
    uint metric;
} IpV6DBusRoute;

Q_DECLARE_METATYPE(IpV6DBusRoute)
/*!
 * \typedef IpV6DBusRouteList
 */
typedef QList<IpV6DBusRoute> IpV6DBusRouteList;
Q_DECLARE_METATYPE(IpV6DBusRouteList)

NETWORKMANAGERQT_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const IpV6DBusRoute &route);
NETWORKMANAGERQT_EXPORT const QDBusArgument &operator>>(const QDBusArgument &argument, IpV6DBusRoute &route);

// ipv6 nameservers
/*!
 * \typedef IpV6DBusNameservers
 */
typedef QList<QByteArray> IpV6DBusNameservers;
Q_DECLARE_METATYPE(IpV6DBusNameservers)

/*!
 * \typedef DeviceDBusStateReason
 */
typedef struct {
    uint state;
    uint reason;
} DeviceDBusStateReason;
Q_DECLARE_METATYPE(DeviceDBusStateReason)

NETWORKMANAGERQT_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const DeviceDBusStateReason &reason);
NETWORKMANAGERQT_EXPORT const QDBusArgument &operator>>(const QDBusArgument &argument, DeviceDBusStateReason &reason);

#endif // GENERIC_TYPES_H
