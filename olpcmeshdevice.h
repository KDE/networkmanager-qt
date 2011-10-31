/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NM_OLPCMESHDEVICE_H
#define NM_OLPCMESHDEVICE_H

#include "device.h"

#include "nmqt-export.h"

#include <QDBusObjectPath>

namespace NetworkManager
{

class OlpcMeshDevicePrivate;

/**
 * A OLPC mesh interface
 */
class NMQT_EXPORT OlpcMeshDevice : public Device
{
Q_OBJECT
Q_DECLARE_PRIVATE(OlpcMeshDevice)

public:
    /**
     * Creates a new OlpcMeshDevice object.
     *
     * @param path the DBus path of the device
     */
    OlpcMeshDevice(const QString & path, QObject * parent);
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
    Device* companionDevice() const;

protected Q_SLOTS:
    void propertiesChanged(const QVariantMap &);
Q_SIGNALS:
    /**
     * The active channel changed.
     */
    void activeChannelChanged(uint);
    /**
     * The companion changed.
     */
    void companionChanged(Device *);
    /**
     * The device changed its hardware address
     */
    void hardwareAddressChanged(const QString &);
};

} // namespace NetworkManager
#endif //NM_WIMAXDEVICE_H

