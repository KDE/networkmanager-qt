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

#include "vpnsetting.h"
#include "vpnsetting_p.h"

#include <nm-setting-vpn.h>

#include <QtCore/QDebug>

NetworkManager::VpnSettingPrivate::VpnSettingPrivate():
    name(NM_SETTING_VPN_SETTING_NAME)
{ }

NetworkManager::VpnSetting::VpnSetting():
    Setting(Setting::Vpn),
    d_ptr(new VpnSettingPrivate())
{ }

NetworkManager::VpnSetting::VpnSetting(const Ptr &other):
    Setting(other),
    d_ptr(new VpnSettingPrivate())
{
    setServiceType(other->serviceType());
    setUsername(other->username());
    setData(other->data());
    setSecrets(other->secrets());
}

NetworkManager::VpnSetting::~VpnSetting()
{
    delete d_ptr;
}

QString NetworkManager::VpnSetting::name() const
{
    Q_D(const VpnSetting);

    return d->name;
}

void NetworkManager::VpnSetting::setServiceType(const QString& type)
{
    Q_D(VpnSetting);

    d->serviceType = type;
}

QString NetworkManager::VpnSetting::serviceType() const
{
    Q_D(const VpnSetting);

    return d->serviceType;
}

void NetworkManager::VpnSetting::setUsername(const QString& username)
{
    Q_D(VpnSetting);

    d->username = username;
}

QString NetworkManager::VpnSetting::username() const
{
    Q_D(const VpnSetting);

    return d->username;
}

void NetworkManager::VpnSetting::setData(const NMStringMap& data)
{
    Q_D(VpnSetting);

    d->data = data;
}

NMStringMap NetworkManager::VpnSetting::data() const
{
    Q_D(const VpnSetting);

    return d->data;
}

void NetworkManager::VpnSetting::setSecrets(const NMStringMap& secrets)
{
    Q_D(VpnSetting);

    d->secrets = secrets;
}

NMStringMap NetworkManager::VpnSetting::secrets() const
{
    Q_D(const VpnSetting);

    return d->secrets;
}

void NetworkManager::VpnSetting::secretsFromMap(const QVariantMap& secrets)
{
    if (secrets.contains(QLatin1String(NM_SETTING_VPN_SECRETS))) {
        setSecrets(qdbus_cast<NMStringMap>(secrets.value(QLatin1String(NM_SETTING_VPN_SECRETS))));
    }
}

QVariantMap NetworkManager::VpnSetting::secretsToMap() const
{
    QVariantMap secretsMap;

    if (!secrets().isEmpty()) {
        secretsMap.insert(QLatin1String(NM_SETTING_VPN_SECRETS), QVariant::fromValue<NMStringMap>(secrets()));
    }

    qDebug() << secretsMap;
    return secretsMap;
}

void NetworkManager::VpnSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_VPN_SERVICE_TYPE))) {
        setServiceType(setting.value(QLatin1String(NM_SETTING_VPN_SERVICE_TYPE)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_VPN_USER_NAME))) {
        setUsername(setting.value(QLatin1String(NM_SETTING_VPN_USER_NAME)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_VPN_DATA))) {
        setData(qdbus_cast<NMStringMap>(setting.value(QLatin1String(NM_SETTING_VPN_DATA))));
    }

    if (setting.contains(QLatin1String(NM_SETTING_VPN_SECRETS))) {
        setSecrets(qdbus_cast<NMStringMap>(setting.value(QLatin1String(NM_SETTING_VPN_SECRETS))));
    }
}

QVariantMap NetworkManager::VpnSetting::toMap() const
{
    QVariantMap setting;

    if (!serviceType().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_VPN_SERVICE_TYPE), serviceType());
    }

    if (!username().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_VPN_USER_NAME), username());
    }

    if (!data().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_VPN_DATA), QVariant::fromValue<NMStringMap>(data()));
    }

    if (!secrets().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_VPN_SECRETS), QVariant::fromValue<NMStringMap>(secrets()));
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::VpnSetting &setting)
{
    dbg.nospace() << static_cast<NetworkManager::Setting>(setting);

    dbg.nospace() << NM_SETTING_VPN_SERVICE_TYPE << ": " << setting.serviceType() << '\n';
    dbg.nospace() << NM_SETTING_VPN_USER_NAME << ": " << setting.username() << '\n';
    dbg.nospace() << NM_SETTING_VPN_DATA << ": " << setting.data() << '\n';
    dbg.nospace() << NM_SETTING_VPN_SECRETS << ": " << setting.secrets() << '\n';

    return dbg.maybeSpace();
}
