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

#ifndef NETWORKMANAGERQT_FAKE_NETWORK_WIRED_DEVICE_H
#define NETWORKMANAGERQT_FAKE_NETWORK_WIRED_DEVICE_H

#include <QObject>

#include <QDBusObjectPath>

#include "../generictypes.h"
#include "../device.h"

#include "device.h"

class WiredDevice : public Device
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.fakenetwork.Device.Wired")
public:
    explicit WiredDevice(QObject *parent = nullptr);
    virtual ~WiredDevice();

    Q_PROPERTY(bool Carrier READ carrier)
    Q_PROPERTY(QString HwAddress READ hwAddress)
    Q_PROPERTY(QString PermHwAddress READ permHwAddress)
    Q_PROPERTY(uint Speed READ speed)

    bool carrier() const;
    QString hwAddress() const;
    QString permHwAddress() const;
    uint speed() const;

    /* Not part of DBus interface */
    void setCarrier(bool carrier);
    void setHwAddress(const QString &hwAddress);
    void setPermanentHwAddress(const QString &permanentHwAddress);
    void setSpeed(uint speed);
    void setState(uint state) override;

Q_SIGNALS:
    Q_SCRIPTABLE void PropertiesChanged(const QVariantMap &properties);

private:
    bool m_carrier;
    QString m_hwAddress;
    QString m_permHwAddress;
    uint m_speed;
};

#endif
