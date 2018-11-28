/*
    Copyright 2018 Pranav Gade <pranavgade20@gmail.com>

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

#ifndef NETWORKMANAGERQT_IPTUNNEL_SETTING_H
#define NETWORKMANAGERQT_IPTUNNEL_SETTING_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "setting.h"

namespace NetworkManager
{

class IpTunnelSettingPrivate;

/**
 * Represents IpTunnel setting
 */
class NETWORKMANAGERQT_EXPORT IpTunnelSetting : public Setting
{
public:
    typedef QSharedPointer<IpTunnelSetting> Ptr;
    typedef QList<Ptr> List;
    enum Mode {
        Unknown = NM_IP_TUNNEL_MODE_UNKNOWN,
        Ipip = NM_IP_TUNNEL_MODE_IPIP,
        Gre = NM_IP_TUNNEL_MODE_GRE,
        Sit = NM_IP_TUNNEL_MODE_SIT,
        Isatap = NM_IP_TUNNEL_MODE_ISATAP,
        Vti = NM_IP_TUNNEL_MODE_VTI,
        Ip6ip6 = NM_IP_TUNNEL_MODE_IP6IP6,
        Ipip6 = NM_IP_TUNNEL_MODE_IPIP6,
        Ip6gre = NM_IP_TUNNEL_MODE_IP6GRE,
        Vti6 = NM_IP_TUNNEL_MODE_VTI
    };

    enum Flag {
        None = NM_IP_TUNNEL_FLAG_NONE,
        Ip6IgnEncapLimit = NM_IP_TUNNEL_FLAG_IP6_IGN_ENCAP_LIMIT,
        Ip6UseOrigTclass = NM_IP_TUNNEL_FLAG_IP6_USE_ORIG_TCLASS,
        Ip6UseOrigFlowlabel = NM_IP_TUNNEL_FLAG_IP6_USE_ORIG_FLOWLABEL,
        Ip6Mip6Dev = NM_IP_TUNNEL_FLAG_IP6_MIP6_DEV,
        Ip6RcvDscpCopy = NM_IP_TUNNEL_FLAG_IP6_RCV_DSCP_COPY,
        Ip6UseOrigFwmark = NM_IP_TUNNEL_FLAG_IP6_USE_ORIG_FWMARK
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    IpTunnelSetting();
    explicit IpTunnelSetting(const Ptr &other);
    ~IpTunnelSetting() override;

    QString name() const override;

    void setMode(Mode mode);
    Mode mode() const;

    void setPathMtuDiscovery(bool discovery);
    bool pathMtuDiscovery() const;

    void setEncapsulationLimit(quint32 limit);
    quint32 encapsulationLimit() const;

    void setFlags(Flags flags);
    Flags flags() const;

    void setFlowLabel(quint32 label);
    quint32 flowLabel() const;

    void setMtu(quint32 mtu);
    quint32 mtu() const;

    void setTos(quint32 tos);
    quint32 tos() const;

    void setTtl(quint32 ttl);
    quint32 ttl() const;

    void setInputKey(const QString &key);
    QString inputKey() const;

    void setLocal(const QString &local);
    QString local() const;

    void setParent(const QString &parent);
    QString parent() const;

    void setOutputKey(const QString &key);
    QString outputKey() const;

    void setRemote(const QString &remote);
    QString remote() const;

    void fromMap(const QVariantMap &setting) override;

    QVariantMap toMap() const override;

protected:
    IpTunnelSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(IpTunnelSetting)
};
Q_DECLARE_OPERATORS_FOR_FLAGS(IpTunnelSetting::Flags)

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const IpTunnelSetting &setting);

}

#endif // NETWORKMANAGERQT_IP_TUNNEL_SETTING_H

