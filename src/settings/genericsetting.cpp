/*
    Copyright 2015 Jan Grulich <jgrulich@redhat.com>

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

#include "genericsetting.h"
#include "genericsetting_p.h"

#include "nmdebug.h"

#if !NM_CHECK_VERSION(1, 0, 0)
#include <nm-setting-generic.h>
#endif

NetworkManager::GenericSettingPrivate::GenericSettingPrivate()
    : name(NM_SETTING_GENERIC_SETTING_NAME)
{ }

NetworkManager::GenericSetting::GenericSetting()
    : Setting(Setting::Generic)
    , d_ptr(new GenericSettingPrivate())
{ }

NetworkManager::GenericSetting::GenericSetting(const Ptr &other)
    : Setting(other)
    , d_ptr(new GenericSettingPrivate())
{
}

NetworkManager::GenericSetting::~GenericSetting()
{
    delete d_ptr;
}

QString NetworkManager::GenericSetting::name() const
{
    Q_D(const GenericSetting);

    return d->name;
}

void NetworkManager::GenericSetting::fromMap(const QVariantMap &setting)
{
    Q_UNUSED(setting);
}

QVariantMap NetworkManager::GenericSetting::toMap() const
{
    QVariantMap setting;

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::GenericSetting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    return dbg.maybeSpace();
}
