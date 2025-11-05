/*
    SPDX-FileCopyrightText: 2025 Martin Rodriguez Reboredo <yakoyoku@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_WIFIP2P_SETTING_H
#define NETWORKMANAGERQT_WIFIP2P_SETTING_H

#include "setting.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

#include <QSharedPointer>
#include <QStringList>

namespace NetworkManager
{
class WifiP2PSettingPrivate;

/*!
 * \class NetworkManager::WifiP2PSetting
 * \inheaderfile NetworkManagerQt/WifiP2PSetting
 * \inmodule NetworkManagerQt
 *
 * \brief Represents wifip2p setting.
 * \since 6.20
 */
class NETWORKMANAGERQT_EXPORT WifiP2PSetting : public Setting
{
public:
    Q_PROPERTY(name READ name)

    Q_PROPERTY(peer READ peer WRITE setPeer NOTIFY peerChanged)

    Q_PROPERTY(wfdIEs READ wfdIEs WRITE setWfdIES NOTIFY wfdIEsChanged)

    /*!
     * \typedef NetworkManager::WifiP2PSetting::Ptr
     */
    using Ptr = QSharedPointer<WifiP2PSetting>;
    /*!
     * \typedef NetworkManager::WifiP2PSetting::List
     */
    using List = QList<Ptr>;

    /*!
     */
    WifiP2PSetting();
    /*!
     */
    explicit WifiP2PSetting(const Ptr &setting);
    ~WifiP2PSetting() override;

    QString name() const override;

    QString peer() const;
    void setPeer(const QString &peer);

    QByteArray wfdIEs() const;
    void setWfdIEs(const QByteArray &wfdIEs);

    void fromMap(const QVariantMap &setting) override;

    QVariantMap toMap() const override;

Q_SIGNALS:
    void peerChanged(const QString &peer);
    void wfdIEsChanged(const QByteArray &wfdIEs);

protected:
    std::unique_ptr<WifiP2PSettingPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(WifiP2PSetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const WifiP2PSetting &setting);

}

#endif // NETWORKMANAGERQT_WIFIP2P_SETTING_H
