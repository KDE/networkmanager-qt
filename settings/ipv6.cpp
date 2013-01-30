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

#include "ipv6.h"
#include "ipv6_p.h"

#include "generic-types.h"
#include "../ipv4config.h"

#include <arpa/inet.h>
#include <nm-setting-ip6-config.h>

#include <QtCore/QDebug>

NetworkManager::Settings::Ipv6SettingPrivate::Ipv6SettingPrivate():
    name(QString("ipv6")),
    method(NetworkManager::Settings::Ipv6Setting::Automatic),
    dns(QList<QHostAddress>()),
    dnsSearch(QStringList()),
    addresses(QList<NetworkManager::IPv6Address>()),
    routes(QList<NetworkManager::IPv6Route>()),
    ignoreAutoRoutes(false),
    ignoreAutoDns(false),
    neverDefault(false),
    mayFail(true),
    privacy(NetworkManager::Settings::Ipv6Setting::Unknown)
{ }

NetworkManager::Settings::Ipv6Setting::Ipv6Setting():
    Setting(Setting::Ipv6),
    d_ptr(new Ipv6SettingPrivate())
{ }

NetworkManager::Settings::Ipv6Setting::Ipv6Setting(NetworkManager::Settings::Ipv6Setting* setting):
    Setting(setting),
    d_ptr(new Ipv6SettingPrivate())
{
    setMethod(setting->method());
    setDns(setting->dns());
    setDnsSearch(setting->dnsSearch());
    setAddresses(setting->addresses());
    setRoutes(setting->routes());
    setIgnoreAutoRoutes(setting->ignoreAutoRoutes());
    setIgnoreAutoDns(setting->ignoreAutoDns());
    setNeverDefault(setting->neverDefault());
    setMayFail(setting->mayFail());
}

NetworkManager::Settings::Ipv6Setting::~Ipv6Setting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::Ipv6Setting::name() const
{
    Q_D(const Ipv6Setting);

    return d->name;
}

void NetworkManager::Settings::Ipv6Setting::setMethod(NetworkManager::Settings::Ipv6Setting::ConfigMethod type)
{
    Q_D(Ipv6Setting);

    d->method = type;
}

NetworkManager::Settings::Ipv6Setting::ConfigMethod NetworkManager::Settings::Ipv6Setting::method() const
{
    Q_D(const Ipv6Setting);

    return d->method;
}

void NetworkManager::Settings::Ipv6Setting::setDns(const QList<QHostAddress>& dns)
{
    Q_D(Ipv6Setting);

    d->dns = dns;
}

QList<QHostAddress> NetworkManager::Settings::Ipv6Setting::dns() const
{
    Q_D(const Ipv6Setting);

    return d->dns;
}

void NetworkManager::Settings::Ipv6Setting::setDnsSearch(const QStringList& domains)
{
    Q_D(Ipv6Setting);

    d->dnsSearch = domains;
}

QStringList NetworkManager::Settings::Ipv6Setting::dnsSearch() const
{
    Q_D(const Ipv6Setting);

    return d->dnsSearch;
}

void NetworkManager::Settings::Ipv6Setting::setAddresses(const QList< NetworkManager::IPv6Address > ipv6addresses)
{
    Q_D(Ipv6Setting);

    d->addresses = ipv6addresses;
}

QList< NetworkManager::IPv6Address > NetworkManager::Settings::Ipv6Setting::addresses() const
{
    Q_D(const Ipv6Setting);

    return d->addresses;
}

void NetworkManager::Settings::Ipv6Setting::setRoutes(const QList< NetworkManager::IPv6Route > ipv6routes)
{
    Q_D(Ipv6Setting);

    d->routes = ipv6routes;
}

QList< NetworkManager::IPv6Route > NetworkManager::Settings::Ipv6Setting::routes() const
{
    Q_D(const Ipv6Setting);

    return d->routes;
}

void NetworkManager::Settings::Ipv6Setting::setIgnoreAutoRoutes(bool ignore)
{
    Q_D(Ipv6Setting);

    d->ignoreAutoRoutes = ignore;
}

bool NetworkManager::Settings::Ipv6Setting::ignoreAutoRoutes() const
{
    Q_D(const Ipv6Setting);

    return d->ignoreAutoRoutes;
}

void NetworkManager::Settings::Ipv6Setting::setIgnoreAutoDns(bool ignore)
{
    Q_D(Ipv6Setting);

    d->ignoreAutoDns = ignore;
}

bool NetworkManager::Settings::Ipv6Setting::ignoreAutoDns() const
{
    Q_D(const Ipv6Setting);

    return d->ignoreAutoDns;
}

void NetworkManager::Settings::Ipv6Setting::setNeverDefault(bool neverDefault)
{
    Q_D(Ipv6Setting);

    d->neverDefault = neverDefault;
}

