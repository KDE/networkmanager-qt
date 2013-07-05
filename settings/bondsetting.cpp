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

#include "bondsetting.h"
#include "bondsetting_p.h"

#include <nm-setting-bond.h>

#include <QtCore/QDebug>

NetworkManager::BondSettingPrivate::BondSettingPrivate():
    name(NM_SETTING_BOND_SETTING_NAME)
{ }

NetworkManager::BondSetting::BondSetting():
    Setting(Setting::Bond),
    d_ptr(new BondSettingPrivate())
{ }

NetworkManager::BondSetting::BondSetting(const Ptr &other):
    Setting(other),
    d_ptr(new BondSettingPrivate())
{
    setInterfaceName(other->interfaceName());
    setOptions(other->options());
}

NetworkManager::BondSetting::~BondSetting()
{
    delete d_ptr;
}

QString NetworkManager::BondSetting::name() const
{
    Q_D(const BondSetting);

    return d->name;
}

void NetworkManager::BondSetting::setInterfaceName(const QString &name)
{
    Q_D(BondSetting);

    d->interfaceName = name;
}

QString NetworkManager::BondSetting::interfaceName() const
{
    Q_D(const BondSetting);

    return d->interfaceName;
}

void NetworkManager::BondSetting::addOption(const QString &option, const QString &value)
{
    Q_D(BondSetting);

    d->options.insert(option, value);
}

void NetworkManager::BondSetting::setOptions(const NMStringMap &options)
{
    Q_D(BondSetting);

    d->options = options;
}

NMStringMap NetworkManager::BondSetting::options() const
{
    Q_D(const BondSetting);

    return d->options;
}

void NetworkManager::BondSetting::fromMap(const QVariantMap &setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_BOND_INTERFACE_NAME))) {
        setInterfaceName(setting.value(QLatin1String(NM_SETTING_BOND_INTERFACE_NAME)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_BOND_OPTIONS))) {
        setOptions(qdbus_cast<NMStringMap>(setting.value(QLatin1String(NM_SETTING_BOND_OPTIONS))));
    }
}

QVariantMap NetworkManager::BondSetting::toMap() const
{
    QVariantMap setting;

    if (!interfaceName().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_BOND_INTERFACE_NAME), interfaceName());
    }

    if (!options().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_BOND_OPTIONS), QVariant::fromValue<NMStringMap>(options()));
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::BondSetting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    dbg.nospace() << NM_SETTING_BOND_INTERFACE_NAME << ": " << setting.interfaceName() << '\n';
    dbg.nospace() << NM_SETTING_BOND_OPTIONS << ": " << setting.options() << '\n';

    return dbg.maybeSpace();
}
