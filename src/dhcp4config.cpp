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

#include "dhcp4config.h"
#include "dhcp4config_p.h"
#include "manager_p.h"
#include "nmdebug.h"

NetworkManager::Dhcp4ConfigPrivate::Dhcp4ConfigPrivate(const QString &path, Dhcp4Config *q)
#ifdef NMQT_STATIC
    : dhcp4Iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    : dhcp4Iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , myPath(path)
    , q_ptr(q)
{
}

NetworkManager::Dhcp4ConfigPrivate::~Dhcp4ConfigPrivate()
{
}

NetworkManager::Dhcp4Config::Dhcp4Config(const QString &path, QObject *owner)
    : d_ptr(new Dhcp4ConfigPrivate(path, this))
{
    Q_D(Dhcp4Config);
    Q_UNUSED(owner);

    connect(&d->dhcp4Iface, &OrgFreedesktopNetworkManagerDHCP4ConfigInterface::PropertiesChanged, d, &Dhcp4ConfigPrivate::dhcp4PropertiesChanged);
    d->options = d->dhcp4Iface.options();
}

NetworkManager::Dhcp4Config::~Dhcp4Config()
{
    delete d_ptr;
}

QString NetworkManager::Dhcp4Config::path() const
{
    Q_D(const Dhcp4Config);
    return d->myPath;
}

QVariantMap NetworkManager::Dhcp4Config::options() const
{
    Q_D(const Dhcp4Config);
    return d->options;
}

QString NetworkManager::Dhcp4Config::optionValue(const QString &key) const
{
    Q_D(const Dhcp4Config);
    QString value;
    if (d->options.contains(key)) {
        value = d->options.value(key).toString();
    }
    return value;
}

void NetworkManager::Dhcp4ConfigPrivate::dhcp4PropertiesChanged(const QVariantMap &properties)
{
    Q_Q(Dhcp4Config);

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
