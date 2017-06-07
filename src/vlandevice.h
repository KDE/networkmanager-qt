/*
    Copyright 2013 Jan Grulich <jgrulich@redhat.com>

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

#ifndef NETWORKMANAGERQT_VLAN_DEVICE_H
#define NETWORKMANAGERQT_VLAN_DEVICE_H

#include "device.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

namespace NetworkManager
{

class VlanDevicePrivate;

/**
 * A vlan device interface
 */
class NETWORKMANAGERQT_EXPORT VlanDevice : public Device
{
    Q_OBJECT
    Q_PROPERTY(bool carrier READ carrier NOTIFY carrierChanged)
    Q_PROPERTY(QString hwAddress READ hwAddress NOTIFY hwAddressChanged)
    Q_PROPERTY(uint vlanId READ vlanId NOTIFY vlanIdChanged)
    Q_PROPERTY(NetworkManager::Device::Ptr parent READ parent NOTIFY parentChanged)

public:
    typedef QSharedPointer<VlanDevice> Ptr;
    typedef QList<Ptr> List;

    explicit VlanDevice(const QString &path, QObject *parent = nullptr);
    virtual ~VlanDevice();

    virtual Type type() const Q_DECL_OVERRIDE;

    /**
     * Indicates whether the physical carrier is found
     */
    bool carrier() const;
    /**
     * Hardware address of the device
     */
    QString hwAddress() const;
    /**
     * The parent device of this VLAN device
     * @since 5.8.0
     * @note always returns nullptr in runtime NM < 1.0.0
     */
    NetworkManager::Device::Ptr parent() const;
    /**
     * The VLAN ID of this VLAN interface
     */
    uint vlanId() const;

Q_SIGNALS:
    /**
     * Emitted when the carrier of this device has changed
     */
    void carrierChanged(bool plugged);
    /**
     * Emitted when the hardware address of this device has changed
     */
    void hwAddressChanged(const QString &address);
    /**
     * Emitted when the parent device of this device has changed
     * @note never emitted in runtime NM < 1.0.0
     */
    void parentChanged(const QString &path);
    /**
     * Emitted when the VLAN ID of this device has changed
     */
    void vlanIdChanged(uint id);

private:
    Q_DECLARE_PRIVATE(VlanDevice)
};

}

#endif
