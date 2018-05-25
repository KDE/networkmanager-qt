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

#ifndef NETWORKMANAGERQT_FAKE_NETWORK_WIRELESS_DEVICE_H
#define NETWORKMANAGERQT_FAKE_NETWORK_WIRELESS_DEVICE_H

#include <QObject>

#include <QDBusObjectPath>
#include <QDBusPendingReply>

#include "../generictypes.h"
#include "../device.h"

#include "accesspoint.h"
#include "device.h"

class WirelessDevice : public Device
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.fakenetwork.Device.Wireless")
public:
    explicit WirelessDevice(QObject *parent = nullptr);
    virtual ~WirelessDevice();

    Q_PROPERTY(QList<QDBusObjectPath> AccessPoints READ accessPoints)
    Q_PROPERTY(QDBusObjectPath ActiveAccessPoint READ activeAccessPoint)
    Q_PROPERTY(uint Bitrate READ bitrate)
    Q_PROPERTY(QString HwAddress READ hwAddress)
    Q_PROPERTY(uint Mode READ mode)
    Q_PROPERTY(QString PermHwAddress READ permHwAddress)
    Q_PROPERTY(uint WirelessCapabilities READ wirelessCapabilities)

    QList<QDBusObjectPath> accessPoints() const;
    QDBusObjectPath activeAccessPoint() const;
    uint bitrate() const;
    QString hwAddress() const;
    uint mode() const;
    QString permHwAddress() const;
    uint wirelessCapabilities() const;

    /* Not part of DBus interface */
    void addAccessPoint(AccessPoint *accessPoint);
    void removeAccessPoint(AccessPoint *accessPoint);
    void setActiveAccessPoint(const QString &activeAccessPoint);
    void setBitrate(uint bitrate);
    void setHwAddress(const QString &hwAddress);
    void setMode(uint mode);
    void setPermHwAddress(const QString &permHwAddress);
    void setState(uint state) override;
    void setWirelessCapabilities(uint capabilities);

public Q_SLOTS:
    Q_SCRIPTABLE QList<QDBusObjectPath> GetAccessPoints();
    Q_SCRIPTABLE QList<QDBusObjectPath> GetAllAccessPoints();
    Q_SCRIPTABLE void RequestScan(const QVariantMap &options);

Q_SIGNALS:
    Q_SCRIPTABLE void AccessPointAdded(const QDBusObjectPath &access_point);
    Q_SCRIPTABLE void AccessPointRemoved(const QDBusObjectPath &access_point);
    Q_SCRIPTABLE void PropertiesChanged(const QVariantMap &properties);

private:
    QMap<QDBusObjectPath, AccessPoint *> m_accessPoints;
    QDBusObjectPath m_activeAccessPoint;
    uint m_bitrate;
    QString m_hwAddress;
    uint m_mode;
    QString m_permHwAddress;
    uint m_wirelessCapabilities;

    /* Not part of DBus interface */
    int m_accessPointCounter;
};

#endif
