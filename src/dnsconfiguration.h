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

#ifndef NETWORKMANAGERQT_DNSCONFIGURATION_H
#define NETWORKMANAGERQT_DNSCONFIGURATION_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "dnsdomain.h"

// To prevent signals in glib2 be defined by QT
#undef signals
#include <nm-version.h>
#include <libnm/NetworkManager.h>
#define signals Q_SIGNALS

#include <QStringList>

namespace NetworkManager
{

/**
 * This class represents IP configuration
 */
class NETWORKMANAGERQT_EXPORT DnsConfiguration
{
public:

    /**
     * Constructs an initialized DnsConfiguration object
     */
    DnsConfiguration(const QStringList &searches,
                     const QStringList &options,
                     const QList<DnsDomain> domains);

    /**
     * Constructs an empty DnsConfiguration object
     */
    DnsConfiguration();

    /**
     * Destroys this DnsConfiguration object.
     */
    ~DnsConfiguration();

    /**
     * Constructs a DnsConfiguration object that is a copy of the object @p other.
     */
    DnsConfiguration(const DnsConfiguration &other);

    /**
     * Returns the list of search domains
     */
    QStringList searches() const;

    /**
     * Sets the list of search domains
     */
    void setSearches(const QStringList &list);

    /**
     * Returns the list of resolver options
     */
    QStringList options() const;

    /**
     * Sets the list of resolver options
     */
    void setOptions(const QStringList &list);

    /**
     * Returns the list of domains
     */
    QList<DnsDomain> domains() const;

    /**
     * Sets the list of domains
     */
    void setDomains(const QList<DnsDomain> &domains);

    /**
     * Marshall into a map
     */
    QVariantMap toMap() const;

    /**
     * De-marshall from a map
     */
    void fromMap (const QVariantMap &map);

    /**
     * Makes a copy of the DnsConfiguration object @p other.
     */
    DnsConfiguration &operator=(const DnsConfiguration &other);

private:
    class Private;
    Private *const d;
};

} // namespace NetworkManager

#endif // NETWORKMANAGERQT_DNSCONFIGURATION_H
