/*
    SPDX-FileCopyrightText: 2012-2013 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_WIRELESSSECURITY_SETTING_H
#define NETWORKMANAGERQT_WIRELESSSECURITY_SETTING_H

#include "setting.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

#include <QStringList>

namespace NetworkManager
{
class WirelessSecuritySettingPrivate;

/*!
 * \class NetworkManager::WirelessSecuritySetting
 * \inheaderfile NetworkManagerQt/WirelessSecuritySetting
 * \inmodule NetworkManagerQt
 *
 * \brief Represents wireless security setting.
 */
class NETWORKMANAGERQT_EXPORT WirelessSecuritySetting : public Setting
{
public:
    /*!
     * \typedef NetworkManager::WirelessSecuritySetting::Ptr
     */
    typedef QSharedPointer<WirelessSecuritySetting> Ptr;
    /*!
     * \typedef NetworkManager::WirelessSecuritySetting::List
     */
    typedef QList<Ptr> List;
    /*!
     *
     * \value Unknown
     * \value Wep
     * \value Ieee8021x
     * \value WpaNone
     * \value WpaPsk
     * \value WpaEap
     * \value SAE
     * \value WpaEapSuiteB192
     * \value OWE
     */
    enum KeyMgmt {
        Unknown = -1,
        Wep,
        Ieee8021x,
        WpaNone,
        WpaPsk,
        WpaEap,
        SAE,
        WpaEapSuiteB192,
        OWE,
    };
    /*!
     *
     * \value None
     * \value Open
     * \value Shared
     * \value Leap
     */
    enum AuthAlg {
        None,
        Open,
        Shared,
        Leap,
    };
    /*!
     *
     * \value Wpa
     * \value Rsn
     */
    enum WpaProtocolVersion {
        Wpa,
        Rsn,
    };
    /*!
     *
     * \value Wep40
     * \value Wep104
     * \value Tkip
     * \value Ccmp
     */
    enum WpaEncryptionCapabilities {
        Wep40,
        Wep104,
        Tkip,
        Ccmp,
    };
    /*!
     *
     * \value NotSpecified
     * \value Hex
     * \value Passphrase
     */
    enum WepKeyType {
        NotSpecified,
        Hex,
        Passphrase,
    };
    /*!
     *
     * \value DefaultPmf
     * \value DisablePmf
     * \value OptionalPmf
     * \value RequiredPmf
     */
    enum Pmf {
        DefaultPmf,
        DisablePmf,
        OptionalPmf,
        RequiredPmf,
    };

    /*!
     */
    WirelessSecuritySetting();
    /*!
     */
    explicit WirelessSecuritySetting(const Ptr &other);
    ~WirelessSecuritySetting() override;

    QString name() const override;

    /*!
     */
    void setKeyMgmt(KeyMgmt mgmt);
    /*!
     */
    KeyMgmt keyMgmt() const;

    /*!
     */
    void setWepTxKeyindex(quint32 index);
    /*!
     */
    quint32 wepTxKeyindex() const;

    /*!
     */
    void setAuthAlg(AuthAlg alg);
    /*!
     */
    AuthAlg authAlg() const;

    /*!
     */
    void setProto(const QList<WpaProtocolVersion> &list);
    /*!
     */
    QList<WpaProtocolVersion> proto() const;

    /*!
     */
    void setPairwise(const QList<WpaEncryptionCapabilities> &list);
    /*!
     */
    QList<WpaEncryptionCapabilities> pairwise() const;

    /*!
     */
    void setGroup(const QList<WpaEncryptionCapabilities> &list);
    /*!
     */
    QList<WpaEncryptionCapabilities> group() const;

    /*!
     */
    void setLeapUsername(const QString &username);
    /*!
     */
    QString leapUsername() const;

    /*!
     */
    void setWepKey0(const QString key);
    /*!
     */
    QString wepKey0() const;

    /*!
     */
    void setWepKey1(const QString key);
    /*!
     */
    QString wepKey1() const;

    /*!
     */
    void setWepKey2(const QString key);
    /*!
     */
    QString wepKey2() const;

    /*!
     */
    void setWepKey3(const QString key);
    /*!
     */
    QString wepKey3() const;

    /*!
     */
    void setWepKeyFlags(SecretFlags type);
    /*!
     */
    SecretFlags wepKeyFlags() const;

    /*!
     */
    void setWepKeyType(WepKeyType type);
    /*!
     */
    WepKeyType wepKeyType() const;

    /*!
     */
    void setPsk(const QString &key);
    /*!
     */
    QString psk() const;

    /*!
     */
    void setPskFlags(SecretFlags type);
    /*!
     */
    SecretFlags pskFlags() const;

    /*!
     */
    void setLeapPassword(const QString &password);
    /*!
     */
    QString leapPassword() const;

    /*!
     */
    void setLeapPasswordFlags(SecretFlags type);
    /*!
     */
    SecretFlags leapPasswordFlags() const;

    /*!
     */
    void setPmf(Pmf pmf);
    /*!
     */
    Pmf pmf() const;

    void secretsFromMap(const QVariantMap &secrets) override;

    QVariantMap secretsToMap() const override;

    QStringList needSecrets(bool requestNew = false) const override;

    void fromMap(const QVariantMap &map) override;

    QVariantMap toMap() const override;

protected:
    WirelessSecuritySettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(WirelessSecuritySetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const WirelessSecuritySetting &setting);

}

#endif // NETWORKMANAGERQT_WIRELESSSECURITY_SETTING_H
