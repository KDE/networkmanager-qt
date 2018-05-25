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

#ifndef NETWORKMANAGERQT_GRE_DEVICE_H
#define NETWORKMANAGERQT_GRE_DEVICE_H

#include "device.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

namespace NetworkManager
{

class GreDevicePrivate;

/**
 * A gre device interface
 * @deprecated renamed to IpTunnelDevice
 */
class NETWORKMANAGERQT_EXPORT GreDevice : public Device
{
    Q_OBJECT
    Q_PROPERTY(ushort inputFlags READ inputFlags NOTIFY inputFlagsChanged)
    Q_PROPERTY(ushort outputFlags READ outputFlags NOTIFY outputFlagsChanged)
    Q_PROPERTY(uint inputKey READ inputKey NOTIFY inputKeyChanged)
    Q_PROPERTY(uint outputKey READ outputKey NOTIFY outputKeyChanged)
    Q_PROPERTY(QString localEnd READ localEnd NOTIFY localEndChanged)
    Q_PROPERTY(QString remoteEnd READ remoteEnd NOTIFY remoteEndChanged)
    Q_PROPERTY(QString parent READ parent NOTIFY parentChanged)
    Q_PROPERTY(bool pathMtuDiscovery READ pathMtuDiscovery NOTIFY pathMtuDiscoveryChanged)
    Q_PROPERTY(uchar tos READ tos NOTIFY tosChanged)
    Q_PROPERTY(uchar ttl READ ttl NOTIFY ttlChanged)

public:
    typedef QSharedPointer<GreDevice> Ptr;
    typedef QList<Ptr> List;
    explicit GreDevice(const QString &path, QObject *parent = nullptr);
    virtual ~GreDevice();

    Type type() const override;

    ushort inputFlags() const;
    ushort outputFlags() const;
    uint inputKey() const;
    uint outputKey() const;
    QString localEnd() const;
    QString remoteEnd() const;
    QString parent() const;
    bool pathMtuDiscovery() const;
    uchar tos() const;
    uchar ttl() const;

Q_SIGNALS:
    void inputFlagsChanged(ushort inputflags);
    void outputFlagsChanged(ushort outputFlags);
    void inputKeyChanged(uint inputKey);
    void outputKeyChanged(uint outputKey);
    void localEndChanged(const QString &localEnd);
    void remoteEndChanged(const QString &remoteEnd);
    void parentChanged(const QString &parent);
    void pathMtuDiscoveryChanged(bool pathMtuDiscovery);
    void tosChanged(uchar tos);
    void ttlChanged(uchar ttl);

private:
    Q_DECLARE_PRIVATE(GreDevice)
};

}

#endif
