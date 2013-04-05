/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>

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

#ifndef NMQT_WIREDDEVICE_H
#define NMQT_WIREDDEVICE_H

#include "QtNetworkManager-export.h"

#include "device.h"

namespace NetworkManager
{

class WiredDevicePrivate;

class NMQT_EXPORT WiredDevice : public Device
{
Q_OBJECT
Q_DECLARE_PRIVATE(WiredDevice)
Q_PROPERTY(QString hardwareAddress READ hardwareAddress)
Q_PROPERTY(QString permanentHardwareAddress READ permanentHardwareAddress)
Q_PROPERTY(bool carrier READ carrier NOTIFY carrierChanged)
Q_PROPERTY(int bitRate READ bitRate NOTIFY bitRateChanged)

public:
    typedef QSharedPointer<WiredDevice> Ptr;
    typedef QList<Ptr> List;
    WiredDevice(const QString & path, QObject * parent = 0);
    virtual ~WiredDevice();
    /**
     * Return the type
     */
    virtual Type type() const;
    QString hardwareAddress() const;
    QString permanentHardwareAddress() const;
    int bitRate() const;
    bool carrier() const;
protected Q_SLOTS:
    void wiredPropertiesChanged(const QVariantMap &);
Q_SIGNALS:
    void bitRateChanged(int bitRate);
    void carrierChanged(bool plugged);
    void hardwareAddressChanged(const QString &hwAddress);
    void permanentHardwareAddressChanged(const QString &permHwAddress);
};

}

#endif // NMQT_WIREDDEVICE_H

