/*
    SPDX-FileCopyrightText: 2018 Pranav Gade <pranavgade20@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_PROXY_SETTING_H
#define NETWORKMANAGERQT_PROXY_SETTING_H

#include "setting.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

#include <QString>

namespace NetworkManager
{
class ProxySettingPrivate;

/*!
 * \class NetworkManager::ProxySetting
 * \inheaderfile NetworkManagerQt/ProxySetting
 * \inmodule NetworkManagerQt
 *
 * \brief Represents proxy setting.
 */
class NETWORKMANAGERQT_EXPORT ProxySetting : public Setting
{
public:
    /*!
     * \typedef NetworkManager::ProxySetting::Ptr
     */
    typedef QSharedPointer<ProxySetting> Ptr;
    /*!
     * \typedef NetworkManager::ProxySetting::List
     */
    typedef QList<Ptr> List;
    /*!
     *
     * \value None
     * \value Auto
     */
    enum Mode {
        None = 0,
        Auto = 1,
    };

    /*!
     */
    ProxySetting();
    /*!
     */
    explicit ProxySetting(const Ptr &other);
    ~ProxySetting() override;

    QString name() const override;

    /*!
     */
    void setBrowserOnly(bool browserOnly);
    /*!
     */
    bool browserOnly() const;

    /*!
     */
    void setMethod(Mode method);
    /*!
     */
    Mode method() const;

    /*!
     */
    void setPacScript(const QString &script);
    /*!
     */
    QString pacScript() const;

    /*!
     */
    void setPacUrl(const QString &url);
    /*!
     */
    QString pacUrl() const;

    void fromMap(const QVariantMap &setting) override;

    QVariantMap toMap() const override;

protected:
    ProxySettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(ProxySetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const ProxySetting &setting);

}

#endif // NETWORKMANAGERQT_PROXY_SETTING_H
