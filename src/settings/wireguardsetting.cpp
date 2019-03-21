/*
    Copyright 2019 Jan Grulich <jgrulich@redhat.com>

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

#include "wireguardsetting.h"
#include "wireguardsetting_p.h"

#include <QDebug>

#if !NM_CHECK_VERSION(1, 16, 0)
#define NM_SETTING_WIREGUARD_SETTING_NAME "wireguard"

#define NM_SETTING_WIREGUARD_FWMARK            "fwmark"
#define NM_SETTING_WIREGUARD_LISTEN_PORT       "listen-port"
#define NM_SETTING_WIREGUARD_PRIVATE_KEY       "private-key"
#define NM_SETTING_WIREGUARD_PRIVATE_KEY_FLAGS "private-key-flags"
#define NM_SETTING_WIREGUARD_PEERS             "peers"
#define NM_SETTING_WIREGUARD_MTU               "mtu"
#define NM_SETTING_WIREGUARD_PEER_ROUTES       "peer-routes"
#endif

NetworkManager::WireguardSettingPrivate::WireguardSettingPrivate()
    : name(NM_SETTING_WIREGUARD_SETTING_NAME)
    , fwmark(0)
    , listenPort(0)
    , mtu(0)
    , peerRoutes(true)
    , privateKeyFlags(NetworkManager::Setting::None)
{ }

NetworkManager::WireguardSetting::WireguardSetting():
    Setting(Setting::Wireguard),
    d_ptr(new WireguardSettingPrivate())
{ }

NetworkManager::WireguardSetting::WireguardSetting(const Ptr &other)
    : Setting(other)
    , d_ptr(new WireguardSettingPrivate())
{
    setFwmark(other->fwmark());
    setListenPort(other->listenPort());
    setMtu(other->mtu());
    setPeerRoutes(other->peerRoutes());
    setPeers(other->peers());
    setPrivateKey(other->privateKey());
    setPrivateKeyFlags(other->privateKeyFlags());
}

NetworkManager::WireguardSetting::~WireguardSetting()
{
    delete d_ptr;
}

QString NetworkManager::WireguardSetting::name() const
{
    Q_D(const WireguardSetting);

    return d->name;
}

quint32 NetworkManager::WireguardSetting::fwmark() const
{
    Q_D(const WireguardSetting);

    return d->fwmark;
}

void NetworkManager::WireguardSetting::setFwmark(quint32 fwmark)
{
    Q_D(WireguardSetting);

    d->fwmark = fwmark;
}

quint32 NetworkManager::WireguardSetting::listenPort() const
{
    Q_D(const WireguardSetting);

    return d->listenPort;
}

void NetworkManager::WireguardSetting::setListenPort(quint32 port)
{
    Q_D(WireguardSetting);

    d->listenPort = port;
}

quint32 NetworkManager::WireguardSetting::mtu() const
{
    Q_D(const WireguardSetting);

    return d->mtu;

}

void NetworkManager::WireguardSetting::setMtu(quint32 mtu)
{
    Q_D(WireguardSetting);

    d->mtu = mtu;
}

bool NetworkManager::WireguardSetting::peerRoutes() const
{
    Q_D(const WireguardSetting);

    return d->peerRoutes;
}

void NetworkManager::WireguardSetting::setPeerRoutes(bool peerRoutes)
{
    Q_D(WireguardSetting);

    d->peerRoutes = peerRoutes;
}

NMVariantMapList NetworkManager::WireguardSetting::peers() const
{
    Q_D(const WireguardSetting);

    return d->peers;
}

void NetworkManager::WireguardSetting::setPeers(const NMVariantMapList &peers)
{
    Q_D(WireguardSetting);

    d->peers = peers;
}

QString NetworkManager::WireguardSetting::privateKey() const
{
    Q_D(const WireguardSetting);

    return d->privateKey;
}

void NetworkManager::WireguardSetting::setPrivateKey(const QString &key)
{
    Q_D(WireguardSetting);

    d->privateKey = key;
}

NetworkManager::Setting::SecretFlags NetworkManager::WireguardSetting::privateKeyFlags() const
{
    Q_D(const WireguardSetting);

    return d->privateKeyFlags;
}

void NetworkManager::WireguardSetting::setPrivateKeyFlags(NetworkManager::Setting::SecretFlags flags)
{
    Q_D(WireguardSetting);

    d->privateKeyFlags = flags;
}

void NetworkManager::WireguardSetting::fromMap(const QVariantMap &setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_WIREGUARD_FWMARK))) {
        setFwmark(setting.value(QLatin1String(NM_SETTING_WIREGUARD_FWMARK)).toInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIREGUARD_LISTEN_PORT))) {
        setListenPort(setting.value(QLatin1String(NM_SETTING_WIREGUARD_LISTEN_PORT)).toInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIREGUARD_MTU))) {
        setMtu(setting.value(QLatin1String(NM_SETTING_WIREGUARD_MTU)).toInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIREGUARD_PEER_ROUTES))) {
        setPeerRoutes(setting.value(QLatin1String(NM_SETTING_WIREGUARD_PEER_ROUTES)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIREGUARD_PEERS))) {
        setPeers(qdbus_cast<NMVariantMapList>(setting.value(QLatin1String(NM_SETTING_WIREGUARD_PEERS))));
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIREGUARD_PRIVATE_KEY))) {
        setPrivateKey(setting.value(QLatin1String(NM_SETTING_WIREGUARD_PRIVATE_KEY)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIREGUARD_PRIVATE_KEY_FLAGS))) {
        setPrivateKeyFlags((SecretFlags)setting.value(QLatin1String(NM_SETTING_WIREGUARD_PRIVATE_KEY_FLAGS)).toInt());
    }
}

QVariantMap NetworkManager::WireguardSetting::toMap() const
{
    QVariantMap setting;

    setting.insert(QLatin1String(NM_SETTING_WIREGUARD_FWMARK), fwmark());
    setting.insert(QLatin1String(NM_SETTING_WIREGUARD_LISTEN_PORT), listenPort());
    setting.insert(QLatin1String(NM_SETTING_WIREGUARD_MTU), mtu());
    setting.insert(QLatin1String(NM_SETTING_WIREGUARD_PEER_ROUTES), peerRoutes());

    if (!peers().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_WIREGUARD_PEERS), QVariant::fromValue(peers()));
    }

    if (!privateKey().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_WIREGUARD_PRIVATE_KEY), privateKey());
        setting.insert(QLatin1String(NM_SETTING_WIREGUARD_PRIVATE_KEY_FLAGS), (int)privateKeyFlags());
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::WireguardSetting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    dbg.nospace() << NM_SETTING_WIREGUARD_FWMARK << ": " << setting.fwmark() << '\n';
    dbg.nospace() << NM_SETTING_WIREGUARD_LISTEN_PORT << ": " << setting.listenPort() << '\n';
    dbg.nospace() << NM_SETTING_WIREGUARD_MTU << ": " << setting.mtu() << '\n';
    dbg.nospace() << NM_SETTING_WIREGUARD_PEER_ROUTES << ": " << setting.peerRoutes() << '\n';
    dbg.nospace() << NM_SETTING_WIREGUARD_PEERS << ": " << setting.peers() << '\n';
    dbg.nospace() << NM_SETTING_WIREGUARD_PRIVATE_KEY << ": " << setting.privateKey() << '\n';
    dbg.nospace() << NM_SETTING_WIREGUARD_PRIVATE_KEY_FLAGS << ": " << setting.privateKeyFlags() << '\n';

    return dbg.maybeSpace();
}
