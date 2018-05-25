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

#ifndef NETWORKMANAGERQT_VETH_DEVICE_H
#define NETWORKMANAGERQT_VETH_DEVICE_H

#include "device.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

namespace NetworkManager
{

class VethDevicePrivate;

/**
 * A veth device interface
 */
class NETWORKMANAGERQT_EXPORT VethDevice : public Device
{
    Q_OBJECT
    Q_PROPERTY(QString peer READ peer NOTIFY peerChanged)

public:
    typedef QSharedPointer<VethDevice> Ptr;
    typedef QList<Ptr> List;

    explicit VethDevice(const QString &path, QObject *parent = nullptr);
    ~VethDevice() override;

    Type type() const override;

    QString peer() const;

Q_SIGNALS:
    void peerChanged(const QString &peer);

private:
    Q_DECLARE_PRIVATE(VethDevice)
};

}

#endif
