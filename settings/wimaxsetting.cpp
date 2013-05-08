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

#include "wimaxsetting.h"
#include "wimaxsetting_p.h"

#include <nm-setting-wimax.h>

#include <QtCore/QDebug>

NetworkManager::Settings::WimaxSettingPrivate::WimaxSettingPrivate():
    name(NM_SETTING_WIMAX_SETTING_NAME)
{ }

NetworkManager::Settings::WimaxSetting::WimaxSetting():
    Setting(Setting::Wimax),
    d_ptr(new WimaxSettingPrivate())
{ }

NetworkManager::Settings::WimaxSetting::WimaxSetting(const Ptr &other):
    Setting(other),
    d_ptr(new WimaxSettingPrivate())
{
    setNetworkName(other->networkName());
    setMacAddress(other->macAddress());
}

NetworkManager::Settings::WimaxSetting::~WimaxSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::WimaxSetting::name() const
{
    Q_D(const WimaxSetting);

    return d->name;
}

void NetworkManager::Settings::WimaxSetting::setNetworkName(const QString& name)
{
    Q_D(WimaxSetting);

    d->networkName = name;
}

QString NetworkManager::Settings::WimaxSetting::networkName() const
{
    Q_D(const WimaxSetting);

    return d->networkName;
}

void NetworkManager::Settings::WimaxSetting::setMacAddress(const QByteArray& address)
{
    Q_D(WimaxSetting);

    d->macAddress = address;
}

QByteArray NetworkManager::Settings::WimaxSetting::macAddress() const
{
    Q_D(const WimaxSetting);

    return d->macAddress;
}

void NetworkManager::Settings::WimaxSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_WIMAX_NETWORK_NAME))) {
        setNetworkName(setting.value(QLatin1String(NM_SETTING_WIMAX_NETWORK_NAME)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIMAX_MAC_ADDRESS))) {
        setMacAddress(setting.value(QLatin1String(NM_SETTING_WIMAX_MAC_ADDRESS)).toByteArray());
    }
}

QVariantMap NetworkManager::Settings::WimaxSetting::toMap() const
{
    QVariantMap setting;

    if (!networkName().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_WIMAX_NETWORK_NAME), networkName());
    }

    if (!macAddress().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_WIMAX_MAC_ADDRESS), macAddress());
    }

    return setting;
}

QDebug NetworkManager::Settings::operator <<(QDebug dbg, const NetworkManager::Settings::WimaxSetting &setting)
{
    dbg.nospace() << static_cast<NetworkManager::Settings::Setting>(setting);

    dbg.nospace() << NM_SETTING_WIMAX_NETWORK_NAME << ": " << setting.networkName() << '\n';
    dbg.nospace() << NM_SETTING_WIMAX_MAC_ADDRESS << ": " << setting.macAddress() << '\n';

    return dbg.maybeSpace();
}
