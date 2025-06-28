/*
    SPDX-FileCopyrightText: 2025 Martin Rodriguez Reboredo <yakoyoku@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "wifip2psetting.h"

#include "utils.h"

#include <QDebug>

namespace NetworkManager
{
class WifiP2PSettingPrivate
{
public:
    WifiP2PSettingPrivate();

    QString name;
    QString peer;
    QByteArray wfdIEs;
};

}

NetworkManager::WifiP2PSettingPrivate::WifiP2PSettingPrivate()
    : name(NM_SETTING_WIFI_P2P_SETTING_NAME)
{
}

NetworkManager::WifiP2PSetting::WifiP2PSetting()
    : Setting(Setting::WifiP2P)
    , d_ptr(std::make_unique<WifiP2PSettingPrivate>())
{
}

NetworkManager::WifiP2PSetting::WifiP2PSetting(const Ptr &setting)
    : Setting(setting)
    , d_ptr(std::make_unique<WifiP2PSettingPrivate>())
{
    setPeer(setting->peer());
    setWfdIEs(setting->wfdIEs());
}

NetworkManager::WifiP2PSetting::~WifiP2PSetting() = default;

QString NetworkManager::WifiP2PSetting::name() const
{
    Q_D(const WifiP2PSetting);

    return d->name;
}

QString NetworkManager::WifiP2PSetting::peer() const
{
    Q_D(const WifiP2PSetting);

    return d->peer;
}

void NetworkManager::WifiP2PSetting::setPeer(const QString &peer)
{
    Q_D(WifiP2PSetting);

    d->peer = peer;
}

QByteArray NetworkManager::WifiP2PSetting::wfdIEs() const
{
    Q_D(const WifiP2PSetting);

    return d->wfdIEs;
}

void NetworkManager::WifiP2PSetting::setWfdIEs(const QByteArray &wfdIEs)
{
    Q_D(WifiP2PSetting);

    d->wfdIEs = wfdIEs;
}

void NetworkManager::WifiP2PSetting::fromMap(const QVariantMap &setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_WIFI_P2P_PEER))) {
        setPeer(setting.value(QLatin1String(NM_SETTING_WIFI_P2P_PEER)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIFI_P2P_WFD_IES))) {
        setWfdIEs(setting.value(QLatin1String(NM_SETTING_WIFI_P2P_WFD_IES)).toByteArray());
    }
}

QVariantMap NetworkManager::WifiP2PSetting::toMap() const
{
    QVariantMap setting;

    if (!peer().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_WIFI_P2P_PEER), peer());
    }

    if (!wfdIEs().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_WIFI_P2P_WFD_IES), wfdIEs());
    }

    return setting;
}

QDebug NetworkManager::operator<<(QDebug dbg, const NetworkManager::WifiP2PSetting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    dbg.nospace() << NM_SETTING_WIFI_P2P_PEER << ": " << setting.peer() << '\n';

    dbg.nospace() << NM_SETTING_WIFI_P2P_WFD_IES << ": " << setting.wfdIEs() << '\n';

    return dbg.maybeSpace();
}
