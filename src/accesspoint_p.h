/*
    Copyright 2008 Will Stephenson <wstephenson@kde.org>
    Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>
    Copyright 2013-2014 Jan Grulich <jgrulich@redhat.com>

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

#ifndef NETWORKMANAGERQT_ACCESSPOINT_P_H
#define NETWORKMANAGERQT_ACCESSPOINT_P_H

#include "accesspoint.h"
#include "dbus/nm-access-pointinterface.h"

namespace NetworkManager
{

class AccessPointPrivate : public QObject
{
Q_OBJECT
public:
    AccessPointPrivate(const QString &path, AccessPoint *q);

    OrgFreedesktopNetworkManagerAccessPointInterface iface;
    QString uni;
    AccessPoint::Capabilities capabilities;
    AccessPoint::WpaFlags wpaFlags;
    AccessPoint::WpaFlags rsnFlags;
    QString ssid;
    QByteArray rawSsid;
    uint frequency;
    QString hardwareAddress;
    uint maxBitRate;
    AccessPoint::OperationMode mode;
    int signalStrength;

    NetworkManager::AccessPoint::Capabilities convertCapabilities(int caps);
    NetworkManager::AccessPoint::WpaFlags convertWpaFlags(uint theirFlags);

    Q_DECLARE_PUBLIC(AccessPoint)
    AccessPoint *q_ptr;
private Q_SLOTS:
    void propertiesChanged(const QVariantMap &properties);
};

} // namespace NetworkManager

#endif // NETWORKMANAGERQT_ACCESSPOINT_P_H
