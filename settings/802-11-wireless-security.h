/*
    Copyright 2012  Jan Grulich <jgrulich@redhat.com>

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


#ifndef NM_WIRELESSSECURITYSETTING_H
#define NM_WIRELESSSECURITYSETTING_H

#include "QtNetworkManager-export.h"
#include "setting.h"

#include <QStringList>

namespace NetworkManager
{
namespace Settings
{
    class WirelessSecuritySettingPrivate;

    class NMQT_EXPORT WirelessSecuritySetting : public Setting
    {
    Q_DECLARE_PRIVATE(WirelessSecuritySetting)
    public:
        enum KeyMgmt {Wep, Ieee8021x, WpaNone, WpaPsk, WpaEap};
        enum AuthAlg {None, Open, Shared, Leap};
        enum WpaProtocolVersion {Wpa, Rsn};
        enum WpaEncryptionCapabilities {Wep40, Wep104, Tkip, Ccmp};
        enum WepKeyType {NotSpecified, Hex, Passphrase};

        WirelessSecuritySetting();
        WirelessSecuritySetting(WirelessSecuritySetting * setting);
        virtual ~WirelessSecuritySetting();

        QString name() const;

        void setKeyMgmt(KeyMgmt mgmt);
        KeyMgmt keyMgmt() const;

        void setWepTxKeyindex(quint32 index);
        quint32 wepTxKeyindex() const;

        void setAuthAlg(AuthAlg alg);
        AuthAlg authAlg() const;

        void setProto(const QList<WpaProtocolVersion> & list);
        QList<WpaProtocolVersion> proto() const;

        void setPairwise(const QList<WpaEncryptionCapabilities> & list);
        QList<WpaEncryptionCapabilities> pairwise() const;

        void setGroup(const QList<WpaEncryptionCapabilities> & list);
        QList<WpaEncryptionCapabilities> group() const;

        void setLeapUsername(const QString & username);
        QString leapUsername() const;

        void setWepKey0(const QString key);
        QString wepKey0() const;

        void setWepKey1(const QString key);
        QString wepKey1() const;

        void setWepKey2(const QString key);
        QString wepKey2() const;

        void setWepKey3(const QString key);
        QString wepKey3() const;

        void setWepKeyFlags(SecretFlagType type);
        SecretFlagType wepKeyFlags() const;

        void setWepKeyType(WepKeyType type);
        WepKeyType wepKeyType() const;

        void setPsk(const QString & key);
        QString psk() const;

        void setPskFlags(SecretFlagType type);
        SecretFlagType pskFlags() const;

        void setLeapPassword(const QString & password);
        QString leapPassword() const;

        void setLeapPasswordFlags(SecretFlagType type);
        SecretFlagType leapPasswordFlags() const;

        void secretsFromMap(const QVariantMap & secrets);

        bool hasSecrets() const;

        QVariantMap secretsToMap() const;

        QStringList needSecrets() const;

        void fromMap(const QVariantMap & map);

        QVariantMap toMap() const;

        //FOR DEBUG
        void printSetting();

    protected:
        WirelessSecuritySettingPrivate * d_ptr;

    };
}
}

#endif // NM_WIRELESSSECURITYSETTING_H
