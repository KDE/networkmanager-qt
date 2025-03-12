/*
    SPDX-FileCopyrightText: 2013 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_BRIDGE_DEVICE_H
#define NETWORKMANAGERQT_BRIDGE_DEVICE_H

#include "device.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

namespace NetworkManager
{
class BridgeDevicePrivate;

/*!
 * \class NetworkManager::BridgeDevice
 * \inheaderfile NetworkManagerQt/BridgeDevice
 * \inmodule NetworkManagerQt
 *
 * \brief A bridge device interface.
 */
class NETWORKMANAGERQT_EXPORT BridgeDevice : public Device
{
    Q_OBJECT

    /*!
     * \property NetworkManager::BridgeDevice::carrier
     */
    Q_PROPERTY(bool carrier READ carrier NOTIFY carrierChanged)

    /*!
     * \property NetworkManager::BridgeDevice::hwAddress
     */
    Q_PROPERTY(QString hwAddress READ hwAddress NOTIFY hwAddressChanged)

    /*!
     * \property NetworkManager::BridgeDevice::slaves
     */
    Q_PROPERTY(QStringList slaves READ slaves NOTIFY slavesChanged)

public:
    /*!
     * \typedef NetworkManager::BridgeDevice::Ptr
     */
    typedef QSharedPointer<BridgeDevice> Ptr;
    /*!
     * \typedef NetworkManager::BridgeDevice::List
     */
    typedef QList<Ptr> List;
    /*!
     */
    explicit BridgeDevice(const QString &path, QObject *parent = nullptr);
    ~BridgeDevice() override;

    Type type() const override;

    /*!
     * Indicates whether the physical carrier is found
     */
    bool carrier() const;
    /*!
     * Hardware address of the device
     */
    QString hwAddress() const;
    /*!
     * Array of object paths representing devices which are currently slaved to this device
     */
    QStringList slaves() const;

Q_SIGNALS:
    /*!
     * Emitted when the carrier of this device has changed
     */
    void carrierChanged(bool plugged);
    /*!
     * Emitted when the hardware address of this device has changed
     */
    void hwAddressChanged(const QString &address);
    /*!
     * Emitted when the slaves of this device have changed
     */
    void slavesChanged(const QStringList &slaves);

private:
    Q_DECLARE_PRIVATE(BridgeDevice)
};

}

#endif
