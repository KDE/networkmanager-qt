/*
    SPDX-FileCopyrightText: 2012-2013 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_ADSL_SETTING_H
#define NETWORKMANAGERQT_ADSL_SETTING_H

#include "setting.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

#include <QString>

namespace NetworkManager
{
class AdslSettingPrivate;

/*!
 * \class NetworkManager::AdslSetting
 * \inheaderfile NetworkManagerQt/AdslSetting
 * \inmodule NetworkManagerQt
 *
 * \brief Represents adsl setting.
 */
class NETWORKMANAGERQT_EXPORT AdslSetting : public Setting
{
public:
    /*!
     * \typedef NetworkManager::AdslSetting::Ptr
     */
    typedef QSharedPointer<AdslSetting> Ptr;
    /*!
     * \typedef NetworkManager::AdslSetting::List
     */
    typedef QList<Ptr> List;
    /*!
     *
     * \value UnknownProtocol
     * \value Pppoa
     * \value Pppoe
     * \value Ipoatm
     */
    enum Protocol {
        UnknownProtocol = 0,
        Pppoa,
        Pppoe,
        Ipoatm,
    };
    /*!
     *
     * \value UnknownEncapsulation
     * \value Vcmux
     * \value Llc
     */
    enum Encapsulation {
        UnknownEncapsulation = 0,
        Vcmux,
        Llc,
    };

    /*!
     */
    AdslSetting();
    /*!
     */
    explicit AdslSetting(const Ptr &other);
    ~AdslSetting() override;

    QString name() const override;

    /*!
     */
    void setUsername(const QString &username);
    /*!
     */
    QString username() const;

    /*!
     */
    void setPassword(const QString &password);
    /*!
     */
    QString password() const;

    /*!
     */
    void setPasswordFlags(SecretFlags flags);
    /*!
     */
    SecretFlags passwordFlags() const;

    /*!
     */
    void setProtocol(Protocol protocol);
    /*!
     */
    Protocol protocol() const;

    /*!
     */
    void setEncapsulation(Encapsulation encapsulation);
    /*!
     */
    Encapsulation encapsulation() const;

    /*!
     */
    void setVpi(quint32 vpi);
    /*!
     */
    quint32 vpi() const;

    /*!
     */
    void setVci(quint32 vci);
    /*!
     */
    quint32 vci() const;

    QStringList needSecrets(bool requestNew = false) const override;

    void fromMap(const QVariantMap &setting) override;

    QVariantMap toMap() const override;

protected:
    /*!
     */
    AdslSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(AdslSetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const AdslSetting &setting);

}

#endif // NETWORKMANAGERQT_ADSL_SETTING_H
