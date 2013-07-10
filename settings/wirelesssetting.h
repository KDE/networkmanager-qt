/*
    Copyright 2012-2013  Jan Grulich <jgrulich@redhat.com>

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

#ifndef NMQT_SETTINGS_802_11_WIRELESS_H
#define NMQT_SETTINGS_802_11_WIRELESS_H

#include "../NetworkManagerQt-export.h"
#include "setting.h"

#include <QtCore/QStringList>
#include <QSharedPointer>

namespace NetworkManager
{

class WirelessSettingPrivate;
class NETWORKMANAGERQT_EXPORT WirelessSetting : public Setting
{
public:
    typedef QSharedPointer<WirelessSetting> Ptr;
    typedef QList<Ptr> List;
    enum NetworkMode {Infrastructure, Adhoc, Ap};
    enum FrequencyBand {Automatic, A, Bg};

    WirelessSetting();
    explicit WirelessSetting(const Ptr &setting);
    virtual ~WirelessSetting();

    QString name() const;

    void setSsid(const QByteArray &ssid);
    QByteArray ssid() const;

    void setMode(NetworkMode mode);
    NetworkMode mode() const;

    void setBand(FrequencyBand band);
    FrequencyBand band() const;

    void setChannel(quint32 channel);
    quint32 channel() const;

    void setBssid(const QByteArray &bssid);
    QByteArray bssid() const;

    void setRate(quint32 rate);
    quint32 rate() const;

    void setTxPower(quint32 power);
    quint32 txPower() const;

    void setMacAddress(const QByteArray &address);
    QByteArray macAddress() const;

    void setClonedMacAddress(const QByteArray &address);
    QByteArray clonedMacAddress() const;

    void setMacAddressBlacklist(const QStringList &list);
    QStringList macAddressBlacklist() const;

    void setMtu(quint32 mtu);
    quint32 mtu() const;

    void setSeenBssids(const QStringList &list);
    QStringList seenBssids() const;

    void setSecurity(const QString &security);
    QString security() const;

    void setHidden(bool hidden);
    bool hidden() const;

    void fromMap(const QVariantMap &setting);

    QVariantMap toMap() const;

protected:
    WirelessSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(WirelessSetting)
};

QDebug operator<<(QDebug dbg, const WirelessSetting &setting);

}

#endif // NMQT_SETTINGS_802_11_WIRELESS_H

