/*
    Copyright 2014 Jan Grulich <jgrulich@redhat.com>

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

#ifndef NETWORKMANAGERQT_GENERICDEVICE_H
#define NETWORKMANAGERQT_GENERICDEVICE_H

#include <networkmanagerqt/networkmanagerqt_export.h>

#include "device.h"

namespace NetworkManager
{

class GenericDevicePrivate;

/**
 * A generic device interface
 */
class NETWORKMANAGERQT_EXPORT GenericDevice : public Device
{
    Q_OBJECT
    Q_PROPERTY(QString hardwareAddress READ hardwareAddress)
    Q_PROPERTY(QString typeDescription READ typeDescription)

public:
    typedef QSharedPointer<GenericDevice> Ptr;
    typedef QList<Ptr> List;
    explicit GenericDevice(const QString &path, QObject *parent = 0);
    virtual ~GenericDevice();
    /**
     * Return the type
     */
    virtual Type type() const Q_DECL_OVERRIDE;
    /**
     * Active hardware address of the device
     */
    QString hardwareAddress() const;
    /**
     * A (non-localized) description of the interface type, if known.
     */
    QString typeDescription() const;

Q_SIGNALS:
    /**
     * Emmited when the hardware address of this device has changed
     */
    void hardwareAddressChanged(const QString &hwAddress);
    /**
     * Emmited when the type description this device has changed
     */
    void permanentHardwareAddressChanged(const QString &permHwAddress);

private:
    Q_DECLARE_PRIVATE(GenericDevice)
};

}

#endif // NETWORKMANAGERQT_GENERICDEVICE_H

