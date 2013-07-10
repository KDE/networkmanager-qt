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

#ifndef NMQT_BLUETOOTH_DEVICE_H
#define NMQT_BLUETOOTH_DEVICE_H

#include "NetworkManagerQt-export.h"

namespace NetworkManager {

class BluetoothDevicePrivate;

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
     * Dun: DUN - Dial Up Networking profile
     * Pan: PAN - Personal Area Network profile
     */
    enum Capability { NoCapability = 0x0, Dun = 0x1, Pan = 0x2 };

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
    virtual Type type() const;

Q_SIGNALS:
    void nameChanged(const QString &name);

protected:
    /**
     * When subclassing make sure to call the parent class method
     * if the property was not useful to your new class
     */
    virtual void propertyChanged(const QString &property, const QVariant &value);

private:
    Q_DECLARE_PRIVATE(BluetoothDevice)
};
}
#endif
