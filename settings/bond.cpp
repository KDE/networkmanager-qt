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

#include "bond.h"
#include "bond_p.h"

#include <nm-setting-bond.h>

#include <QtCore/QDebug>

NetworkManager::Settings::BondSettingPrivate::BondSettingPrivate():
    name(QString("bond")),
    interfaceName(QString()),
    options(QStringMap())
{ }

NetworkManager::Settings::BondSetting::BondSetting():
    Setting(Setting::Bond),
    d_ptr(new BondSettingPrivate())
{ }

NetworkManager::Settings::BondSetting::BondSetting(BondSetting * setting):
    Setting(setting),
    d_ptr(new BondSettingPrivate())
{
    setInterfaceName(setting->interfaceName());
    setOptions(setting->options());
}

NetworkManager::Settings::BondSetting::~BondSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::BondSetting::name() const
{
    Q_D(const BondSetting);

    return d->name;
}

void NetworkManager::Settings::BondSetting::setInterfaceName(const QString& name)
{
    Q_D(BondSetting);

    d->interfaceName = name;
}

QString NetworkManager::Settings::BondSetting::interfaceName() const
{
    Q_D(const BondSetting);

    return d->interfaceName;
}

void NetworkManager::Settings::BondSetting::addOption(const QString& option, const QString& value)
{
    Q_D(BondSetting);

    d->options.insert(option, value);
}

void NetworkManager::Settings::BondSetting::setOptions(const QStringMap& options)
{
    Q_D(BondSetting);

    d->options = options;
}

QStringMap NetworkManager::Settings::BondSetting::options() const
{
    Q_D(const BondSetting);

    return d->options;
}

bool NetworkManager::Settings::BondSetting::hasSecrets() const
{
    return false;
}

void NetworkManager::Settings::BondSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_BOND_INTERFACE_NAME))) {
        setInterfaceName(setting.value(QLatin1String(NM_SETTING_BOND_INTERFACE_NAME)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_BOND_OPTIONS))) {
        setOptions(setting.value(QLatin1String(NM_SETTING_BOND_OPTIONS)).value<QStringMap>());
    }
}

QVariantMap NetworkManager::Settings::BondSetting::toMap() const
{
    QVariantMap setting;

    if (!interfaceName().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_BOND_INTERFACE_NAME), interfaceName());
    }

    if (!options().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_BOND_OPTIONS), QVariant::fromValue<QStringMap>(options()));
    }

    return setting;
}

void NetworkManager::Settings::BondSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << "INTERFACE NAME - " << interfaceName();
    qDebug() << "OPTIONS - " << options();
}
