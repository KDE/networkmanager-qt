/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NM_DEVICE_P_H
#define NM_DEVICE_P_H

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
    //bool isLinkUp;
    QString uni;
    QString udi;
    int designSpeed;
    Device::State connectionState;
    bool managed;
    IPv4Config ipV4Config;
    IPv6Config ipV6Config;
    QString driver;
    int ipV4Address;
    QString interfaceName;
    bool firmwareMissing;
    NetworkManagerPrivate * manager;

    static NetworkManager::Device::Capabilities convertCapabilities(uint);
    static NetworkManager::Device::State convertState(uint);
    static NetworkManager::Device::StateChangeReason convertReason(uint);
    //NMPropertyHelper propHelper;
};

} // namespace NetworkManager
#endif

