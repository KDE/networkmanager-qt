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

#ifndef NMQT_SETTINGS_IPV4_H
#define NMQT_SETTINGS_IPV4_H

#include "../ipconfig.h"
#include "../NetworkManagerQt-export.h"
#include "setting.h"

#include <QtCore/QStringList>
#include <QtNetwork/QHostAddress>

namespace NetworkManager
{

class Ipv4SettingPrivate;
class NETWORKMANAGERQT_EXPORT Ipv4Setting : public Setting
{
public:
    typedef QSharedPointer<Ipv4Setting> Ptr;
    typedef QList<Ptr> List;
    enum ConfigMethod {Automatic, LinkLocal, Manual, Shared, Disabled};

    Ipv4Setting();
    explicit Ipv4Setting(const Ptr &other);
    ~Ipv4Setting();

    QString name() const;

    void setMethod(ConfigMethod method);
    ConfigMethod method() const;

    void setDns(const QList<QHostAddress> &dns);
    QList<QHostAddress> dns() const;

    void setDnsSearch(const QStringList &domains);
    QStringList dnsSearch() const;

    void setAddresses(const QList<NetworkManager::IpAddress> &ipv4addresses);
    QList<NetworkManager::IpAddress> addresses() const;

    void setRoutes(const QList<NetworkManager::IpRoute> &ipv4routes);
    QList<NetworkManager::IpRoute> routes() const;

    void setIgnoreAutoRoutes(bool ignore);
    bool ignoreAutoRoutes() const;

    void setIgnoreAutoDns(bool ignore);
    bool ignoreAutoDns() const;

    void setDhcpClientId(const QString &id);
    QString dhcpClientId() const;

    void setDhcpSendHostname(bool send);
    bool dhcpSendHostname() const;

    void setDhcpHostname(const QString &hostname);
    QString dhcpHostname() const;

    void setNeverDefault(bool neverDefault);
    bool neverDefault() const;

    void setMayFail(bool mayFail);
    bool mayFail() const;

    void fromMap(const QVariantMap &setting);

    QVariantMap toMap() const;

protected:
    Ipv4SettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(Ipv4Setting)
};

QDebug operator<<(QDebug dbg, const Ipv4Setting &setting);

}

#endif // NMQT_SETTINGS_IPV4_H
