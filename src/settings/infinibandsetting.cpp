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

#include "infinibandsetting.h"
#include "infinibandsetting_p.h"

#include <QDebug>

NetworkManager::InfinibandSettingPrivate::InfinibandSettingPrivate()
    : name(NM_SETTING_INFINIBAND_SETTING_NAME)
    , mtu(0)
    , transportMode(InfinibandSetting::Unknown)
    , pKey(-1)
{ }

NetworkManager::InfinibandSetting::InfinibandSetting()
    : Setting(Setting::Infiniband)
    , d_ptr(new InfinibandSettingPrivate())
{ }

NetworkManager::InfinibandSetting::InfinibandSetting(const NetworkManager::InfinibandSetting::Ptr &other)
    : Setting(other)
    , d_ptr(new InfinibandSettingPrivate())
{
    setMacAddress(other->macAddress());
    setMtu(other->mtu());
    setTransportMode(other->transportMode());
    setPKey(other->pKey());
    setParent(other->parent());
}

NetworkManager::InfinibandSetting::~InfinibandSetting()
{
    delete d_ptr;
}

QString NetworkManager::InfinibandSetting::name() const
{
    Q_D(const InfinibandSetting);

    return d->name;
}

void NetworkManager::InfinibandSetting::setMacAddress(const QByteArray &address)
{
    Q_D(InfinibandSetting);

    d->macAddress = address;
}

QByteArray NetworkManager::InfinibandSetting::macAddress() const
{
    Q_D(const InfinibandSetting);

    return d->macAddress;
}

void NetworkManager::InfinibandSetting::setMtu(quint32 mtu)
{
    Q_D(InfinibandSetting);

    d->mtu = mtu;
}

quint32 NetworkManager::InfinibandSetting::mtu() const
{
    Q_D(const InfinibandSetting);

    return d->mtu;
}

void NetworkManager::InfinibandSetting::setTransportMode(NetworkManager::InfinibandSetting::TransportMode mode)
{
    Q_D(InfinibandSetting);

    d->transportMode = mode;
}

NetworkManager::InfinibandSetting::TransportMode NetworkManager::InfinibandSetting::transportMode() const
{
    Q_D(const InfinibandSetting);

    return d->transportMode;
}

void NetworkManager::InfinibandSetting::setPKey(qint32 key)
{
    Q_D(InfinibandSetting);

    d->pKey = key;
}

qint32 NetworkManager::InfinibandSetting::pKey() const
{
    Q_D(const InfinibandSetting);

    return d->pKey;
}

void NetworkManager::InfinibandSetting::setParent(const QString &parent)
{
    Q_D(InfinibandSetting);

    d->parent = parent;
}

QString NetworkManager::InfinibandSetting::parent() const
{
    Q_D(const InfinibandSetting);

    return d->parent;
}

void NetworkManager::InfinibandSetting::fromMap(const QVariantMap &setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_INFINIBAND_MAC_ADDRESS))) {
        setMacAddress(setting.value(QLatin1String(NM_SETTING_INFINIBAND_MAC_ADDRESS)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_INFINIBAND_MTU))) {
        setMtu(setting.value(QLatin1String(NM_SETTING_INFINIBAND_MTU)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_INFINIBAND_TRANSPORT_MODE))) {
        QString mode = setting.value(QLatin1String(NM_SETTING_INFINIBAND_TRANSPORT_MODE)).toString();

        if (mode == "datagram") {
            setTransportMode(Datagram);
        } else if (mode == "connected") {
            setTransportMode(Connected);
        }
    }

    if (setting.contains(QLatin1String(NM_SETTING_INFINIBAND_P_KEY))) {
        setPKey(setting.value(QLatin1String(NM_SETTING_INFINIBAND_P_KEY)).toInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_INFINIBAND_PARENT))) {
        setParent(setting.value(QLatin1String(NM_SETTING_INFINIBAND_PARENT)).toString());
    }
}

QVariantMap NetworkManager::InfinibandSetting::toMap() const
{
    QVariantMap setting;

    if (!macAddress().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_INFINIBAND_MAC_ADDRESS), macAddress());
    }

    if (mtu()) {
        setting.insert(QLatin1String(NM_SETTING_INFINIBAND_MTU), mtu());
    }

    if (transportMode() != Unknown) {
        if (transportMode() == Datagram) {
            setting.insert(QLatin1String(NM_SETTING_INFINIBAND_TRANSPORT_MODE), "datagram");
        } else if (transportMode() == Connected) {
            setting.insert(QLatin1String(NM_SETTING_INFINIBAND_TRANSPORT_MODE), "connected");
        }
    }

    if (pKey() != -1) {
        setting.insert(QLatin1String(NM_SETTING_INFINIBAND_P_KEY), pKey());
    }

    if (!parent().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_INFINIBAND_PARENT), parent());
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::InfinibandSetting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    dbg.nospace() << NM_SETTING_INFINIBAND_MAC_ADDRESS << ": " << setting.macAddress() << '\n';
    dbg.nospace() << NM_SETTING_INFINIBAND_MTU << ": " << setting.mtu() << '\n';
    dbg.nospace() << NM_SETTING_INFINIBAND_TRANSPORT_MODE << ": " << setting.transportMode() << '\n';
    dbg.nospace() << NM_SETTING_INFINIBAND_P_KEY << ": " << setting.pKey() << '\n';
    dbg.nospace() << NM_SETTING_INFINIBAND_PARENT << ": " << setting.parent() << '\n';

    return dbg.maybeSpace();
}
