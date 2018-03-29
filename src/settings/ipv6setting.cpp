/*
    Copyright 2012-2013  Jan Grulich <jgrulich@redhat.com>
    Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>

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

#include "ipv6setting.h"
#include "ipv6setting_p.h"

#include "utils.h"
#include "generictypes.h"
#include "../ipconfig.h"

#include <arpa/inet.h>

NetworkManager::Ipv6SettingPrivate::Ipv6SettingPrivate()
    : name(NMQT_SETTING_IP6_CONFIG_SETTING_NAME)
    , method(NetworkManager::Ipv6Setting::Automatic)
    , routeMetric(-1)
    , ignoreAutoRoutes(false)
    , ignoreAutoDns(false)
    , neverDefault(false)
    , mayFail(true)
    , privacy(NetworkManager::Ipv6Setting::Unknown)
{ }

NetworkManager::Ipv6Setting::Ipv6Setting()
    : Setting(Setting::Ipv6)
    , d_ptr(new Ipv6SettingPrivate())
{ }

NetworkManager::Ipv6Setting::Ipv6Setting(const Ptr &other)
    : Setting(other)
    , d_ptr(new Ipv6SettingPrivate())
{
    setMethod(other->method());
    setDns(other->dns());
    setDnsSearch(other->dnsSearch());
    setAddresses(other->addresses());
    setRoutes(other->routes());
    setRouteMetric(other->routeMetric());
    setIgnoreAutoRoutes(other->ignoreAutoRoutes());
    setIgnoreAutoDns(other->ignoreAutoDns());
    setNeverDefault(other->neverDefault());
    setMayFail(other->mayFail());
}

NetworkManager::Ipv6Setting::~Ipv6Setting()
{
    delete d_ptr;
}

QString NetworkManager::Ipv6Setting::name() const
{
    Q_D(const Ipv6Setting);

    return d->name;
}

void NetworkManager::Ipv6Setting::setMethod(NetworkManager::Ipv6Setting::ConfigMethod type)
{
    Q_D(Ipv6Setting);

    d->method = type;
}

NetworkManager::Ipv6Setting::ConfigMethod NetworkManager::Ipv6Setting::method() const
{
    Q_D(const Ipv6Setting);

    return d->method;
}

void NetworkManager::Ipv6Setting::setDns(const QList<QHostAddress> &dns)
{
    Q_D(Ipv6Setting);

    d->dns = dns;
}

QList<QHostAddress> NetworkManager::Ipv6Setting::dns() const
{
    Q_D(const Ipv6Setting);

    return d->dns;
}

void NetworkManager::Ipv6Setting::setDnsSearch(const QStringList &domains)
{
    Q_D(Ipv6Setting);

    d->dnsSearch = domains;
}

QStringList NetworkManager::Ipv6Setting::dnsSearch() const
{
    Q_D(const Ipv6Setting);

    return d->dnsSearch;
}

void NetworkManager::Ipv6Setting::setAddresses(const QList<IpAddress> ipv6addresses)
{
    Q_D(Ipv6Setting);

    d->addresses = ipv6addresses;
}

QList< NetworkManager::IpAddress > NetworkManager::Ipv6Setting::addresses() const
{
    Q_D(const Ipv6Setting);

    return d->addresses;
}

void NetworkManager::Ipv6Setting::setRoutes(const QList< NetworkManager::IpRoute > ipv6routes)
{
    Q_D(Ipv6Setting);

    d->routes = ipv6routes;
}

QList<NetworkManager::IpRoute> NetworkManager::Ipv6Setting::routes() const
{
    Q_D(const Ipv6Setting);

    return d->routes;
}

void NetworkManager::Ipv6Setting::setRouteMetric(int metric)
{
    Q_D(Ipv6Setting);

    d->routeMetric = metric;
}

int NetworkManager::Ipv6Setting::routeMetric() const
{
    Q_D(const Ipv6Setting);

    return d->routeMetric;
}

void NetworkManager::Ipv6Setting::setIgnoreAutoRoutes(bool ignore)
{
    Q_D(Ipv6Setting);

    d->ignoreAutoRoutes = ignore;
}

bool NetworkManager::Ipv6Setting::ignoreAutoRoutes() const
{
    Q_D(const Ipv6Setting);

    return d->ignoreAutoRoutes;
}

void NetworkManager::Ipv6Setting::setIgnoreAutoDns(bool ignore)
{
    Q_D(Ipv6Setting);

    d->ignoreAutoDns = ignore;
}

bool NetworkManager::Ipv6Setting::ignoreAutoDns() const
{
    Q_D(const Ipv6Setting);

    return d->ignoreAutoDns;
}

void NetworkManager::Ipv6Setting::setNeverDefault(bool neverDefault)
{
    Q_D(Ipv6Setting);

    d->neverDefault = neverDefault;
}

bool NetworkManager::Ipv6Setting::neverDefault() const
{
    Q_D(const Ipv6Setting);

    return d->neverDefault;
}

void NetworkManager::Ipv6Setting::setMayFail(bool mayFail)
{
    Q_D(Ipv6Setting);

    d->mayFail = mayFail;
}

bool NetworkManager::Ipv6Setting::mayFail() const
{
    Q_D(const Ipv6Setting);

    return d->mayFail;
}

void NetworkManager::Ipv6Setting::setPrivacy(IPv6Privacy privacy)
{
    Q_D(Ipv6Setting);

    d->privacy = privacy;
}

NetworkManager::Ipv6Setting::IPv6Privacy NetworkManager::Ipv6Setting::privacy() const
{
    Q_D(const Ipv6Setting);

    return d->privacy;
}

void NetworkManager::Ipv6Setting::fromMap(const QVariantMap &setting)
{
    if (setting.contains(QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD))) {
        const QString methodType = setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD)).toString();

        if (methodType.toLower() == QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD_AUTO)) {
            setMethod(Automatic);
        } else if (methodType.toLower() == QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD_DHCP)) {
            setMethod(Dhcp);
        } else if (methodType.toLower() == QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD_LINK_LOCAL)) {
            setMethod(LinkLocal);
        } else if (methodType.toLower() == QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD_MANUAL)) {
            setMethod(Manual);
        } else if (methodType.toLower() == QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD_IGNORE)) {
            setMethod(Ignored);
        } else {
            setMethod(Automatic);
        }
    }

    if (setting.contains(QLatin1String(NMQT_SETTING_IP6_CONFIG_DNS))) {
        QList<QHostAddress> dbusDns;
        QList<QByteArray> temp;
        if (setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_DNS)).canConvert<QDBusArgument>()) {
            QDBusArgument dnsArg = setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_DNS)).value< QDBusArgument>();
            temp = qdbus_cast<QList<QByteArray> >(dnsArg);
        } else {
            temp = setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_DNS)).value<QList<QByteArray> >();
        }

        Q_FOREACH (const QByteArray & utmp, temp) {
            dbusDns << ipv6AddressAsHostAddress(utmp);
        }

        setDns(dbusDns);
    }

    if (setting.contains(QLatin1String(NMQT_SETTING_IP6_CONFIG_DNS_SEARCH))) {
        setDnsSearch(setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_DNS_SEARCH)).toStringList());
    }

    if (setting.contains(QLatin1String(NMQT_SETTING_IP6_CONFIG_ADDRESSES))) {
        QList<IpV6DBusAddress> temp;
        if (setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_DNS)).canConvert<QDBusArgument>()) {
            QDBusArgument addressArg = setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_ADDRESSES)).value< QDBusArgument>();
            temp = qdbus_cast<QList<IpV6DBusAddress> >(addressArg);
        } else {
            temp = setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_ADDRESSES)).value<QList<IpV6DBusAddress> >();
        }
        QList<NetworkManager::IpAddress> addresses;

        Q_FOREACH (const IpV6DBusAddress & addressMap, temp) {
            if (addressMap.address.isEmpty() || !addressMap.prefix || addressMap.gateway.isEmpty()) {
                continue;
            }

            NetworkManager::IpAddress address;
            address.setIp(ipv6AddressAsHostAddress(addressMap.address));
            address.setPrefixLength(addressMap.prefix);
            address.setGateway(ipv6AddressAsHostAddress(addressMap.gateway));
            if (!address.isValid()) {
                continue;
            }

            addresses << address;
        }

        setAddresses(addresses);
    }

    if (setting.contains(QLatin1String(NMQT_SETTING_IP6_CONFIG_ROUTES))) {
        QList<IpV6DBusRoute> temp;
        if (setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_ROUTES)).canConvert<QDBusArgument>()) {
            QDBusArgument routeArg = setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_ROUTES)).value< QDBusArgument>();
            temp = qdbus_cast<QList<IpV6DBusRoute> >(routeArg);
        } else {
            temp = setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_ROUTES)).value<QList<IpV6DBusRoute> >();
        }
        QList<NetworkManager::IpRoute> routes;

        Q_FOREACH (const IpV6DBusRoute & routeMap, temp) {
            if (routeMap.destination.isEmpty() || !routeMap.prefix || routeMap.nexthop.isEmpty() || !routeMap.metric) {
                continue;
            }

            NetworkManager::IpRoute route;
            route.setIp(ipv6AddressAsHostAddress(routeMap.destination));
            route.setPrefixLength(routeMap.prefix);
            route.setNextHop(ipv6AddressAsHostAddress(routeMap.nexthop));
            route.setMetric(routeMap.metric);
            if (!route.isValid()) {
                continue;
            }

            routes << route;
        }
        setRoutes(routes);
    }

    if (setting.contains(QLatin1String(NMQT_SETTING_IP6_CONFIG_ROUTE_METRIC))) {
        setRouteMetric(setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_ROUTE_METRIC)).toInt());
    }

    if (setting.contains(QLatin1String(NMQT_SETTING_IP6_CONFIG_IGNORE_AUTO_ROUTES))) {
        setIgnoreAutoRoutes(setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_IGNORE_AUTO_ROUTES)).toBool());
    }

    if (setting.contains(QLatin1String(NMQT_SETTING_IP6_CONFIG_IGNORE_AUTO_DNS))) {
        setIgnoreAutoDns(setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_IGNORE_AUTO_DNS)).toBool());
    }

    if (setting.contains(QLatin1String(NMQT_SETTING_IP6_CONFIG_NEVER_DEFAULT))) {
        setNeverDefault(setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_NEVER_DEFAULT)).toBool());
    }

    if (setting.contains(QLatin1String(NMQT_SETTING_IP6_CONFIG_MAY_FAIL))) {
        setMayFail(setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_MAY_FAIL)).toBool());
    }

    if (setting.contains(QLatin1String(NMQT_SETTING_IP6_CONFIG_IP6_PRIVACY))) {
        setPrivacy((IPv6Privacy)setting.value(QLatin1String(NMQT_SETTING_IP6_CONFIG_IP6_PRIVACY)).toUInt());
    }
}

QVariantMap NetworkManager::Ipv6Setting::toMap() const
{
    QVariantMap setting;

    if (method() == Automatic) {
        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD), QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD_AUTO));
    } else if (method() == Dhcp) {
        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD), QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD_DHCP));
    } else if (method() == LinkLocal) {
        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD), QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD_LINK_LOCAL));
    } else if (method() == Manual) {
        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD), QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD_MANUAL));
    } else if (method() == Ignored) {
        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD), QLatin1String(NMQT_SETTING_IP6_CONFIG_METHOD_IGNORE));
    }

    if (!dns().isEmpty()) {
        QList<QByteArray> dbusDns;
        Q_FOREACH (const QHostAddress & dns, dns()) {
            dbusDns << ipv6AddressFromHostAddress(dns);
        }
        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_DNS), QVariant::fromValue(dbusDns));
    }

    if (!dnsSearch().isEmpty()) {
        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_DNS_SEARCH), dnsSearch());
    }

    if (!addresses().isEmpty()) {
        QList<IpV6DBusAddress> dbusAddresses;
        Q_FOREACH (const NetworkManager::IpAddress & addr, addresses()) {
            IpV6DBusAddress dbusAddress;
            dbusAddress.address = ipv6AddressFromHostAddress(addr.ip());
            dbusAddress.prefix = addr.prefixLength();
            dbusAddress.gateway = ipv6AddressFromHostAddress(addr.gateway());
            dbusAddresses << dbusAddress;
        }

        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_ADDRESSES), QVariant::fromValue(dbusAddresses));
    }

    if (!routes().isEmpty()) {
        QList<IpV6DBusRoute> dbusRoutes;
        Q_FOREACH (const NetworkManager::IpRoute & route, routes()) {
            IpV6DBusRoute dbusRoute;
            dbusRoute.destination = ipv6AddressFromHostAddress(route.ip());
            dbusRoute.prefix = route.prefixLength();
            dbusRoute.nexthop = ipv6AddressFromHostAddress(route.nextHop());
            dbusRoute.metric = route.metric();
            dbusRoutes << dbusRoute;
        }

        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_ROUTES), QVariant::fromValue(dbusRoutes));
    }

    if(routeMetric() >= 0) {
        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_ROUTE_METRIC), routeMetric());
    }

    if (ignoreAutoRoutes()) {
        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_IGNORE_AUTO_ROUTES), ignoreAutoRoutes());
    }

    if (ignoreAutoDns()) {
        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_IGNORE_AUTO_DNS), ignoreAutoDns());
    }

    if (neverDefault()) {
        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_NEVER_DEFAULT), neverDefault());
    }

    if (!mayFail()) {
        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_MAY_FAIL), mayFail());
    }

    if (privacy() != Unknown) {
        setting.insert(QLatin1String(NMQT_SETTING_IP6_CONFIG_IP6_PRIVACY), privacy());
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::Ipv6Setting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    dbg.nospace() << NMQT_SETTING_IP6_CONFIG_METHOD << ": " << setting.method() << '\n';
    dbg.nospace() << NMQT_SETTING_IP6_CONFIG_DNS << '\n';
    Q_FOREACH (const QHostAddress & address, setting.dns()) {
        dbg.nospace() << address.toString() << '\n';
    }
    dbg.nospace() << NMQT_SETTING_IP6_CONFIG_DNS_SEARCH << ": " << setting.dnsSearch() << '\n';
    dbg.nospace() << NMQT_SETTING_IP6_CONFIG_ADDRESSES << '\n';
    Q_FOREACH (const NetworkManager::IpAddress & address, setting.addresses()) {
        dbg.nospace() << address.ip().toString() << ": " << address.gateway().toString() << ": " << address.netmask() << '\n';
    }
    dbg.nospace() << NMQT_SETTING_IP6_CONFIG_ROUTES << '\n';
    Q_FOREACH (const NetworkManager::IpRoute & route, setting.routes()) {
        dbg.nospace() << route.ip().toString() << ": " << route.metric() << ": " << route.nextHop().toString() << ": " << route.metric() << '\n';
    }
    dbg.nospace() << NMQT_SETTING_IP6_CONFIG_ROUTE_METRIC << ":" << setting.routeMetric() << '\n';
    dbg.nospace() << NMQT_SETTING_IP6_CONFIG_IGNORE_AUTO_ROUTES << ": " << setting.ignoreAutoRoutes() << '\n';
    dbg.nospace() << NMQT_SETTING_IP6_CONFIG_IGNORE_AUTO_DNS << ": " << setting.ignoreAutoDns() << '\n';
    dbg.nospace() << NMQT_SETTING_IP6_CONFIG_NEVER_DEFAULT << ": " << setting.neverDefault() << '\n';
    dbg.nospace() << NMQT_SETTING_IP6_CONFIG_MAY_FAIL << ": " << setting.mayFail() << '\n';
    dbg.nospace() << NMQT_SETTING_IP6_CONFIG_IP6_PRIVACY << ": " << setting.privacy() << '\n';

    return dbg.maybeSpace();
}
