/*
    SPDX-FileCopyrightText: 2025 Martin Rodriguez Reboredo <yakoyoku@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_WIFIP2PPEER_P_H
#define NETWORKMANAGERQT_WIFIP2PPEER_P_H

#include "accesspoint.h"
#include "dbus/wifip2ppeerinterface.h"
#include "wifip2ppeer.h"

namespace NetworkManager
{
class WifiP2PPeerPrivate : public QObject
{
    Q_OBJECT
public:
    WifiP2PPeerPrivate(const QString &path, WifiP2PPeer *q);

    OrgFreedesktopNetworkManagerWifiP2PPeerInterface iface;
    QString uni;
    AccessPoint::Capabilities flags;
    QString hardwareAddress;
    int lastSeen;
    QString manufacturer;
    QString model;
    QString modelNumber;
    QString name;
    QString serial;
    uchar strength;
    QByteArray wfdIEs;

    NetworkManager::AccessPoint::Capabilities convertCapabilities(int caps);

    Q_DECLARE_PUBLIC(WifiP2PPeer)
    WifiP2PPeer *q_ptr;
private Q_SLOTS:
    void dbusPropertiesChanged(const QString &interfaceName, const QVariantMap &properties, const QStringList &invalidatedProperties);
    void propertiesChanged(const QVariantMap &properties);
};

}

#endif /* NETWORKMANAGERQT_WIFIP2PPEER_P_H */
