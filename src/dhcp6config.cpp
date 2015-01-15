/*
    Copyright 2011 Lamarque V. Souza <lamarque@kde.org>
    Copyright 2014 Jan Grulich <jgrulich@redhat.com>

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

#include "dhcp6config_p.h"
#include "manager_p.h"
#include "nmdebug.h"

NetworkManager::Dhcp6ConfigPrivate::Dhcp6ConfigPrivate(const QString &path, Dhcp6Config *q)
#ifdef NMQT_STATIC
    : dhcp6Iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    : dhcp6Iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , path(path)
    , q_ptr(q)
{
}

NetworkManager::Dhcp6ConfigPrivate::~Dhcp6ConfigPrivate()
{
}

NetworkManager::Dhcp6Config::Dhcp6Config(const QString &path, QObject *owner)
    : d_ptr(new Dhcp6ConfigPrivate(path, this))
{
    Q_D(Dhcp6Config);
    Q_UNUSED(owner);

    connect(&d->dhcp6Iface, &OrgFreedesktopNetworkManagerDHCP6ConfigInterface::PropertiesChanged, d, &Dhcp6ConfigPrivate::dhcp6PropertiesChanged);
    d->options = d->dhcp6Iface.options();
}

NetworkManager::Dhcp6Config::~Dhcp6Config()
{
    delete d_ptr;
}

QString NetworkManager::Dhcp6Config::path() const
{
    Q_D(const Dhcp6Config);
    return d->path;
}

QVariantMap NetworkManager::Dhcp6Config::options() const
{
    Q_D(const Dhcp6Config);
    return d->options;
}

QString NetworkManager::Dhcp6Config::optionValue(const QString &key) const
{
    Q_D(const Dhcp6Config);
    QString value;
    if (d->options.contains(key)) {
        value = d->options.value(key).toString();
    }
    return value;
}

void NetworkManager::Dhcp6ConfigPrivate::dhcp6PropertiesChanged(const QVariantMap &properties)
{
    Q_Q(Dhcp6Config);

    QVariantMap::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        const QString property = it.key();
        if (property == QLatin1String("Options")) {
            options = it.value().toMap();
            Q_EMIT q->optionsChanged(options);
        } else {
            qCWarning(NMQT) << Q_FUNC_INFO << "Unhandled property" << property;
        }
        ++it;
    }
}
