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

#include "ovsinterfacesetting.h"
#include "ovsinterfacesetting_p.h"

#include <QDebug>

#if !NM_CHECK_VERSION(1, 10, 0)
#define NM_SETTING_OVS_INTERFACE_SETTING_NAME "ovs-interface"
#define NM_SETTING_OVS_INTERFACE_TYPE         "type"
#endif

NetworkManager::OvsInterfaceSettingPrivate::OvsInterfaceSettingPrivate()
    : name(NM_SETTING_OVS_INTERFACE_SETTING_NAME)
{ }

NetworkManager::OvsInterfaceSetting::OvsInterfaceSetting()
    : Setting(Setting::OvsInterface)
    , d_ptr(new OvsInterfaceSettingPrivate())
{ }

NetworkManager::OvsInterfaceSetting::OvsInterfaceSetting(const Ptr &other)
    : Setting(other)
    , d_ptr(new OvsInterfaceSettingPrivate())
{
    setInterfaceType(other->interfaceType());
}

NetworkManager::OvsInterfaceSetting::~OvsInterfaceSetting()
{
    delete d_ptr;
}

QString NetworkManager::OvsInterfaceSetting::name() const
{
    Q_D(const OvsInterfaceSetting);

    return d->name;
}

void NetworkManager::OvsInterfaceSetting::setInterfaceType(const QString &type)
{
    Q_D(OvsInterfaceSetting);

    d->type = type;
}

QString NetworkManager::OvsInterfaceSetting::interfaceType() const
{
    Q_D(const OvsInterfaceSetting);

    return d->type;
}

void NetworkManager::OvsInterfaceSetting::fromMap(const QVariantMap &setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_OVS_INTERFACE_TYPE))) {
        setInterfaceType(setting.value(QLatin1String(NM_SETTING_OVS_INTERFACE_TYPE)).toString());
    }

}

QVariantMap NetworkManager::OvsInterfaceSetting::toMap() const
{
    QVariantMap setting;

    if (!interfaceType().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_OVS_INTERFACE_TYPE), interfaceType());
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::OvsInterfaceSetting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    dbg.nospace() << NM_SETTING_OVS_INTERFACE_TYPE << ": " << setting.type() << '\n';

    return dbg.maybeSpace();
}
