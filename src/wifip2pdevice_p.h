/*
    SPDX-FileCopyrightText: 2025 Martin Rodriguez Reboredo <yakoyoku@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_WIFIP2PDEVICE_P_H
#define NETWORKMANAGERQT_WIFIP2PDEVICE_P_H

#include "dbus/wifip2pdeviceinterface.h"
#include "device_p.h"
#include "wifip2pdevice.h"
#include "wifip2ppeer.h"

namespace NetworkManager
{
class WifiP2PDevicePrivate : public DevicePrivate
{
    Q_OBJECT
public:
    WifiP2PDevicePrivate(const QString &path, WifiP2PDevice *q);

    OrgFreedesktopNetworkManagerDeviceWifiP2PInterface wifip2pIface;
    QString hardwareAddress;
    QMap<QString, WifiP2PPeer::Ptr> peerMap;

    Q_DECLARE_PUBLIC(WifiP2PDevice)
protected:
    /**
     * When subclassing make sure to call the parent class method
     * if the property was not useful to your new class
     */
    void propertyChanged(const QString &property, const QVariant &value) override;

private:
    void peerAdded(const QDBusObjectPath &peer);
    void peerRemoved(const QDBusObjectPath &peer);
};

}

#endif /* NETWORKMANAGERQT_WIFIP2PDEVICE_P_H */
