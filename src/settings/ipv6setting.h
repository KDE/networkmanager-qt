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

#ifndef NETWORKMANAGERQT_IPV6_SETTING_H
#define NETWORKMANAGERQT_IPV6_SETTING_H

#include "ipconfig.h"
#include <networkmanagerqt/networkmanagerqt_export.h>
#include "setting.h"

#include <QtCore/QStringList>
#include <QtNetwork/QHostAddress>

#if NM_CHECK_VERSION(1, 0, 0)
#define NMQT_SETTING_IP6_CONFIG_SETTING_NAME       NM_SETTING_IP6_CONFIG_SETTING_NAME
#define NMQT_SETTING_IP6_CONFIG_METHOD             NM_SETTING_IP_CONFIG_METHOD
#define NMQT_SETTING_IP6_CONFIG_DNS                NM_SETTING_IP_CONFIG_DNS
#define NMQT_SETTING_IP6_CONFIG_DNS_SEARCH         NM_SETTING_IP_CONFIG_DNS_SEARCH
#define NMQT_SETTING_IP6_CONFIG_ADDRESSES          NM_SETTING_IP_CONFIG_ADDRESSES
#define NMQT_SETTING_IP6_CONFIG_GATEWAY            NM_SETTING_IP_CONFIG_GATEWAY
#define NMQT_SETTING_IP6_CONFIG_ROUTES             NM_SETTING_IP_CONFIG_ROUTES
#define NMQT_SETTING_IP6_CONFIG_ROUTE_METRIC       NM_SETTING_IP_CONFIG_ROUTE_METRIC
#define NMQT_SETTING_IP6_CONFIG_IGNORE_AUTO_ROUTES NM_SETTING_IP_CONFIG_IGNORE_AUTO_ROUTES
#define NMQT_SETTING_IP6_CONFIG_IGNORE_AUTO_DNS    NM_SETTING_IP_CONFIG_IGNORE_AUTO_DNS
#define NMQT_SETTING_IP6_CONFIG_DHCP_HOSTNAME      NM_SETTING_IP_CONFIG_DHCP_HOSTNAME
#define NMQT_SETTING_IP6_CONFIG_DHCP_SEND_HOSTNAME NM_SETTING_IP_CONFIG_DHCP_SEND_HOSTNAME
#define NMQT_SETTING_IP6_CONFIG_NEVER_DEFAULT      NM_SETTING_IP_CONFIG_NEVER_DEFAULT
#define NMQT_SETTING_IP6_CONFIG_MAY_FAIL           NM_SETTING_IP_CONFIG_MAY_FAIL
#define NMQT_SETTING_IP6_CONFIG_DHCP_CLIENT_ID     NM_SETTING_IP6_CONFIG_DHCP_CLIENT_ID
#define NMQT_SETTING_IP6_CONFIG_IP6_PRIVACY        NM_SETTING_IP6_CONFIG_IP6_PRIVACY
#define NMQT_SETTING_IP6_CONFIG_METHOD_IGNORE      NM_SETTING_IP6_CONFIG_METHOD_IGNORE
#define NMQT_SETTING_IP6_CONFIG_METHOD_AUTO        NM_SETTING_IP6_CONFIG_METHOD_AUTO
#define NMQT_SETTING_IP6_CONFIG_METHOD_DHCP        NM_SETTING_IP6_CONFIG_METHOD_DHCP
#define NMQT_SETTING_IP6_CONFIG_METHOD_LINK_LOCAL  NM_SETTING_IP6_CONFIG_METHOD_LINK_LOCAL
#define NMQT_SETTING_IP6_CONFIG_METHOD_MANUAL      NM_SETTING_IP6_CONFIG_METHOD_MANUAL
#define NMQT_SETTING_IP6_CONFIG_METHOD_SHARED      NM_SETTING_IP6_CONFIG_METHOD_SHARED
#else
#define NMQT_SETTING_IP6_CONFIG_SETTING_NAME       NM_SETTING_IP6_CONFIG_SETTING_NAME
#define NMQT_SETTING_IP6_CONFIG_METHOD             NM_SETTING_IP6_CONFIG_METHOD
#define NMQT_SETTING_IP6_CONFIG_DNS                NM_SETTING_IP6_CONFIG_DNS
#define NMQT_SETTING_IP6_CONFIG_DNS_SEARCH         NM_SETTING_IP6_CONFIG_DNS_SEARCH
#define NMQT_SETTING_IP6_CONFIG_ADDRESSES          NM_SETTING_IP6_CONFIG_ADDRESSES
#define NMQT_SETTING_IP6_CONFIG_GATEWAY            NM_SETTING_IP6_CONFIG_GATEWAY
#define NMQT_SETTING_IP6_CONFIG_ROUTES             NM_SETTING_IP6_CONFIG_ROUTES
#define NMQT_SETTING_IP6_CONFIG_ROUTE_METRIC       NM_SETTING_IP6_CONFIG_ROUTE_METRIC
#define NMQT_SETTING_IP6_CONFIG_IGNORE_AUTO_ROUTES NM_SETTING_IP6_CONFIG_IGNORE_AUTO_ROUTES
#define NMQT_SETTING_IP6_CONFIG_IGNORE_AUTO_DNS    NM_SETTING_IP6_CONFIG_IGNORE_AUTO_DNS
#define NMQT_SETTING_IP6_CONFIG_DHCP_HOSTNAME      NM_SETTING_IP6_CONFIG_DHCP_HOSTNAME
#define NMQT_SETTING_IP6_CONFIG_DHCP_SEND_HOSTNAME NM_SETTING_IP6_CONFIG_DHCP_SEND_HOSTNAME
#define NMQT_SETTING_IP6_CONFIG_NEVER_DEFAULT      NM_SETTING_IP6_CONFIG_NEVER_DEFAULT
#define NMQT_SETTING_IP6_CONFIG_MAY_FAIL           NM_SETTING_IP6_CONFIG_MAY_FAIL
#define NMQT_SETTING_IP6_CONFIG_DHCP_CLIENT_ID     NM_SETTING_IP6_CONFIG_DHCP_CLIENT_ID
#define NMQT_SETTING_IP6_CONFIG_IP6_PRIVACY        NM_SETTING_IP6_CONFIG_IP6_PRIVACY
#define NMQT_SETTING_IP6_CONFIG_METHOD_IGNORE      NM_SETTING_IP6_CONFIG_METHOD_IGNORE
#define NMQT_SETTING_IP6_CONFIG_METHOD_AUTO        NM_SETTING_IP6_CONFIG_METHOD_AUTO
#define NMQT_SETTING_IP6_CONFIG_METHOD_DHCP        NM_SETTING_IP6_CONFIG_METHOD_DHCP
#define NMQT_SETTING_IP6_CONFIG_METHOD_LINK_LOCAL  NM_SETTING_IP6_CONFIG_METHOD_LINK_LOCAL
#define NMQT_SETTING_IP6_CONFIG_METHOD_MANUAL      NM_SETTING_IP6_CONFIG_METHOD_MANUAL
#define NMQT_SETTING_IP6_CONFIG_METHOD_SHARED      NM_SETTING_IP6_CONFIG_METHOD_SHARED
#endif

namespace NetworkManager
{

class Ipv6SettingPrivate;

/**
 * Represents ipv6 setting
 */
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

    QString name() const Q_DECL_OVERRIDE;

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

    void setRouteMetric(int metric);
    int routeMetric() const;

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

    void fromMap(const QVariantMap &setting) Q_DECL_OVERRIDE;

    QVariantMap toMap() const Q_DECL_OVERRIDE;

protected:
    Ipv6SettingPrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(Ipv6Setting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const Ipv6Setting &setting);

}

#endif // NETWORKMANAGERQT_IPV6_SETTING_H
