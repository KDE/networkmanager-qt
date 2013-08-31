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
#ifndef NETWORKMANAGERQT_MODEMDEVICE_P_H
#define NETWORKMANAGERQT_MODEMDEVICE_P_H

#include "device_p.h"
#include "dbus/nm-device-modeminterface.h"

namespace NetworkManager
{

class ModemDevicePrivate : public DevicePrivate
{
public:
    explicit ModemDevicePrivate(const QString &path, ModemDevice *q);

    void initModemProperties();

    OrgFreedesktopNetworkManagerDeviceModemInterface modemIface;
    NetworkManager::ModemDevice::Capabilities modemCapabilities;
    NetworkManager::ModemDevice::Capabilities currentCapabilities;
#if WITH_MODEMMANAGERQT
    QString m_modemUdi;
#endif

    Q_DECLARE_PUBLIC(ModemDevice)
    ModemDevice *q_ptr;
};

} // namespace NetworkManager

#endif
