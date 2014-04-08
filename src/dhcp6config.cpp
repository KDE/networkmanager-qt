/*
    Copyright 2011 Lamarque V. Souza <lamarque@kde.org>

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

#include "dhcp6config.h"
#include "dhcp6config_p.h"
#include "manager_p.h"
#include "nmdebug.h"

NetworkManager::Dhcp6ConfigPrivate::Dhcp6ConfigPrivate(const QString &path, QObject *owner) :
    dhcp6Iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus()),
    myPath(path)
{
    Q_UNUSED(owner);
}

NetworkManager::Dhcp6ConfigPrivate::~Dhcp6ConfigPrivate()
{
}

NetworkManager::Dhcp6Config::Dhcp6Config(const QString &path, QObject *owner)
    : d_ptr(new Dhcp6ConfigPrivate(path, owner))
{
    Q_D(Dhcp6Config);
    connect(&d->dhcp6Iface, &OrgFreedesktopNetworkManagerDHCP6ConfigInterface::PropertiesChanged, this, &Dhcp6Config::dhcp6PropertiesChanged);
    d->properties = d->dhcp6Iface.options();
}

NetworkManager::Dhcp6Config::~Dhcp6Config()
{
    delete d_ptr;
}

QString NetworkManager::Dhcp6Config::path() const
{
    Q_D(const Dhcp6Config);
    return d->myPath;
}

QVariantMap NetworkManager::Dhcp6Config::options() const
{
    Q_D(const Dhcp6Config);
    return d->properties;
}

QString NetworkManager::Dhcp6Config::optionValue(const QString &key) const
{
    Q_D(const Dhcp6Config);
    QString value;
    if (d->properties.contains(key)) {
        value = d->properties.value(key).toString();
    }
    return value;
}


void NetworkManager::Dhcp6Config::dhcp6PropertiesChanged(const QVariantMap &properties)
{
    Q_D(Dhcp6Config);
    d->properties = properties;
    emit optionsChanged(d->properties);
}
