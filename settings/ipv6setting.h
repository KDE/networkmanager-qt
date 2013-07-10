/*
    Copyright 2012-2013  Jan Grulich <jgrulich@redhat.com>

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

#ifndef NMQT_SETTINGS_IPV6_H
#define NMQT_SETTINGS_IPV6_H

#include "../ipconfig.h"
#include "../NetworkManagerQt-export.h"
#include "setting.h"

#include <QtCore/QStringList>
#include <QtNetwork/QHostAddress>

namespace NetworkManager
{

class Ipv6SettingPrivate;
class NETWORKMANAGERQT_EXPORT Ipv6Setting : public Setting
{
public:
    typedef QSharedPointer<Ipv6Setting> Ptr;
    typedef QList<Ptr> List;
    enum ConfigMethod {Automatic, Dhcp, LinkLocal, Manual, Ignored};
    enum IPv6Privacy {Unknown = -1, Disabled, PreferPublic, PreferTemporary};

    Ipv6Setting();
    explicit Ipv6Setting(const Ptr &other);
    ~Ipv6Setting();

    QString name() const;

    void setMethod(ConfigMethod method);
    ConfigMethod method() const;

    void setDns(const QList<QHostAddress> &dns);
    QList<QHostAddress> dns() const;

    void setDnsSearch(const QStringList &domains);
    QStringList dnsSearch() const;

    void setAddresses(const QList<NetworkManager::IpAddress> ipv6addresses);
    QList<NetworkManager::IpAddress> addresses() const;

    void setRoutes(const QList<NetworkManager::IpRoute> ipv6routes);
    QList<NetworkManager::IpRoute> routes() const;

    void setIgnoreAutoRoutes(bool ignore);
    bool ignoreAutoRoutes() const;

    void setIgnoreAutoDns(bool ignore);
    bool ignoreAutoDns() const;

    void setNeverDefault(bool neverDefault);
    bool neverDefault() const;

    void setMayFail(bool mayFail);
    bool mayFail() const;

    void setPrivacy(IPv6Privacy privacy);
    IPv6Privacy privacy() const;

    void fromMap(const QVariantMap &setting);

    QVariantMap toMap() const;

protected:
    Ipv6SettingPrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(Ipv6Setting)
};

QDebug operator<<(QDebug dbg, const Ipv6Setting &setting);

}

#endif // NMQT_SETTINGS_IPV6_H
