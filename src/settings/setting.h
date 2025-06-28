/*
    SPDX-FileCopyrightText: 2012-2019 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_SETTING_H
#define NETWORKMANAGERQT_SETTING_H

#include <networkmanagerqt/networkmanagerqt_export.h>

#include "generictypes.h"

#undef signals
#include <libnm/NetworkManager.h>
#define signals Q_SIGNALS

#include <QSharedPointer>
#include <QStringList>
#include <QVariantMap>

namespace NetworkManager
{
class SettingPrivate;

/*!
 * \class NetworkManager::Setting
 * \inheaderfile NetworkManagerQt/Setting
 * \inmodule NetworkManagerQt
 *
 * \brief Base class for all kinds of setting.
 */
class NETWORKMANAGERQT_EXPORT Setting
{
public:
    /*!
     * \typedef NetworkManager::SerialSetting::Ptr
     */
    typedef QSharedPointer<Setting> Ptr;
    /*!
     * \typedef NetworkManager::SerialSetting::List
     */
    typedef QList<Ptr> List;
    /*!
     *
     * \value Adsl
     * \value Cdma
     * \value Gsm
     * \value Infiniband
     * \value Ipv4
     * \value Ipv6
     * \value Ppp
     * \value Pppoe
     * \value Security8021x
     * \value Serial
     * \value Vpn
     * \value Wired
     * \value Wireless
     * \value WirelessSecurity
     * \value Bluetooth
     * \value OlpcMesh
     * \value Vlan
     * \value Wimax
     * \value Bond
     * \value Bridge
     * \value BridgePort
     * \value Team
     * \value Generic
     * \value Tun
     * \value Vxlan
     * \value IpTunnel
     * \value Proxy
     * \value User
     * \value OvsBridge
     * \value OvsInterface
     * \value OvsPatch
     * \value OvsPort
     * \value Match
     * \value Tc
     * \value TeamPort
     * \value Macsec
     * \value Dcb
     * \value WireGuard
     */
    enum SettingType {
        Adsl,
        Cdma,
        Gsm,
        Infiniband,
        Ipv4,
        Ipv6,
        Ppp,
        Pppoe,
        Security8021x,
        Serial,
        Vpn,
        Wired,
        Wireless,
        WirelessSecurity,
        WifiP2P,
        Bluetooth,
        OlpcMesh,
        Vlan,
        Wimax,
        Bond,
        Bridge,
        BridgePort,
        Team,
        Generic,
        Tun,
        Vxlan,
        IpTunnel,
        Proxy,
        User,
        OvsBridge,
        OvsInterface,
        OvsPatch,
        OvsPort,
        Match,
        Tc,
        TeamPort,
        Macsec,
        Dcb,
        WireGuard,
    };

    /*!
     *
     * \value None
     * \value AgentOwned
     * \value NotSaved
     * \value NotRequired
     */
    enum SecretFlagType {
        None = 0,
        AgentOwned = 0x01,
        NotSaved = 0x02,
        NotRequired = 0x04,
    };
    Q_DECLARE_FLAGS(SecretFlags, SecretFlagType)

    /*!
     *
     * \value MacAddressRandomizationDefault
     * \value MacAddressRandomizationNever
     * \value MacAddressRandomizationAlways
     */
    enum MacAddressRandomization {
        MacAddressRandomizationDefault = 0,
        MacAddressRandomizationNever,
        MacAddressRandomizationAlways,
    };

    /*!
     */
    static QString typeAsString(SettingType type);
    /*!
     */
    static SettingType typeFromString(const QString &type);

    /*!
     */
    explicit Setting(SettingType type);
    /*!
     */
    explicit Setting(const Ptr &setting);
    /*!
     */
    virtual ~Setting();

    /*!
     * \brief Must be reimplemented, default implementation does nothing.
     */
    virtual void fromMap(const QVariantMap &map);

    /*!
     * \brief Must be reimplemented, default implementationd does nothing.
     */
    virtual QVariantMap toMap() const;

    /*!
     */
    virtual void secretsFromMap(const QVariantMap &map);

    /*!
     * \brief secretsFromStringMap is a convenience function
     * to set the secrets from a map of strings.
     * \a map to extract secrets from
     */
    virtual void secretsFromStringMap(const NMStringMap &map);

    /*!
     */
    virtual QVariantMap secretsToMap() const;

    /*!
     * \brief secretsToStringMap is a convenience function
     * to get the secrets to map of strings.
     * Returns string map with current secrets
     */
    virtual NMStringMap secretsToStringMap() const;

    /*!
     */
    virtual QStringList needSecrets(bool requestNew = false) const;

    /*!
     * \brief Must be reimplemented, default implementationd does nothing.
     */
    virtual QString name() const;

    /*!
     */
    void setInitialized(bool initialized);

    /*!
     */
    bool isNull() const;

    /*!
     */
    void setType(SettingType type);
    /*!
     */
    SettingType type() const;

protected:
    SettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(Setting)
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Setting::SecretFlags)

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const Setting &setting);

}

#endif // NETWORKMANAGERQT_SETTING_H
