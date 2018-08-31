/*
    Copyright 2018 Aleksander Morgado <aleksander@aleksander.es>

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

#ifndef NETWORKMANAGERQT_DNSDOMAIN_H
#define NETWORKMANAGERQT_DNSDOMAIN_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "ipaddress.h"
#include "iproute.h"

// To prevent signals in glib2 be defined by QT
#undef signals
#include <nm-version.h>
#include <libnm/NetworkManager.h>
#define signals Q_SIGNALS

#include <QtCore/QStringList>
#include <QNetworkAddressEntry>

namespace NetworkManager
{

/**
 * This class represents the configuration for a DNS domain
 */
class NETWORKMANAGERQT_EXPORT DnsDomain
{
public:

    /**
     * Constructs a DnsDomain object with a list of
     */
    DnsDomain(const QString &name,
              const QList<QHostAddress> &servers,
              const QStringList &options);

    /**
     * Constructs a DnsDomain object
     */
    DnsDomain();

    /**
     * Destroys this DnsDomain object.
     */
    ~DnsDomain();

    /**
     * Constructs a DnsDomain object that is a copy of the object @p other.
     */
    DnsDomain(const DnsDomain &other);

    /**
     * Returns the domain name
     */
    QString name() const;

    /**
     * Sets the domain name
     */
    void setName(const QString &name);

    /**
     * Returns the list of servers
     */
    QList<QHostAddress> servers() const;

    /**
     * Sets the list of servers
     */
    void setServers(const QList<QHostAddress> &list);

    /**
     * Returns the list of resolver options
     */
    QStringList options() const;

    /**
     * Sets the list of resolver options
     */
    void setOptions(const QStringList &list);

    /**
     * Makes a copy of the DnsDomain object @p other.
     */
    DnsDomain &operator=(const DnsDomain &other);

private:
    class Private;
    Private *const d;
};

} // namespace NetworkManager

#endif // NETWORKMANAGERQT_DNSDOMAIN_H
