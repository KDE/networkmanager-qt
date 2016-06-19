/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>

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

#ifndef NETWORKMANAGERQT_DEVICE_P_H
#define NETWORKMANAGERQT_DEVICE_P_H

#include "dbus/nm-deviceinterface.h"
#include "device.h"

namespace NetworkManager
{

class NetworkManagerPrivate;

class DevicePrivate : public QObject
{
    Q_OBJECT
public:
    explicit DevicePrivate(const QString &path, Device *q);
    virtual ~DevicePrivate();

    void init();

    OrgFreedesktopNetworkManagerDeviceInterface deviceIface;
    Device::Capabilities capabilities;
    QString uni;
    QString udi;
    QString activeConnection;
    int designSpeed;
    Device::Type deviceType;
    Device::State connectionState;
    bool managed;
    mutable IpConfig ipV4Config;
    QString ipV4ConfigPath;
    mutable IpConfig ipV6Config;
    QString ipV6ConfigPath;
    QString driver;
    QHostAddress ipV4Address;
    QString interfaceName;
    QString ipInterface;
    bool firmwareMissing;
    mutable Dhcp4Config::Ptr dhcp4Config;
    QString dhcp4ConfigPath;
    mutable Dhcp6Config::Ptr dhcp6Config;
    QString dhcp6ConfigPath;
    QString driverVersion;
    QString firmwareVersion;
    QStringList availableConnections;
    bool autoconnect;
    Device::StateChangeReason reason;
    QString physicalPortId;
    uint mtu;
    bool nmPluginMissing;
    Device::MeteredStatus metered;

    static NetworkManager::Device::MeteredStatus convertMeteredStatus(uint);
    static NetworkManager::Device::Capabilities convertCapabilities(uint);
    static NetworkManager::Device::State convertState(uint);
    static NetworkManager::Device::StateChangeReason convertReason(uint);

    Q_DECLARE_PUBLIC(Device)
    Device *q_ptr;
public:
    /**
     * When subclassing make sure to call the parent class method
     * if the property was not useful to your new class
     */
    virtual void propertyChanged(const QString &property, const QVariant &value);

public Q_SLOTS:
    void deviceStateChanged(uint, uint, uint);
    void propertiesChanged(const QVariantMap &properties);
};

} // namespace NetworkManager
#endif