bool NetworkManager::Settings::Ipv6Setting::neverDefault() const
{
    Q_D(const Ipv6Setting);

    return d->neverDefault;
}

void NetworkManager::Settings::Ipv6Setting::setMayFail(bool mayFail)
{
    Q_D(Ipv6Setting);

    d->mayFail = mayFail;
}

bool NetworkManager::Settings::Ipv6Setting::mayFail() const
{
    Q_D(const Ipv6Setting);

    return d->mayFail;
}

void NetworkManager::Settings::Ipv6Setting::setPrivacy(IPv6Privacy privacy)
{
    Q_D(Ipv6Setting);

    d->privacy = privacy;
}

NetworkManager::Settings::Ipv6Setting::IPv6Privacy NetworkManager::Settings::Ipv6Setting::privacy() const
{
    Q_D(const Ipv6Setting);

    return d->privacy;
}

bool NetworkManager::Settings::Ipv6Setting::hasSecrets() const
{
    return false;
}

void NetworkManager::Settings::Ipv6Setting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_IP6_CONFIG_METHOD))) {
        QString methodType = setting.value(QLatin1String(NM_SETTING_IP6_CONFIG_METHOD)).toString();

        if (methodType.toLower() == QLatin1String(NM_SETTING_IP6_CONFIG_METHOD_AUTO)) {
            setMethod(Automatic);
        } else if (methodType.toLower() == QLatin1String(NM_SETTING_IP6_CONFIG_METHOD_DHCP)) {
            setMethod(Dhcp);
        } else if (methodType.toLower() == QLatin1String(NM_SETTING_IP6_CONFIG_METHOD_LINK_LOCAL)) {
            setMethod(LinkLocal);
        } else if (methodType.toLower() == QLatin1String(NM_SETTING_IP6_CONFIG_METHOD_MANUAL)) {
            setMethod(Manual);
        } else if (methodType.toLower() == QLatin1String(NM_SETTING_IP6_CONFIG_METHOD_IGNORE)) {
            setMethod(Ignored);
        } else {
            setMethod(Automatic);
        }
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP6_CONFIG_DNS))) {
        IpV6DBusNameservers dnsArg = setting.value(QLatin1String(NM_SETTING_IP6_CONFIG_DNS)).value<IpV6DBusNameservers>();
        QList<QHostAddress> dbusDns;

        foreach (const QByteArray &dns, dnsArg) {
            QHostAddress tmpHost;
            tmpHost.setAddress(QString(dns));
            dbusDns << tmpHost;
        }

        setDns(dbusDns);
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP6_CONFIG_DNS_SEARCH))) {
        setDnsSearch(setting.value(QLatin1String(NM_SETTING_IP6_CONFIG_DNS_SEARCH)).toStringList());
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP6_CONFIG_ADDRESSES))) {
        IpV6DBusAddressList addressArg = setting.value(QLatin1String(NM_SETTING_IP6_CONFIG_ADDRESSES)).value<IpV6DBusAddressList>();
        QList<NetworkManager::IPv6Address> addresses;

        foreach (const IpV6DBusAddress & address, addressArg) {
            QHostAddress tmpAddress;
            tmpAddress.setAddress(QString(address.address));
            QHostAddress tmpGateway;
            tmpGateway.setAddress(QString(address.gateway));
            NetworkManager::IPv6Address addr(tmpAddress.toIPv6Address(), address.netMask, tmpGateway.toIPv6Address());

            addresses << addr;
        }

        setAddresses(addresses);
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP6_CONFIG_ROUTES))) {
        IpV6DBusRouteList routeArg = setting.value(QLatin1String(NM_SETTING_IP6_CONFIG_ROUTES)).value<IpV6DBusRouteList>();
        QList<NetworkManager::IPv6Route> routes;

        foreach (const IpV6DBusRoute & dbusRoute, routeArg) {
            QHostAddress tmpDestination;
            tmpDestination.setAddress(QString(dbusRoute.destination));
            QHostAddress tmpNexthop;
            tmpNexthop.setAddress(QString(dbusRoute.nexthop));
            NetworkManager::IPv6Route route(tmpDestination.toIPv6Address(), dbusRoute.prefix, tmpNexthop.toIPv6Address(), dbusRoute.metric);

            routes << route;
        }

        setRoutes(routes);
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP6_CONFIG_IGNORE_AUTO_ROUTES))) {
        setIgnoreAutoRoutes(setting.value(QLatin1String(NM_SETTING_IP6_CONFIG_IGNORE_AUTO_ROUTES)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP6_CONFIG_IGNORE_AUTO_DNS))) {
        setIgnoreAutoDns(setting.value(QLatin1String(NM_SETTING_IP6_CONFIG_IGNORE_AUTO_DNS)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP6_CONFIG_NEVER_DEFAULT))) {
        setNeverDefault(setting.value(QLatin1String(NM_SETTING_IP6_CONFIG_NEVER_DEFAULT)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP6_CONFIG_MAY_FAIL))) {
        setMayFail(setting.value(QLatin1String(NM_SETTING_IP6_CONFIG_MAY_FAIL)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP6_CONFIG_IP6_PRIVACY))) {
        setPrivacy((IPv6Privacy)setting.value(QLatin1String(NM_SETTING_IP6_CONFIG_IP6_PRIVACY)).toUInt());
    }
}

