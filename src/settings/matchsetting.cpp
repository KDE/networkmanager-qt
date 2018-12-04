/*
    Copyright Pranav Gade <pranavgade20@gmail.com>

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

#include "matchsetting.h"
#include "matchsetting_p.h"

#include <QDebug>

#if !NM_CHECK_VERSION(1, 14, 0)
#define NM_SETTING_MATCH_SETTING_NAME      "match"
#define NM_SETTING_MATCH_INTERFACE_NAME    "interface-name"
#endif

NetworkManager::MatchSettingPrivate::MatchSettingPrivate()
    : name(NM_SETTING_MATCH_SETTING_NAME)
{ }

NetworkManager::MatchSetting::MatchSetting()
    : Setting(Setting::Match)
    , d_ptr(new MatchSettingPrivate())
{ }

NetworkManager::MatchSetting::MatchSetting(const Ptr &other)
    : Setting(other)
    , d_ptr(new MatchSettingPrivate())
{
    setInterfaceName(other->interfaceName());
}

NetworkManager::MatchSetting::~MatchSetting()
{
    delete d_ptr;
}

QString NetworkManager::MatchSetting::name() const
{
    Q_D(const MatchSetting);

    return d->name;
}

void NetworkManager::MatchSetting::setInterfaceName(const QStringList &name)
{
    Q_D(MatchSetting);

    d->interfaceName = name;
}

QStringList NetworkManager::MatchSetting::interfaceName() const
{
    Q_D(const MatchSetting);

    return d->interfaceName;
}

void NetworkManager::MatchSetting::fromMap(const QVariantMap &setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_MATCH_INTERFACE_NAME))) {
        setInterfaceName(setting.value(QLatin1String(NM_SETTING_MATCH_INTERFACE_NAME)).toStringList());
    }
}

QVariantMap NetworkManager::MatchSetting::toMap() const
{
    QVariantMap setting;

    if (!interfaceName().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_MATCH_INTERFACE_NAME), interfaceName());
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::MatchSetting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    dbg.nospace() << NM_SETTING_MATCH_INTERFACE_NAME << ": " << setting.interfaceName() << '\n';

    return dbg.maybeSpace();
}
