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

#include "cdma.h"
#include "cdma_p.h"

#include <nm-setting-cdma.h>

#include <QtCore/QDebug>

NetworkManager::Settings::CdmaSettingPrivate::CdmaSettingPrivate():
    name(QString("cdma")),
    passwordFlags(Setting::None)
{ }

NetworkManager::Settings::CdmaSetting::CdmaSetting():
    Setting(Setting::Cdma),
    d_ptr(new CdmaSettingPrivate())
{ }

NetworkManager::Settings::CdmaSetting::CdmaSetting(CdmaSetting * setting):
    Setting(setting),
    d_ptr(new CdmaSettingPrivate())
{
    setUsername(setting->username());
    setNumber(setting->number());
    setPassword(setting->password());
    setPasswordFlags(setting->passwordFlags());
}

NetworkManager::Settings::CdmaSetting::~CdmaSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::CdmaSetting::name() const
{
    Q_D(const CdmaSetting);

    return d->name;
}

void NetworkManager::Settings::CdmaSetting::setNumber(const QString& number)
{
    Q_D(CdmaSetting);

    d->number = number;
}

QString NetworkManager::Settings::CdmaSetting::number() const
{
    Q_D(const CdmaSetting);

    return d->number;
}

void NetworkManager::Settings::CdmaSetting::setUsername(const QString& username)
{
    Q_D(CdmaSetting);

    d->username = username;
}

QString NetworkManager::Settings::CdmaSetting::username() const
{
    Q_D(const CdmaSetting);

    return d->username;
}

void NetworkManager::Settings::CdmaSetting::setPassword(const QString& password)
{
    Q_D(CdmaSetting);

    d->password = password;
}

QString NetworkManager::Settings::CdmaSetting::password() const
{
    Q_D(const CdmaSetting);

    return d->password;
}

void NetworkManager::Settings::CdmaSetting::setPasswordFlags(NetworkManager::Settings::Setting::SecretFlags flags)
{
    Q_D(CdmaSetting);

    d->passwordFlags = flags;
}

NetworkManager::Settings::Setting::SecretFlags NetworkManager::Settings::CdmaSetting::passwordFlags() const
{
    Q_D(const CdmaSetting);

    return d->passwordFlags;
}

QStringList NetworkManager::Settings::CdmaSetting::needSecrets(bool requestNew) const
{
    QStringList secrets;
    if (!username().isEmpty()) {
        if ((password().isEmpty() || requestNew) && !passwordFlags().testFlag(NotRequired)) {
            secrets << QLatin1String(NM_SETTING_CDMA_PASSWORD);
        }
    }

    return secrets;
}

void NetworkManager::Settings::CdmaSetting::secretsFromMap(const QVariantMap& secrets)
{
    if (secrets.contains(QLatin1String(NM_SETTING_CDMA_PASSWORD))) {
        setPassword(secrets.value(QLatin1String(NM_SETTING_CDMA_PASSWORD)).toString());
    }
}

QVariantMap NetworkManager::Settings::CdmaSetting::secretsToMap() const
{
    QVariantMap secrets;

    if (!password().isEmpty()) {
        secrets.insert(QLatin1String(NM_SETTING_CDMA_PASSWORD), password());
    }

    return secrets;
}

void NetworkManager::Settings::CdmaSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_CDMA_NUMBER))) {
        setNumber(setting.value(QLatin1String(NM_SETTING_CDMA_NUMBER)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_CDMA_USERNAME))) {
        setUsername(setting.value(QLatin1String(NM_SETTING_CDMA_USERNAME)).toString());
    }

    // SECRETS
    if (setting.contains(QLatin1String(NM_SETTING_CDMA_PASSWORD))) {
        setPassword(setting.value(QLatin1String(NM_SETTING_CDMA_PASSWORD)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_CDMA_PASSWORD_FLAGS))) {
        setPasswordFlags((Setting::SecretFlags)setting.value(QLatin1String(NM_SETTING_CDMA_PASSWORD_FLAGS)).toInt());
    }
}

QVariantMap NetworkManager::Settings::CdmaSetting::toMap() const
{
    QVariantMap setting;

    if (!number().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_CDMA_NUMBER), number());
    }

    if (!username().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_CDMA_USERNAME), username());
    }

    // SECRETS
    if (!password().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_CDMA_PASSWORD), password());
    }

    if (passwordFlags() != None) {
        setting.insert(QLatin1String(NM_SETTING_CDMA_PASSWORD_FLAGS), (int)passwordFlags());
    }

    return setting;
}

void NetworkManager::Settings::CdmaSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << NM_SETTING_CDMA_NUMBER << ": " << number();
    qDebug() << NM_SETTING_CDMA_USERNAME << ": " << username();
    qDebug() << NM_SETTING_CDMA_PASSWORD << ": " << password();
    qDebug() << NM_SETTING_CDMA_PASSWORD_FLAGS << ": " << passwordFlags();
}
