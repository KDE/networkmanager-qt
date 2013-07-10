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

#ifndef NMQT_SETTINGS_SETTING_H
#define NMQT_SETTINGS_SETTING_H

#include "../NetworkManagerQt-export.h"
#include "../generic-types.h"

#include <QtCore/QVariantMap>
#include <QtCore/QStringList>
#include <QtCore/QFlags>
#include <QSharedPointer>

namespace NetworkManager
{

class SettingPrivate;
class NETWORKMANAGERQT_EXPORT Setting
{
public:
    typedef QSharedPointer<Setting> Ptr;
    typedef QList<Ptr> List;
    enum SettingType {Adsl, Cdma, Gsm, Infiniband, Ipv4, Ipv6, Ppp, Pppoe, Security8021x, Serial,
                      Vpn, Wired, Wireless, WirelessSecurity, Bluetooth, OlpcMesh, Vlan, Wimax, Bond, Bridge, BridgePort
                     };
    enum SecretFlagType {None = 0, AgentOwned = 0x01, NotSaved = 0x02, NotRequired = 0x04};
    Q_DECLARE_FLAGS(SecretFlags, SecretFlagType)

    static QString typeAsString(SettingType type);
    static SettingType typeFromString(const QString &type);

    explicit Setting(SettingType type);
    explicit Setting(const Ptr &setting);
    virtual ~Setting();

    /**
     * @brief Must be reimplemented, default implementationd does nothing
     */
    virtual void fromMap(const QVariantMap &map);

    /**
     * @brief Must be reimplemented, default implementationd does nothing
     */
    virtual QVariantMap toMap() const;

    virtual void secretsFromMap(const QVariantMap &map);

    /**
     * @brief secretsFromStringMap is a convenience function
     * to set the secrets from a map of strings.
     * @param map to extract secrets from
     */
    virtual void secretsFromStringMap(const NMStringMap &map);

    virtual QVariantMap secretsToMap() const;

    /**
     * @brief secretsToStringMap is a convenience function
     * to get the secrets to map of strings.
     * @return string map with current secrets
     */
    virtual NMStringMap secretsToStringMap() const;

    virtual QStringList needSecrets(bool requestNew = false) const;

    /**
     * @brief Must be reimplemented, default implementationd does nothing
     */
    virtual QString name() const;

    void setInitialized(bool initialized);

    bool isNull() const;

    void setType(SettingType type);
    SettingType type() const;

    QDebug foo();

protected:
    SettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(Setting)
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Setting::SecretFlags)

QDebug operator<<(QDebug dbg, const Setting &setting);

}

#endif // NMQT_SETTINGS_SETTING_H
