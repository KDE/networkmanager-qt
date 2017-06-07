/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2011-2013 Lamarque V. Souza <lamarque@kde.org>
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

#ifndef NETWORKMANAGERQT_WIREDDEVICE_H
#define NETWORKMANAGERQT_WIREDDEVICE_H

#include <networkmanagerqt/networkmanagerqt_export.h>

#include "device.h"

namespace NetworkManager
{

class WiredDevicePrivate;

/**
 * A wired device interface
 */
class NETWORKMANAGERQT_EXPORT WiredDevice : public Device
{
    Q_OBJECT
    Q_PROPERTY(QString hardwareAddress READ hardwareAddress)
    Q_PROPERTY(QString permanentHardwareAddress READ permanentHardwareAddress)
    Q_PROPERTY(bool carrier READ carrier NOTIFY carrierChanged)
    Q_PROPERTY(int bitRate READ bitRate NOTIFY bitRateChanged)

public:
    typedef QSharedPointer<WiredDevice> Ptr;
    typedef QList<Ptr> List;
    explicit WiredDevice(const QString &path, QObject *parent = nullptr);
    virtual ~WiredDevice();
    /**
     * Return the type
     */
    virtual Type type() const Q_DECL_OVERRIDE;
    /**
     * Active hardware address of the device
     */
    QString hardwareAddress() const;
    /**
     * Permanent hardware address of the device
     */
    QString permanentHardwareAddress() const;
    /**
     * Design speed of the device, in megabits/second (Mb/s)
     */
    int bitRate() const;
    /**
     * Indicates whether the physical carrier is found (e.g. whether a cable is plugged in or not)
     */
    bool carrier() const;
    /**
     * Array of S/390 subchannels for S/390 or z/Architecture devices
     * @note always returns empty array in runtime NM < 1.2.0
     */
    QStringList s390SubChannels() const;

Q_SIGNALS:
    /**
     * Emitted when the design speed of the device has changed
     */
    void bitRateChanged(int bitRate);
    /**
     * Emitted when the carrier of this device has changed
     */
    void carrierChanged(bool plugged);
    /**
     * Emitted when the hardware address of this device has changed
     */
    void hardwareAddressChanged(const QString &hwAddress);
    /**
     * Emitted when the permanent hardware address of this device has changed
     */
    void permanentHardwareAddressChanged(const QString &permHwAddress);
    /*
     * Emitted when the array of s390SubChannels has changed
     * @note never emitted in runtime NM < 1.2.0
     */
    void s390SubChannelsChanged(const QStringList &channels);

private:
    Q_DECLARE_PRIVATE(WiredDevice)
};

}

#endif // NETWORKMANAGERQT_WIREDDEVICE_H

