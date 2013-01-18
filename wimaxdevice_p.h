/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>

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

#ifndef NMQT_WIMAXDEVICE_P_H
#define NMQT_WIMAXDEVICE_P_H

#include "device_p.h"
#include "dbus/nm-device-wimaxinterface.h"

namespace NetworkManager {

class WimaxDevicePrivate : public DevicePrivate
{
public:
    WimaxDevicePrivate(const QString &path, QObject * owner);
    OrgFreedesktopNetworkManagerDeviceWiMaxInterface wimaxIface;
    QString hardwareAddress;
    mutable QMap<QString, WimaxNsp *> nspMap;
    QString activeNsp;
    uint centerFrequency;
    int cinr;
    QString bsid;
    int rssi;
    int txPower;
};

}

#endif

