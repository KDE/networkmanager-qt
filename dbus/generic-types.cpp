/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>

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

#include "generic-types.h"

QDBusArgument &operator<<(QDBusArgument &argument, const IpV6DBusAddress &address)
{
    argument.beginStructure();
    argument << address.address << address.netMask << address.gateway;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, IpV6DBusAddress &address)
{
    argument.beginStructure();
    argument >> address.address >> address.netMask >> address.gateway;
    argument.endStructure();
    return argument;
}

QDBusArgument &operator<<(QDBusArgument &argument, const IpV6DBusRoute &route)
{
    argument.beginStructure();
    argument << route.destination << route.prefix << route.nexthop << route.metric;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, IpV6DBusRoute &route)
{
    argument.beginStructure();
    argument >> route.destination >> route.prefix >> route.nexthop >> route.metric;
    argument.endStructure();
    return argument;
}

QDBusArgument &operator<<(QDBusArgument &argument, const QStringMap & mydict)
{
    argument.beginMap( QVariant::String, QVariant::String );

    QMapIterator<QString, QString> i(mydict);
    while (i.hasNext()) {
        i.next();
        argument.beginMapEntry();
        argument << i.key() << i.value();
        argument.endMapEntry();
    }
    argument.endMap();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, QStringMap & mydict)
{
    argument.beginMap();
    mydict.clear();

    while (!argument.atEnd()) {
        QString key;
        QString value;
        argument.beginMapEntry();
        argument >> key >> value;
        argument.endMapEntry();
        mydict.insert(key, value);
    }

    argument.endMap();
    return argument;
}