/*
    Copyright 2012  Jan Grulich <jgrulich@redhat.com>

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

#include "ipv4.h"
#include "ipv4_p.h"

#include <arpa/inet.h>
#include <QtNetworkManager/generic-types.h>

#include <nm-setting-ip4-config.h>

#include <QDebug>

NetworkManager::Settings::Ipv4SettingPrivate::Ipv4SettingPrivate():
    name(QString("ipv4")),
    method(NetworkManager::Settings::Ipv4Setting::Automatic),
    dns(QList<QHostAddress>()),
    dnsSearch(QStringList()),
    addresses(QList<NetworkManager::IPv4Address>()),
    routes(QList<NetworkManager::IPv4Route>()),
    ignoreAutoRoutes(false),
    ignoreAutoDns(false),
    dhcpClientId(QString()),
    dhcpSendHostname(true),
    dhcpHostname(QString()),
    neverDefault(false),
    mayFail(true)
{ }

NetworkManager::Settings::Ipv4Setting::Ipv4Setting():
    Setting(Setting::Ipv4),
    d_ptr(new Ipv4SettingPrivate())
{ }

NetworkManager::Settings::Ipv4Setting::Ipv4Setting(NetworkManager::Settings::Ipv4Setting* setting):
    Setting(setting),
    d_ptr(new Ipv4SettingPrivate())
{
    setMethod(setting->method());
    setDns(setting->dns());
    setDnsSearch(setting->dnsSearch());
    setAddresses(setting->addresses());
    setRoutes(setting->routes());
    setIgnoreAutoRoutes(setting->ignoreAutoRoutes());
    setIgnoreAutoDns(setting->ignoreAutoDns());
    setDhcpClientId(setting->dhcpClientId());
    setDhcpSendHostname(setting->dhcpSendHostname());
    setDhcpHostname(setting->dhcpHostname());
    setNeverDefault(setting->neverDefault());
    setMayFail(setting->mayFail());
}

NetworkManager::Settings::Ipv4Setting::~Ipv4Setting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::Ipv4Setting::name() const
{
    Q_D(const Ipv4Setting);

    return d->name;
}

void NetworkManager::Settings::Ipv4Setting::setMethod(NetworkManager::Settings::Ipv4Setting::ConfigMethod type)
{
    Q_D(Ipv4Setting);

    d->method = type;
}

NetworkManager::Settings::Ipv4Setting::ConfigMethod NetworkManager::Settings::Ipv4Setting::method() const
{
    Q_D(const Ipv4Setting);

    return d->method;
}

void NetworkManager::Settings::Ipv4Setting::setDns(const QList<QHostAddress>& dns)
{
    Q_D(Ipv4Setting);

    d->dns = dns;
}

QList<QHostAddress> NetworkManager::Settings::Ipv4Setting::dns() const
{
    Q_D(const Ipv4Setting);

    return d->dns;
}

void NetworkManager::Settings::Ipv4Setting::setDnsSearch(const QStringList& domains)
{
    Q_D(Ipv4Setting);

    d->dnsSearch = domains;
}

QStringList NetworkManager::Settings::Ipv4Setting::dnsSearch() const
{
    Q_D(const Ipv4Setting);

    return d->dnsSearch;
}

void NetworkManager::Settings::Ipv4Setting::setAddresses(const QList< NetworkManager::IPv4Address > ipv4addresses)
{
    Q_D(Ipv4Setting);

    d->addresses = ipv4addresses;
}

QList< NetworkManager::IPv4Address > NetworkManager::Settings::Ipv4Setting::addresses() const
{
    Q_D(const Ipv4Setting);

    return d->addresses;
}

void NetworkManager::Settings::Ipv4Setting::setRoutes(const QList< NetworkManager::IPv4Route > ipv4routes)
{
    Q_D(Ipv4Setting);

    d->routes = ipv4routes;
}

QList< NetworkManager::IPv4Route > NetworkManager::Settings::Ipv4Setting::routes() const
{
    Q_D(const Ipv4Setting);

    return d->routes;
}

void NetworkManager::Settings::Ipv4Setting::setIgnoreAutoRoutes(bool ignore)
{
    Q_D(Ipv4Setting);

    d->ignoreAutoRoutes = ignore;
}

bool NetworkManager::Settings::Ipv4Setting::ignoreAutoRoutes() const
{
    Q_D(const Ipv4Setting);

    return d->ignoreAutoRoutes;
}

void NetworkManager::Settings::Ipv4Setting::setIgnoreAutoDns(bool ignore)
{
    Q_D(Ipv4Setting);

    d->ignoreAutoDns = ignore;
}

bool NetworkManager::Settings::Ipv4Setting::ignoreAutoDns() const
{
    Q_D(const Ipv4Setting);

    return d->ignoreAutoDns;
}

void NetworkManager::Settings::Ipv4Setting::setDhcpClientId(const QString& id)
{
    Q_D(Ipv4Setting);

    d->dhcpClientId = id;
}

QString NetworkManager::Settings::Ipv4Setting::dhcpClientId() const
{
    Q_D(const Ipv4Setting);

    return d->dhcpClientId;
}

void NetworkManager::Settings::Ipv4Setting::setDhcpSendHostname(bool send)
{
    Q_D(Ipv4Setting);

    d->dhcpSendHostname = send;
}

bool NetworkManager::Settings::Ipv4Setting::dhcpSendHostname() const
{
    Q_D(const Ipv4Setting);

    return d->dhcpSendHostname;
}

void NetworkManager::Settings::Ipv4Setting::setDhcpHostname(const QString& hostname)
{
    Q_D(Ipv4Setting);

    d->dhcpHostname = hostname;
}

QString NetworkManager::Settings::Ipv4Setting::dhcpHostname() const
{
    Q_D(const Ipv4Setting);

    return d->dhcpHostname;
}

void NetworkManager::Settings::Ipv4Setting::setNeverDefault(bool neverDefault)
{
    Q_D(Ipv4Setting);

    d->neverDefault = neverDefault;
}

bool NetworkManager::Settings::Ipv4Setting::neverDefault() const
{
    Q_D(const Ipv4Setting);

    return d->neverDefault;
}

void NetworkManager::Settings::Ipv4Setting::setMayFail(bool mayFail)
{
    Q_D(Ipv4Setting);

    d->mayFail = mayFail;
}

bool NetworkManager::Settings::Ipv4Setting::mayFail() const
{
    Q_D(const Ipv4Setting);

    return d->mayFail;
}

void NetworkManager::Settings::Ipv4Setting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_IP4_CONFIG_METHOD))) {
        QString methodType = setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_METHOD)).toString();

        if (methodType.toLower() == QLatin1String(NM_SETTING_IP4_CONFIG_METHOD_AUTO)) {
            setMethod(Automatic);
        } else if (methodType.toLower() == QLatin1String(NM_SETTING_IP4_CONFIG_METHOD_LINK_LOCAL)) {
            setMethod(LinkLocal);
        } else if (methodType.toLower() == QLatin1String(NM_SETTING_IP4_CONFIG_METHOD_MANUAL)) {
            setMethod(Manual);
        } else if (methodType.toLower() == QLatin1String(NM_SETTING_IP4_CONFIG_METHOD_SHARED)) {
            setMethod(Shared);
        } else if (methodType.toLower() == QLatin1String(NM_SETTING_IP4_CONFIG_METHOD_DISABLED)) {
            setMethod(Disabled);
        } else {
            setMethod(Automatic);
        }
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP4_CONFIG_DNS))) {
        QList<QHostAddress> dbusDns;
        QList<uint> temp;
        if (setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_DNS)).canConvert<QDBusArgument>()) {
            QDBusArgument dnsArg = setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_DNS)).value<QDBusArgument>();
            temp = qdbus_cast<QList<uint> >(dnsArg);
        } else {
            temp = setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_DNS)).value<QList<uint> >();
        }

        foreach(const uint utmp, temp) {
            QHostAddress tmpHost(ntohl(utmp));
            dbusDns << tmpHost;
        }
        setDns(dbusDns);
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP4_CONFIG_DNS_SEARCH))) {
        setDnsSearch(setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_DNS_SEARCH)).toStringList());
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP4_CONFIG_ADDRESSES))) {
        QList<NetworkManager::IPv4Address> addresses;
        QList<QList<uint> > temp;
        if (setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_ADDRESSES)).canConvert< QDBusArgument>()) {
            QDBusArgument addressArg = setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_ADDRESSES)).value<QDBusArgument>();
            temp = qdbus_cast<QList<QList<uint> > >(addressArg);
        } else {
            temp = setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_ADDRESSES)).value<QList<QList<uint> > >();
        }

        foreach(const QList<uint> & uintList, temp) {
            if (uintList.count() != 3)
            {
                continue;
            }

            NetworkManager::IPv4Address addr((quint32)ntohl(uintList.at(0)), (quint32)uintList.at(1), (quint32) ntohl(uintList.at(2)));
            if (!addr.isValid())
            {
                continue;
            }

            addresses << addr;
        }

        setAddresses(addresses);
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP4_CONFIG_ROUTES))) {
        QList<NetworkManager::IPv4Route> routes;
        QList<QList<uint> > temp;
        if (setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_ROUTES)).canConvert< QDBusArgument>()) {
            QDBusArgument routeArg = setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_ROUTES)).value<QDBusArgument>();
            temp = qdbus_cast<QList<QList<uint> > >(routeArg);
        } else {
            temp = setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_ROUTES)).value<QList<QList<uint> > >();
        }

        foreach(const QList<uint> & uintList, temp) {
            if (uintList.count() != 4)
            {
              continue;
            }

            NetworkManager::IPv4Route route((quint32)ntohl(uintList.at(0)), (quint32)uintList.at(1), (quint32)ntohl(uintList.at(2)), (quint32)uintList.at(3));
            if (!route.isValid())
            {
                continue;
            }
            routes << route;
        }

        if (!routes.isEmpty()) {
            setRoutes(routes);
        }
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP4_CONFIG_IGNORE_AUTO_ROUTES))) {
        setIgnoreAutoRoutes(setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_IGNORE_AUTO_ROUTES)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP4_CONFIG_IGNORE_AUTO_DNS))) {
        setIgnoreAutoDns(setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_IGNORE_AUTO_DNS)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP4_CONFIG_DHCP_CLIENT_ID))) {
        setDhcpClientId(setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_DHCP_CLIENT_ID)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP4_CONFIG_DHCP_SEND_HOSTNAME))) {
        setDhcpSendHostname(setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_DHCP_SEND_HOSTNAME)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP4_CONFIG_DHCP_HOSTNAME))) {
        setDhcpHostname(setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_DHCP_HOSTNAME)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP4_CONFIG_NEVER_DEFAULT))) {
        setNeverDefault(setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_NEVER_DEFAULT)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_IP4_CONFIG_MAY_FAIL))) {
        setMayFail(setting.value(QLatin1String(NM_SETTING_IP4_CONFIG_MAY_FAIL)).toBool());
    }
}

void NetworkManager::Settings::Ipv4Setting::printSetting()
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
    foreach (const NetworkManager::IPv4Address & address, addresses()) {
        qDebug() << address.address() << " - " << address.gateway() << " - " << address.netMask()  << ", ";
    }
    qDebug() << "ROUTES - ";
    foreach (const NetworkManager::IPv4Route & route, routes()) {
        qDebug() << route.route() << ",";
    }
    qDebug() << "IGNORE AUTO ROUTES - " << ignoreAutoRoutes();
    qDebug() << "IGNORE AUTO DNS - " << ignoreAutoDns();
    qDebug() << "DHCP CLIENT ID - " << dhcpClientId();
    qDebug() << "DHCP SEND HOSTNAME - " << dhcpSendHostname();
    qDebug() << "DHCP HOSTNAME - " << dhcpHostname();
    qDebug() << "NEVER DEFAULT - " << neverDefault();
    qDebug() << "MAY FAIL - " << mayFail();
}
