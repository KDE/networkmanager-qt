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

#ifndef NETWORKMANAGERQT_DHCP6CONFIG_P_H
#define NETWORKMANAGERQT_DHCP6CONFIG_P_H

#include "dhcp6config.h"
#include "dbus/nm-dhcp6-configinterface.h"

namespace NetworkManager
{

class Dhcp6ConfigPrivate : public QObject
{
    Q_OBJECT
public:
    Dhcp6ConfigPrivate(const QString &path, Dhcp6Config *q);
    virtual ~Dhcp6ConfigPrivate();
    OrgFreedesktopNetworkManagerDHCP6ConfigInterface dhcp6Iface;
    QString path;
    QVariantMap options;

    Q_DECLARE_PUBLIC(Dhcp6Config)
    Dhcp6Config *q_ptr;
protected Q_SLOTS:
    void dhcp6PropertiesChanged(const QVariantMap &);
};

} // namespace NetworkManager

#endif
