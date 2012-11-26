/*
    Copyright 2012  Jan Grulich <jgrulich@redhat.com>

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

#include "setting.h"

namespace NetworkManager
{
namespace Settings
{

    class SettingPrivate
    {
    public:
	SettingPrivate();
	virtual ~SettingPrivate();

	Setting::SettingsType type;
	bool initialized;
    };
}
}

NetworkManager::Settings::SettingPrivate::SettingPrivate():
    type(),
    initialized(false)
{ }

NetworkManager::Settings::SettingPrivate::~SettingPrivate()
{ }

NetworkManager::Settings::Setting::Setting():
    d_ptr(new SettingPrivate())
{ }

NetworkManager::Settings::Setting::Setting(NetworkManager::Settings::Setting* setting):
    d_ptr(new SettingPrivate())
{
    setInitialized(setting->isNull());
    setType(setting->type());
}

void NetworkManager::Settings::Setting::setInitialized(const bool initialized)
{
    Q_D(Setting);

    d->initialized = initialized;
}

bool NetworkManager::Settings::Setting::isNull() const
{
    Q_D(const Setting);

    return d->initialized;
}

void NetworkManager::Settings::Setting::setType(const NetworkManager::Settings::Setting::SettingsType type)
{
    Q_D(Setting);

    d->type = type;
}

NetworkManager::Settings::Setting::SettingsType NetworkManager::Settings::Setting::type() const
{
    Q_D(const Setting);

    return d->type;
}

NetworkManager::Settings::Setting::~Setting()
{
    delete d_ptr;
}
