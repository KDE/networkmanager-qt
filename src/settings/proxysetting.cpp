/*
    Copyright Pranav Gade <pranavgade20@gmail.com>

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

#include "proxysetting.h"
#include "proxysetting_p.h"

#include <QDebug>

#if !NM_CHECK_VERSION(1, 6, 0)
#define NM_SETTING_PROXY_SETTING_NAME    "proxy"
#define NM_SETTING_PROXY_BROWSER_ONLY    "browser-only"
#define NM_SETTING_PROXY_METHOD          "method"
#define NM_SETTING_PROXY_PAC_SCRIPT      "pac-script"
#define NM_SETTING_PROXY_PAC_URL         "pac-url"
#endif

NetworkManager::ProxySettingPrivate::ProxySettingPrivate()
    : name(NM_SETTING_PROXY_SETTING_NAME)
    , browserOnly(false)
    , method(ProxySetting::None)
{ }

NetworkManager::ProxySetting::ProxySetting()
    : Setting(Setting::Proxy)
    , d_ptr(new ProxySettingPrivate())
{ }

NetworkManager::ProxySetting::ProxySetting(const Ptr &other)
    : Setting(other)
    , d_ptr(new ProxySettingPrivate())
{
    setBrowserOnly(other->browserOnly());
    setMethod(other->method());
    setPacScript(other->pacScript());
    setPacUrl(other->pacUrl());
}

NetworkManager::ProxySetting::~ProxySetting()
{
    delete d_ptr;
}

QString NetworkManager::ProxySetting::name() const
{
    Q_D(const ProxySetting);

    return d->name;
}

void NetworkManager::ProxySetting::setBrowserOnly(bool browserOnly)
{
    Q_D(ProxySetting);

    d->browserOnly = browserOnly;
}

bool NetworkManager::ProxySetting::browserOnly() const
{
    Q_D(const ProxySetting);

    return d->browserOnly;
}

void NetworkManager::ProxySetting::setMethod(NetworkManager::ProxySetting::Mode method)
{
    Q_D(ProxySetting);

    d->method = method;
}

NetworkManager::ProxySetting::Mode NetworkManager::ProxySetting::method() const
{
    Q_D(const ProxySetting);

    return d->method;
}

void NetworkManager::ProxySetting::setPacScript(const QString &script)
{
    Q_D(ProxySetting);

    d->pacScript = script;
}

QString NetworkManager::ProxySetting::pacScript() const
{
    Q_D(const ProxySetting);

    return d->pacScript;
}

void NetworkManager::ProxySetting::setPacUrl(const QString &url)
{
    Q_D(ProxySetting);

    d->pacUrl = url;
}

QString NetworkManager::ProxySetting::pacUrl() const
{
    Q_D(const ProxySetting);

    return d->pacUrl;
}

void NetworkManager::ProxySetting::fromMap(const QVariantMap &setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_PROXY_BROWSER_ONLY))) {
        setBrowserOnly(setting.value(QLatin1String(NM_SETTING_PROXY_BROWSER_ONLY)).toBool());
    }

   if (setting.contains(QLatin1String(NM_SETTING_PROXY_METHOD))) {
        setMethod((Mode)setting.value(QLatin1String(NM_SETTING_PROXY_METHOD)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PROXY_PAC_SCRIPT))) {
        setPacScript(setting.value(QLatin1String(NM_SETTING_PROXY_PAC_SCRIPT)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PROXY_PAC_URL))) {
        setPacUrl(setting.value(QLatin1String(NM_SETTING_PROXY_PAC_URL)).toString());
    }
}

QVariantMap NetworkManager::ProxySetting::toMap() const
{
    QVariantMap setting;

    setting.insert(QLatin1String(NM_SETTING_PROXY_BROWSER_ONLY), browserOnly());

    if (method() > 0) {
        setting.insert(QLatin1String(NM_SETTING_PROXY_METHOD), (int)method());
    }

    if (!pacScript().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_PROXY_PAC_SCRIPT), pacScript());
    }

    if (!pacUrl().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_PROXY_PAC_URL), pacUrl());
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::ProxySetting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    dbg.nospace() << NM_SETTING_PROXY_BROWSER_ONLY << ": " << setting.browserOnly() << '\n';
    dbg.nospace() << NM_SETTING_PROXY_METHOD << ": " << setting.method() << '\n';
    dbg.nospace() << NM_SETTING_PROXY_PAC_SCRIPT << ": " << setting.pacScript() << '\n';
    dbg.nospace() << NM_SETTING_PROXY_PAC_URL << ": " << setting.pacUrl() << '\n';

    return dbg.maybeSpace();
}
