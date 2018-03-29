/*
    Copyright 2014 Lukáš Tinkl <ltinkl@redhat.com>

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

#include "teamsetting.h"
#include "teamsetting_p.h"
#include "manager.h"

#define NM_SETTING_TEAM_INTERFACE_NAME "interface-name"

#include <QtCore/QDebug>

NetworkManager::TeamSettingPrivate::TeamSettingPrivate()
    : name(NM_SETTING_TEAM_SETTING_NAME)
{ }

NetworkManager::TeamSetting::TeamSetting()
    : Setting(Setting::Team)
    , d_ptr(new TeamSettingPrivate())
{ }

NetworkManager::TeamSetting::TeamSetting(const Ptr &other)
    : Setting(other)
    , d_ptr(new TeamSettingPrivate())
{
    setInterfaceName(other->interfaceName());
    setConfig(other->config());
}

NetworkManager::TeamSetting::~TeamSetting()
{
    delete d_ptr;
}

QString NetworkManager::TeamSetting::name() const
{
    Q_D(const TeamSetting);

    return d->name;
}

void NetworkManager::TeamSetting::setInterfaceName(const QString &name)
{
    Q_D(TeamSetting);

    d->interfaceName = name;
}

QString NetworkManager::TeamSetting::interfaceName() const
{
    Q_D(const TeamSetting);

    return d->interfaceName;
}

QString NetworkManager::TeamSetting::config() const
{
    Q_D(const TeamSetting);
    return d->config;
}

void NetworkManager::TeamSetting::setConfig(const QString &config)
{
    Q_D(TeamSetting);
    d->config = config;
}

void NetworkManager::TeamSetting::fromMap(const QVariantMap &setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_TEAM_INTERFACE_NAME))) {
        setInterfaceName(setting.value(QLatin1String(NM_SETTING_TEAM_INTERFACE_NAME)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_TEAM_CONFIG))) {
        setConfig(setting.value(QLatin1String(NM_SETTING_TEAM_CONFIG)).toString());
    }
}

QVariantMap NetworkManager::TeamSetting::toMap() const
{
    QVariantMap setting;

    if (!interfaceName().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_TEAM_INTERFACE_NAME), interfaceName());
    }
    if (!config().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_TEAM_CONFIG), config());
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::TeamSetting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    dbg.nospace() << NM_SETTING_TEAM_INTERFACE_NAME << ": " << setting.interfaceName() << '\n';
    dbg.nospace() << NM_SETTING_TEAM_CONFIG << ": " << setting.config() << '\n';

    return dbg.maybeSpace();
}
