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


#ifndef NMQT_SETTINGS_802_11_WIRELESS_SECURITY_H
#define NMQT_SETTINGS_802_11_WIRELESS_SECURITY_H

#include "../NetworkManagerQt-export.h"
#include "setting.h"

#include <QtCore/QStringList>

namespace NetworkManager
{

class WirelessSecuritySettingPrivate;
class NETWORKMANAGERQT_EXPORT WirelessSecuritySetting : public Setting
{
public:
    typedef QSharedPointer<WirelessSecuritySetting> Ptr;
    typedef QList<Ptr> List;
    enum KeyMgmt {Unknown = -1, Wep, Ieee8021x, WpaNone, WpaPsk, WpaEap};
    enum AuthAlg {None, Open, Shared, Leap};
    enum WpaProtocolVersion {Wpa, Rsn};
    enum WpaEncryptionCapabilities {Wep40, Wep104, Tkip, Ccmp};
    enum WepKeyType {NotSpecified, Hex, Passphrase};

    WirelessSecuritySetting();
    explicit WirelessSecuritySetting(const Ptr &other);
    virtual ~WirelessSecuritySetting();

    QString name() const;

    void setKeyMgmt(KeyMgmt mgmt);
    KeyMgmt keyMgmt() const;

    void setWepTxKeyindex(quint32 index);
    quint32 wepTxKeyindex() const;

    void setAuthAlg(AuthAlg alg);
    AuthAlg authAlg() const;

    void setProto(const QList<WpaProtocolVersion> &list);
    QList<WpaProtocolVersion> proto() const;

    void setPairwise(const QList<WpaEncryptionCapabilities> &list);
    QList<WpaEncryptionCapabilities> pairwise() const;

    void setGroup(const QList<WpaEncryptionCapabilities> &list);
    QList<WpaEncryptionCapabilities> group() const;

    void setLeapUsername(const QString &username);
    QString leapUsername() const;

    void setWepKey0(const QString key);
    QString wepKey0() const;

    void setWepKey1(const QString key);
    QString wepKey1() const;

    void setWepKey2(const QString key);
    QString wepKey2() const;

    void setWepKey3(const QString key);
    QString wepKey3() const;

    void setWepKeyFlags(SecretFlags type);
    SecretFlags wepKeyFlags() const;

    void setWepKeyType(WepKeyType type);
    WepKeyType wepKeyType() const;

    void setPsk(const QString &key);
    QString psk() const;

    void setPskFlags(SecretFlags type);
    SecretFlags pskFlags() const;

    void setLeapPassword(const QString &password);
    QString leapPassword() const;

    void setLeapPasswordFlags(SecretFlags type);
    SecretFlags leapPasswordFlags() const;

    void secretsFromMap(const QVariantMap &secrets);

    QVariantMap secretsToMap() const;

    QStringList needSecrets(bool requestNew = false) const;

    void fromMap(const QVariantMap &map);

    QVariantMap toMap() const;

protected:
    WirelessSecuritySettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(WirelessSecuritySetting)
};

QDebug operator<<(QDebug dbg, const WirelessSecuritySetting &setting);

}

#endif // NMQT_SETTINGS_802_11_WIRELESS_SECURITY_H
