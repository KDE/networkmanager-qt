/*
    Copyright 2019 Jan Grulich <jgrulich@redhat.com>

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

#ifndef NETWORKMANAGERQT_WIREGUARD_DEVICE_H
#define NETWORKMANAGERQT_WIREGUARD_DEVICE_H

#include "device.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

namespace NetworkManager
{

class WireGuardDevicePrivate;

/**
 * A WireGuard device interface
 */
class NETWORKMANAGERQT_EXPORT WireGuardDevice : public Device
{
    Q_OBJECT
    Q_PROPERTY(QByteArray publicKey READ publicKey NOTIFY publicKeyChanged)
    Q_PROPERTY(uint listenPort READ listenPort NOTIFY listenPortChanged)
    Q_PROPERTY(uint fwMark READ fwMark NOTIFY fwMarkChanged)

public:
    typedef QSharedPointer<WireGuardDevice> Ptr;
    typedef QList<Ptr> List;
    explicit WireGuardDevice(const QString &path, QObject *parent = nullptr);
    ~WireGuardDevice() override;

    Type type() const override;

    /**
     * 32-byte public WireGuard key.
     */
    QByteArray publicKey() const;
    /**
     * Local UDP listening port.
     */
    uint listenPort() const;
    /**
     * Optional 32-bit mark used to set routing policy for outgoing encrypted packets. See: ip-rule(8)
     */
    uint fwMark() const;

Q_SIGNALS:
    /**
     * Emitted when the public key of this device has changed
     */
    void publicKeyChanged(const QByteArray &publicKey);
    /**
     * Emitted when the listen port of this device has changed
     */
    void listenPortChanged(uint listenPort);
    /**
     * Emitted when the fwmark of this device have changed
     */
    void fwMarkChanged(uint fwMark);

private:
    Q_DECLARE_PRIVATE(WireGuardDevice)
};

}

#endif

