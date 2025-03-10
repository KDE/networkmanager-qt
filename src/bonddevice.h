/*
    SPDX-FileCopyrightText: 2013 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_BOND_DEVICE_H
#define NETWORKMANAGERQT_BOND_DEVICE_H

#include "device.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

namespace NetworkManager
{
class BondDevicePrivate;

/*!
 * \class NetworkManager::BondDevice
 * \inheaderfile NetworkManagerQt/BondDevice
 * \inmodule NetworkManagerQt
 *
 * \brief A bond device interface.
 */
class NETWORKMANAGERQT_EXPORT BondDevice : public Device
{
    Q_OBJECT

    /*!
     * \property NetworkManager::BondDevice::carrier
     */
    Q_PROPERTY(bool carrier READ carrier NOTIFY carrierChanged)

    /*!
     * \property NetworkManager::BondDevice::hwAddress
     */
    Q_PROPERTY(QString hwAddress READ hwAddress NOTIFY hwAddressChanged)

    /*!
     * \property NetworkManager::BondDevice::slaves
     */
    Q_PROPERTY(QStringList slaves READ slaves NOTIFY slavesChanged)

public:
    /*!
     * \typedef NetworkManager::BondDevice::Ptr
     */
    typedef QSharedPointer<BondDevice> Ptr;
    /*!
     * \typedef NetworkManager::BondDevice::List
     */
    typedef QList<Ptr> List;
    /*!
     */
    explicit BondDevice(const QString &path, QObject *parent = nullptr);
    ~BondDevice() override;

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
    Q_DECLARE_PRIVATE(BondDevice)
};

}

#endif
