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

#ifndef NETWORKMANAGERQT_FAKE_NETWORK_ACCESS_POINT_H
#define NETWORKMANAGERQT_FAKE_NETWORK_ACCESS_POINT_H

#include <QObject>

#include <QDBusObjectPath>
#include <QDBusPendingReply>

#include "../generictypes.h"

class AccessPoint : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.fakenetwork.AccessPoint")
public:
    explicit AccessPoint(QObject *parent = nullptr);
    virtual ~AccessPoint();

    Q_PROPERTY(uint Flags READ flags)
    Q_PROPERTY(uint Frequency READ frequency)
    Q_PROPERTY(QString HwAddress READ hwAddress)
    Q_PROPERTY(uint MaxBitrate READ maxBitrate)
    Q_PROPERTY(uint Mode READ mode)
    Q_PROPERTY(uint RsnFlags READ rsnFlags)
    Q_PROPERTY(QByteArray Ssid READ ssid)
    Q_PROPERTY(uchar Strength READ strength)
    Q_PROPERTY(uint WpaFlags READ wpaFlags)

    uint flags() const;
    uint frequency() const;
    QString hwAddress() const;
    uint maxBitrate() const;
    uint mode() const;
    uint rsnFlags() const;
    QByteArray ssid() const;
    uchar strength() const;
    uint wpaFlags() const;

    /* Not part of DBus interface */
    QString accessPointPath() const;
    void setAccessPointPath(const QString &path);
    void setFlags(uint flags);
    void setFrequency(uint frequency);
    void setHwAddress(const QString &hwAddress);
    void setMaxBitrate(uint bitrate);
    void setMode(uint mode);
    void setRsnFlags(uint flags);
    void setSsid(const QByteArray &ssid);
    void setStrength(uchar strength);
    void setWpaFlags(uint flags);

Q_SIGNALS:
    Q_SCRIPTABLE void PropertiesChanged(const QVariantMap &properties);

private:
    QString m_apPath;
    uint m_flags;
    uint m_frequency;
    QString m_hwAddress;
    uint m_maxBitrate;
    uint m_mode;
    uint m_rsnFlags;
    QByteArray m_ssid;
    uchar m_strength;
    uint m_wpaFlags;
};

#endif
