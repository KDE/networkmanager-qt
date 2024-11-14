/*
    SPDX-FileCopyrightText: 2011 Lamarque Souza <lamarque@kde.org>
    SPDX-FileCopyrightText: 2011 Will Stephenson <wstephenson@kde.org>
    SPDX-FileCopyrightText: 2011-2013 Lamarque V. Souza <lamarque@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modemdevice.h"

#ifndef NETWORKMANAGERQT_BLUETOOTH_DEVICE_H
#define NETWORKMANAGERQT_BLUETOOTH_DEVICE_H

#include <networkmanagerqt/networkmanagerqt_export.h>

namespace NetworkManager
{
class BluetoothDevicePrivate;

/*!
 * \class NetworkManager::BluetoothDevice
 * \inheaderfile NetworkManagerQt/BluetoothDevice
 * \inmodule NetworkManagerQt
 *
 * \brief A bluetooth device interface.
 */
class NETWORKMANAGERQT_EXPORT BluetoothDevice : public ModemDevice
{
    Q_OBJECT

    /*!
     * \property NetworkManager::BluetoothDevice::bluetoothCapabilities
     */
    Q_PROPERTY(uint bluetoothCapabilities READ bluetoothCapabilities)

    /*!
     * \property NetworkManager::BluetoothDevice::hardwareAddress
     */
    Q_PROPERTY(QString hardwareAddress READ hardwareAddress)

    /*!
     * \property NetworkManager::BluetoothDevice::name
     */
    Q_PROPERTY(QString name READ name)

public:
    /*!
     * \typedef NetworkManager::BluetoothDevice::Ptr
     */
    typedef QSharedPointer<BluetoothDevice> Ptr;
    /*!
     * \typedef NetworkManager::BluetoothDevice::List
     */
    typedef QList<Ptr> List;
    /*!
     *
     * Capabilities of the Bluetooth device
     *
     * \value NoCapability
     *        No special capability
     * \value Dun
     *        Dial Up Networking profile
     * \value Pan
     *        Personal Area Network profile
     */
    enum Capability {
        NoCapability = 0x0,
        Dun = 0x1,
        Pan = 0x2,
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)
    Q_FLAG(Capabilities)
    /*!
     * Creates a new BluetoothDevice object.
     */
    explicit BluetoothDevice(const QString &path, QObject *parent = nullptr);
    /*!
     * Destroys a BluetoothDevice object.
     */
    ~BluetoothDevice() override;

    /*!
     * Retrieves the capabilities supported by this device.
     *
     * \sa NetworkManager::BluetoothDevice::Capability
     */
    Capabilities bluetoothCapabilities() const;
    /*!
     * The hardware address assigned to the bluetooth interface
     */
    QString hardwareAddress() const;
    /*!
     * Name of the bluetooth interface
     */
    QString name() const;
    /*!
     * The NetworkInterface type.
     *
     * Returns the NetworkManager::Device::Type.  This always returns NetworkManager::Device::Bluetooth
     */
    Type type() const override;

Q_SIGNALS:
    /*!
     * Emitted when the BT device \a name changes
     */
    void nameChanged(const QString &name);

private:
    Q_DECLARE_PRIVATE(BluetoothDevice)
};
}
#endif
