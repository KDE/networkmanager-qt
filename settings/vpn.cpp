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

#include "vpn.h"
#include "vpn_p.h"

#include <nm-setting-vpn.h>

#include <QtCore/QDebug>

NetworkManager::Settings::VpnSettingPrivate::VpnSettingPrivate():
    name(QString("vpn")),
    data(QStringMap()),
    secrets(QStringMap())
{ }

NetworkManager::Settings::VpnSetting::VpnSetting():
    Setting(Setting::Vpn),
    d_ptr(new VpnSettingPrivate())
{ }

NetworkManager::Settings::VpnSetting::VpnSetting(VpnSetting * setting):
    Setting(setting),
    d_ptr(new VpnSettingPrivate())
{
    setServiceType(setting->serviceType());
    setUsername(setting->username());
    setData(setting->data());
    setSecrets(setting->secrets());
}

NetworkManager::Settings::VpnSetting::~VpnSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::VpnSetting::name() const
{
    Q_D(const VpnSetting);

    return d->name;
}

void NetworkManager::Settings::VpnSetting::setServiceType(const QString& type)
{
    Q_D(VpnSetting);

    d->name = type;
}

QString NetworkManager::Settings::VpnSetting::serviceType() const
{
    Q_D(const VpnSetting);

    return d->serviceType;
}

void NetworkManager::Settings::VpnSetting::setUsername(const QString& username)
{
    Q_D(VpnSetting);

    d->username = username;
}

QString NetworkManager::Settings::VpnSetting::username() const
{
    Q_D(const VpnSetting);

    return d->username;
}

void NetworkManager::Settings::VpnSetting::setData(const QStringMap& data)
{
    Q_D(VpnSetting);

    d->data = data;
}

QStringMap NetworkManager::Settings::VpnSetting::data() const
{
    Q_D(const VpnSetting);

    return d->data;
}

void NetworkManager::Settings::VpnSetting::setSecrets(const QStringMap& secrets)
{
    Q_D(VpnSetting);

    d->secrets = secrets;
}

QStringMap NetworkManager::Settings::VpnSetting::secrets() const
{
    Q_D(const VpnSetting);

    return d->secrets;
}

void NetworkManager::Settings::VpnSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_VPN_SERVICE_TYPE))) {
        setServiceType(setting.value(QLatin1String(NM_SETTING_VPN_SERVICE_TYPE)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_VPN_USER_NAME))) {
        setUsername(setting.value(QLatin1String(NM_SETTING_VPN_USER_NAME)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_VPN_DATA))) {
        setData(setting.value(QLatin1String(NM_SETTING_VPN_DATA)).value<QStringMap>());
    }

    if (setting.contains(QLatin1String(NM_SETTING_VPN_SECRETS))) {
        setSecrets(setting.value(QLatin1String(NM_SETTING_VPN_SECRETS)).value<QStringMap>());
    }
}

QVariantMap NetworkManager::Settings::VpnSetting::toMap() const
{
    QVariantMap setting;

    if (!serviceType().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_VPN_SERVICE_TYPE), serviceType());
    }

    if (!username().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_VPN_USER_NAME), username());
    }

    if (!data().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_VPN_DATA), QVariant::fromValue<QStringMap>(data()));
    }

    if (!secrets().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_VPN_SECRETS), QVariant::fromValue<QStringMap>(secrets()));
    }

    return setting;
}

void NetworkManager::Settings::VpnSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << NM_SETTING_VPN_SERVICE_TYPE << ": " << serviceType();
    qDebug() << NM_SETTING_VPN_USER_NAME << ": " << username();
    qDebug() << NM_SETTING_VPN_DATA << ": " << data();
    qDebug() << NM_SETTING_VPN_SECRETS << ": " << secrets();
}
