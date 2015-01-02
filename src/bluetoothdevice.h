/*
    Copyright 2011 Lamarque Souza <lamarque@kde.org>
    Copyright 2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2011-2013 Lamarque V. Souza <lamarque@kde.org>

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

#include "modemdevice.h"

#ifndef NETWORKMANAGERQT_BLUETOOTH_DEVICE_H
#define NETWORKMANAGERQT_BLUETOOTH_DEVICE_H

#include <networkmanagerqt_export.h>

namespace NetworkManager
{

class BluetoothDevicePrivate;

/**
 * A bluetooth device interface
 */
class NETWORKMANAGERQT_EXPORT BluetoothDevice: public ModemDevice
{
    Q_OBJECT

    Q_PROPERTY(uint bluetoothCapabilities READ bluetoothCapabilities)
    Q_PROPERTY(QString hardwareAddress READ hardwareAddress)
    Q_PROPERTY(QString name READ name)
    Q_FLAGS(Capabilities)

public:
    typedef QSharedPointer<BluetoothDevice> Ptr;
    typedef QList<Ptr> List;
    /**
     * Capabilities of the Bluetooth device
     */
    enum Capability {
        NoCapability = 0x0, /**< No special capability */
        Dun = 0x1, /**< Dial Up Networking profile */
        Pan = 0x2  /**< Personal Area Network profile */
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)
    /**
     * Creates a new BluetoothDevice object.
     */
    explicit BluetoothDevice(const QString &path, QObject *parent = 0);
    /**
     * Destroys a BluetoothDevice object.
     */
    virtual ~BluetoothDevice();

    /**
     * Retrieves the capabilities supported by this device.
     *
     * @return the capabilities of the device
     * @see NetworkManager::BluetoothDevice::Capability
     */
    Capabilities bluetoothCapabilities() const;
    /**
     * The hardware address assigned to the bluetooth interface
     */
    QString hardwareAddress() const;
    /**
     * Name of the bluetooth interface
     */
    QString name() const;
    /**
     * The NetworkInterface type.
     *
     * @return the NetworkManager::Device::Type.  This always returns NetworkManager::Device::Bluetooth
     */
    virtual Type type() const Q_DECL_OVERRIDE;

Q_SIGNALS:
    /**
     * Emitted when the BT device @p name changes
     */
    void nameChanged(const QString &name);

private:
    Q_DECLARE_PRIVATE(BluetoothDevice)
};
}
#endif