QVariantMap NetworkManager::Settings::Ipv6Setting::toMap() const
{
    QVariantMap setting;

    if (method() == Automatic) {
        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_METHOD), QLatin1String(NM_SETTING_IP6_CONFIG_METHOD_AUTO));
    } else if (method() == Dhcp) {
        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_METHOD), QLatin1String(NM_SETTING_IP6_CONFIG_METHOD_DHCP));
    } else if (method() == LinkLocal) {
        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_METHOD), QLatin1String(NM_SETTING_IP6_CONFIG_METHOD_LINK_LOCAL));
    } else if (method() == Manual) {
        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_METHOD), QLatin1String(NM_SETTING_IP6_CONFIG_METHOD_MANUAL));
    } else if (method() == Ignored) {
        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_METHOD), QLatin1String(NM_SETTING_IP6_CONFIG_METHOD_IGNORE));
    }

    if (!dns().isEmpty()) {
        QList<QByteArray> dbusDns;
        foreach (const QHostAddress &dns, dns()) {
            dbusDns << dns.toString().toAscii();
        }

        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_DNS), QVariant::fromValue(dbusDns));
    }

    if (!dnsSearch().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_DNS_SEARCH), dnsSearch());
    }

    if (!addresses().isEmpty()) {
        IpV6DBusAddressList dbusAddresses;

        foreach (const NetworkManager::IPv6Address &addr, addresses()) {
            IpV6DBusAddress dbusAddress;
            dbusAddress.address = QHostAddress(addr.address()).toString().toAscii();
            dbusAddress.netMask = addr.netMask();
            dbusAddress.gateway = QHostAddress(addr.gateway()).toString().toAscii();

            dbusAddresses << dbusAddress;
        }

        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_ADDRESSES), QVariant::fromValue(dbusAddresses));
    }

    if (!routes().isEmpty()) {
        IpV6DBusRouteList dbusRoutes;

        foreach (const NetworkManager::IPv6Route &route, routes()) {
            IpV6DBusRoute dbusRoute;
            dbusRoute.destination = QHostAddress(route.route()).toString().toAscii();
            dbusRoute.prefix = route.prefix();
            dbusRoute.nexthop = QHostAddress(route.nextHop()).toString().toAscii();
            dbusRoute.metric = route.metric();

            dbusRoutes << dbusRoute;
        }

        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_ROUTES), QVariant::fromValue(dbusRoutes));
    }

    if (ignoreAutoRoutes()) {
        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_IGNORE_AUTO_ROUTES), ignoreAutoRoutes());
    }

    if (ignoreAutoDns()) {
        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_IGNORE_AUTO_DNS), ignoreAutoDns());
    }

    if (neverDefault()) {
        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_NEVER_DEFAULT), neverDefault());
    }

    if (!mayFail()) {
        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_MAY_FAIL), mayFail());
    }

    if (privacy() != Unknown) {
        setting.insert(QLatin1String(NM_SETTING_IP6_CONFIG_IP6_PRIVACY), privacy());
    }

    return setting;
}

void NetworkManager::Settings::Ipv6Setting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << "NAME - " << name();
    qDebug() << "METHOD - " << method();
    qDebug() << "DNS SERVERS -";
    foreach (const QHostAddress & address, dns()) {
        qDebug() << address.toString() << ", ";
    }
    qDebug() << "DNS SEARCH - " << dnsSearch();
    qDebug() << "ADDRESSES - ";
    foreach (const NetworkManager::IPv6Address & address, addresses()) {
        qDebug() << QHostAddress(address.address()).toString() << " - " << QHostAddress(address.gateway()).toString() << " - " << address.netMask()  << ", ";
    }
    qDebug() << "ROUTES - ";
    foreach (const NetworkManager::IPv6Route & route, routes()) {
        qDebug() << QHostAddress(route.route()).toString() << " - " << route.metric() << " - " << QHostAddress(route.nextHop()).toString() << " - " << route.metric() << ", ";
    }
    qDebug() << "IGNORE AUTO ROUTES - " << ignoreAutoRoutes();
    qDebug() << "IGNORE AUTO DNS - " << ignoreAutoDns();
    qDebug() << "NEVER DEFAULT - " << neverDefault();
    qDebug() << "MAY FAIL - " << mayFail();
    qDebug() << "PRIVACY - " << privacy();
}
