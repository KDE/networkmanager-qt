/*
    Copyright 2017 Jan Grulich <jgrulich@redhat.com>

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

#ifndef NETWORKMANAGERQT_DEVICE_STATISTICS_P_H
#define NETWORKMANAGERQT_DEVICE_STATISTICS_P_H

#include "devicestatistics.h"
#include "dbus/devicestatisticsinterface.h"

namespace NetworkManager
{

class DeviceStatisticsPrivate : public QObject
{
    Q_OBJECT
public:
    DeviceStatisticsPrivate(const QString &path, DeviceStatistics *q);

    OrgFreedesktopNetworkManagerDeviceStatisticsInterface iface;
    QString uni;
    uint refreshRateMs;
    qulonglong rxBytes;
    qulonglong txBytes;

    Q_DECLARE_PUBLIC(DeviceStatistics)
    DeviceStatistics *q_ptr;
private Q_SLOTS:
    void dbusPropertiesChanged(const QString &interfaceName, const QVariantMap &properties, const QStringList &invalidatedProperties);
    void propertiesChanged(const QVariantMap &properties);
};

} // namespace NetworkManager

#endif // NETWORKMANAGERQT_DEVICE_STATISTICS_P_H
