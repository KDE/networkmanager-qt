/*
    Copyright 2013 Lukáš Tinkl <ltinkl@redhat.com>
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

#ifndef NETWORKMANAGERQT_TUN_DEVICE_H
#define NETWORKMANAGERQT_TUN_DEVICE_H

#include "device.h"
#include <networkmanagerqt_export.h>

namespace NetworkManager {

class TunDevicePrivate;

class NETWORKMANAGERQT_EXPORT TunDevice : public Device
{
    Q_OBJECT
    Q_PROPERTY(qlonglong owner READ owner NOTIFY ownerChanged)
    Q_PROPERTY(qlonglong group READ group NOTIFY groupChanged)
    Q_PROPERTY(QString mode READ mode NOTIFY modeChanged)
    Q_PROPERTY(bool multiQueue READ multiQueue NOTIFY multiQueueChanged)
    Q_PROPERTY(bool noPi READ noPi NOTIFY noPiChanged)
    Q_PROPERTY(bool vnetHdr READ vnetHdr NOTIFY vnetHdrChanged)

public:
    typedef QSharedPointer<TunDevice> Ptr;
    typedef QList<Ptr> List;
    explicit TunDevice(const QString &path, QObject *parent = 0);
    virtual ~TunDevice();

    virtual Type type() const Q_DECL_OVERRIDE;

    qlonglong owner() const;
    qlonglong group() const;
    QString mode() const;
    bool multiQueue() const;
    bool noPi() const;
    bool vnetHdr() const;

Q_SIGNALS:
    void ownerChanged(qlonglong owner);
    void groupChanged(qlonglong group);
    void modeChanged(const QString &mode);
    void multiQueueChanged(bool multiQueue);
    void noPiChanged(bool noPi);
    void vnetHdrChanged(bool vnetHdr);

private:
    Q_DECLARE_PRIVATE(TunDevice)
};

}

#endif
