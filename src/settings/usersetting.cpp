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

#include "usersetting.h"
#include "usersetting_p.h"

#include <QDebug>

#if !NM_CHECK_VERSION(1, 8, 0)
#define NM_SETTING_USER_SETTING_NAME    "user"
#define NM_SETTING_USER_DATA            "data"
#endif

NetworkManager::UserSettingPrivate::UserSettingPrivate()
    : name(NM_SETTING_USER_SETTING_NAME)
{ }

NetworkManager::UserSetting::UserSetting()
    : Setting(Setting::User)
    , d_ptr(new UserSettingPrivate())
{ }

NetworkManager::UserSetting::UserSetting(const Ptr &other)
    : Setting(other)
    , d_ptr(new UserSettingPrivate())
{
    setData(other->data());
}

NetworkManager::UserSetting::~UserSetting()
{
    delete d_ptr;
}

QString NetworkManager::UserSetting::name() const
{
    Q_D(const UserSetting);

    return d->name;
}

void NetworkManager::UserSetting::addData(const QString &data, const QString &value)
{
    Q_D(UserSetting);

    d->data.insert(data, value);
}

void NetworkManager::UserSetting::setData(const NMStringMap &data)
{
    Q_D(UserSetting);

    d->data = data;
}

NMStringMap NetworkManager::UserSetting::data() const
{
    Q_D(const UserSetting);

    return d->data;
}

void NetworkManager::UserSetting::fromMap(const QVariantMap &setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_USER_DATA))) {
        setData(qdbus_cast<NMStringMap>(setting.value(QLatin1String(NM_SETTING_USER_DATA))));
    }
}

QVariantMap NetworkManager::UserSetting::toMap() const
{
    QVariantMap setting;

    if (!data().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_USER_DATA), QVariant::fromValue<NMStringMap>(data()));
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::UserSetting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    dbg.nospace() << NM_SETTING_USER_DATA << ": " << setting.data() << '\n';

    return dbg.maybeSpace();
}

