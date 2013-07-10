/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
Copyright 2011-2013 Lamarque Souza <lamarque@kde.org>

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

#ifndef NMQT_OLPCMESHDEVICE_H
#define NMQT_OLPCMESHDEVICE_H

#include "device.h"

#include "NetworkManagerQt-export.h"

#include <QDBusObjectPath>

namespace NetworkManager
{

class OlpcMeshDevicePrivate;

/**
 * A OLPC mesh interface
 */
class NETWORKMANAGERQT_EXPORT OlpcMeshDevice : public Device
{
    Q_OBJECT

public:
    typedef QSharedPointer<OlpcMeshDevice> Ptr;
    typedef QList<Ptr> List;
    /**
     * Creates a new OlpcMeshDevice object.
     *
     * @param path the DBus path of the device
     */
    explicit OlpcMeshDevice(const QString &path, QObject *parent = 0);
    /**
     * Destroys a OlpcMeshDevice object.
     */
    ~OlpcMeshDevice();
    /**
     * Return the type
     */
    virtual Type type() const;
    /**
     * Currently active channel.
     */
    uint activeChannel() const;
    /**
     * The hardware address currently used by the network interface
     */
    QString hardwareAddress() const;
    /**
     * The current companion device.
     */
    Device::Ptr companionDevice() const;

Q_SIGNALS:
    /**
     * The active channel changed.
     */
    void activeChannelChanged(uint);
    /**
     * The companion changed.
     */
    void companionChanged(const Device::Ptr &device);
    /**
     * The device changed its hardware address
     */
    void hardwareAddressChanged(const QString &);

protected:
    /**
     * When subclassing make sure to call the parent class method
     * if the property was not useful to your new class
     */
    virtual void propertyChanged(const QString &property, const QVariant &value);

private:
    Q_DECLARE_PRIVATE(OlpcMeshDevice)
};

} // namespace NetworkManager
#endif //NMQT_OLPCMESHDEVICE_H

