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

#ifndef NMQT_DEVICE_P_H
#define NMQT_DEVICE_P_H

#include "dbus/nm-deviceinterface.h"
#include "nmpropertyhelper.h"

namespace NetworkManager
{

class NetworkManagerPrivate;

class DevicePrivate
{
public:
    DevicePrivate( const QString & path, QObject * owner );
    virtual ~DevicePrivate();
    OrgFreedesktopNetworkManagerDeviceInterface deviceIface;
    Device::Capabilities capabilities;
    QString uni;
    QString udi;
    int designSpeed;
    Device::State connectionState;
    bool managed;
    IpConfig ipV4Config;
    IpConfig ipV6Config;
    QString driver;
    int ipV4Address;
    QString interfaceName;
    bool firmwareMissing;
    Dhcp4Config * dhcp4Config;
    Dhcp6Config * dhcp6Config;
    QString driverVersion;
    QString firmwareVersion;
    bool autoconnect;
    Device::StateChangeReason reason;

    static NetworkManager::Device::Capabilities convertCapabilities(uint);
    static NetworkManager::Device::State convertState(uint);
    static NetworkManager::Device::StateChangeReason convertReason(uint);
    //NMPropertyHelper propHelper;
};

} // namespace NetworkManager
#endif

