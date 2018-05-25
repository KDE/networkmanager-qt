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

#ifndef NETWORKMANAGERQT_WIRELESS_SETTING_H
#define NETWORKMANAGERQT_WIRELESS_SETTING_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "setting.h"

#include <QtCore/QStringList>
#include <QSharedPointer>

namespace NetworkManager
{

class WirelessSettingPrivate;

/**
 * Represents wireless setting
 */
class NETWORKMANAGERQT_EXPORT WirelessSetting : public Setting
{
public:
    typedef QSharedPointer<WirelessSetting> Ptr;
    typedef QList<Ptr> List;

    enum NetworkMode {
        Infrastructure,
        Adhoc,
        Ap
    };

    enum FrequencyBand {
        Automatic,
        A,
        Bg
    };

    enum PowerSave {
        PowerSaveDefault = 0,
        PowerSaveIgnore = 1,
        PowerSaveDisable = 2,
        PowerSaveEnable = 3
    };

    WirelessSetting();
    explicit WirelessSetting(const Ptr &setting);
    ~WirelessSetting() override;

    QString name() const override;

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

    void setAssignedMacAddress(const QString &assignedMacAddress);
    QString assignedMacAddress() const;

    void setMacAddress(const QByteArray &address);
    QByteArray macAddress() const;

    /// @deprecated since NM 1.4.0, will internally use assigned-mac-address property
    void setClonedMacAddress(const QByteArray &address);
    QByteArray clonedMacAddress() const;

    void setGenerateMacAddressMask(const QString &macAddressMask);
    QString generateMacAddressMask() const;

    void setMacAddressBlacklist(const QStringList &list);
    QStringList macAddressBlacklist() const;

    void setMacAddressRandomization(MacAddressRandomization randomization);
    MacAddressRandomization macAddressRandomization() const;

    void setMtu(quint32 mtu);
    quint32 mtu() const;

    void setSeenBssids(const QStringList &list);
    QStringList seenBssids() const;

    void setPowerSave(PowerSave powersave);
    PowerSave powerSave() const;

    /// @deprecated since NM 1.0.0
    void setSecurity(const QString &security);
    QString security() const;

    void setHidden(bool hidden);
    bool hidden() const;

    void fromMap(const QVariantMap &setting) override;

    QVariantMap toMap() const override;

protected:
    WirelessSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(WirelessSetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const WirelessSetting &setting);

}

#endif // NETWORKMANAGERQT_WIRELESS_SETTING_H

