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

#include "adsl.h"
#include "adsl_p.h"

#include <nm-setting-adsl.h>

#include <QtCore/QDebug>

NetworkManager::Settings::AdslSettingPrivate::AdslSettingPrivate():
    name(QString("adsl")),
    username(QString()),
    password(QString()),
    passwordFlags(Setting::None),
    protocol(AdslSetting::UnknownProtocol),
    encapsulation(AdslSetting::UnknownEncapsulation),
    vpi(0),
    vci(0)
{ }

NetworkManager::Settings::AdslSetting::AdslSetting():
    Setting(Setting::Adsl),
    d_ptr(new AdslSettingPrivate())
{ }

NetworkManager::Settings::AdslSetting::AdslSetting(AdslSetting * setting):
    Setting(setting),
    d_ptr(new AdslSettingPrivate())
{
    setUsername(setting->username());
    setPassword(setting->password());
    setProtocol(setting->protocol());
    setPasswordFlags(setting->passwordFlags());
    setProtocol(setting->protocol());
    setEncapsulation(setting->encapsulation());
    setVpi(setting->vpi());
    setVci(setting->vci());
}

NetworkManager::Settings::AdslSetting::~AdslSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::AdslSetting::name() const
{
    Q_D(const AdslSetting);

    return d->name;
}

void NetworkManager::Settings::AdslSetting::setUsername(const QString& username)
{
    Q_D(AdslSetting);

    d->username = username;
}

QString NetworkManager::Settings::AdslSetting::username() const
{
    Q_D(const AdslSetting);

    return d->username;
}

void NetworkManager::Settings::AdslSetting::setPassword(const QString& password)
{
    Q_D(AdslSetting);

    d->password = password;
}

QString NetworkManager::Settings::AdslSetting::password() const
{
    Q_D(const AdslSetting);

    return d->password;
}

void NetworkManager::Settings::AdslSetting::setPasswordFlags(NetworkManager::Settings::Setting::SecretFlagType flags)
{
    Q_D(AdslSetting);

    d->passwordFlags = flags;
}

NetworkManager::Settings::Setting::SecretFlagType NetworkManager::Settings::AdslSetting::passwordFlags() const
{
    Q_D(const AdslSetting);

    return d->passwordFlags;
}

void NetworkManager::Settings::AdslSetting::setProtocol(NetworkManager::Settings::AdslSetting::Protocol protocol)
{
    Q_D(AdslSetting);

    d->protocol = protocol;
}

NetworkManager::Settings::AdslSetting::Protocol NetworkManager::Settings::AdslSetting::protocol() const
{
    Q_D(const AdslSetting);

    return d->protocol;
}

void NetworkManager::Settings::AdslSetting::setEncapsulation(NetworkManager::Settings::AdslSetting::Encapsulation encapsulation)
{
    Q_D(AdslSetting);

    d->encapsulation = encapsulation;
}

NetworkManager::Settings::AdslSetting::Encapsulation NetworkManager::Settings::AdslSetting::encapsulation() const
{
    Q_D(const AdslSetting);

    return d->encapsulation;
}

void NetworkManager::Settings::AdslSetting::setVpi(quint32 vpi)
{
    Q_D(AdslSetting);

    d->vpi = vpi;
}

quint32 NetworkManager::Settings::AdslSetting::vpi() const
{
    Q_D(const AdslSetting);

    return d->vpi;
}

void NetworkManager::Settings::AdslSetting::setVci(quint32 vci)
{
    Q_D(AdslSetting);

    d->vci = vci;
}

quint32 NetworkManager::Settings::AdslSetting::vci() const
{
    Q_D(const AdslSetting);

    return d->vci;
}

QStringList NetworkManager::Settings::AdslSetting::needSecrets() const
{
    QStringList secrets;

    if (password().isEmpty() && passwordFlags() != NotRequired) {
        secrets << QLatin1String(NM_SETTING_ADSL_PASSWORD);
    }

    return secrets;
}

void NetworkManager::Settings::AdslSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_ADSL_USERNAME))) {
        setUsername(setting.value(QLatin1String(NM_SETTING_ADSL_USERNAME)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_ADSL_PASSWORD))) {
        setPassword(setting.value(QLatin1String(NM_SETTING_ADSL_PASSWORD)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_ADSL_PASSWORD_FLAGS))) {
        setPasswordFlags((Setting::SecretFlagType)setting.value(QLatin1String(NM_SETTING_ADSL_PASSWORD_FLAGS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_ADSL_PROTOCOL))) {
        QString protocol = setting.value(QLatin1String(NM_SETTING_ADSL_PROTOCOL)).toString();

        if (protocol == QLatin1String(NM_SETTING_ADSL_PROTOCOL_PPPOA)) {
            setProtocol(Pppoa);
        } else if (protocol == QLatin1String(NM_SETTING_ADSL_PROTOCOL_PPPOE)) {
            setProtocol(Pppoe);
        } else if (protocol == QLatin1String(NM_SETTING_ADSL_PROTOCOL_IPOATM)) {
            setProtocol(Ipoatm);
        }
    }

    if (setting.contains(QLatin1String(NM_SETTING_ADSL_ENCAPSULATION))) {
        QString encapsulation = setting.value(QLatin1String(NM_SETTING_ADSL_ENCAPSULATION)).toString();

        if (encapsulation == QLatin1String(NM_SETTING_ADSL_ENCAPSULATION_VCMUX)) {
            setEncapsulation(Vcmux);
        } else if (encapsulation == QLatin1String(NM_SETTING_ADSL_ENCAPSULATION_LLC)) {
            setEncapsulation(Llc);
        }
    }

    if (setting.contains(QLatin1String(NM_SETTING_ADSL_VPI))) {
        setVpi(setting.value(QLatin1String(NM_SETTING_ADSL_VPI)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_ADSL_VCI))) {
        setVci(setting.value(QLatin1String(NM_SETTING_ADSL_VCI)).toUInt());
    }
}

QVariantMap NetworkManager::Settings::AdslSetting::toMap() const
{
    QVariantMap setting;

    if (!username().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_ADSL_USERNAME), username());
    }

    if (!password().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_ADSL_PASSWORD), password());
    }

    if (passwordFlags() != None) {
        setting.insert(QLatin1String(NM_SETTING_ADSL_PASSWORD_FLAGS), passwordFlags());
    }

    if (protocol() != UnknownProtocol) {
        switch (protocol()) {
            case Pppoa:
                setting.insert(QLatin1String(NM_SETTING_ADSL_PROTOCOL), QLatin1String(NM_SETTING_ADSL_PROTOCOL_PPPOA));
                break;
            case Pppoe:
                setting.insert(QLatin1String(NM_SETTING_ADSL_PROTOCOL), QLatin1String(NM_SETTING_ADSL_PROTOCOL_PPPOE));
                break;
            case Ipoatm:
                setting.insert(QLatin1String(NM_SETTING_ADSL_PROTOCOL), QLatin1String(NM_SETTING_ADSL_PROTOCOL_IPOATM));
                break;
        }
    }

    if (encapsulation() != UnknownEncapsulation) {
        switch (encapsulation()) {
            case Vcmux:
                setting.insert(QLatin1String(NM_SETTING_ADSL_ENCAPSULATION), QLatin1String(NM_SETTING_ADSL_ENCAPSULATION_VCMUX));
                break;
            case Llc:
                setting.insert(QLatin1String(NM_SETTING_ADSL_ENCAPSULATION), QLatin1String(NM_SETTING_ADSL_ENCAPSULATION_LLC));
                break;
        }
    }

    if (vpi()) {
        setting.insert(QLatin1String(NM_SETTING_ADSL_VPI), vpi());
    }

    if (vci()) {
        setting.insert(QLatin1String(NM_SETTING_ADSL_VCI), vci());
    }

    return setting;
}

void NetworkManager::Settings::AdslSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << NM_SETTING_ADSL_USERNAME << ": " << username();
    qDebug() << NM_SETTING_ADSL_PASSWORD << ": " << password();
    qDebug() << NM_SETTING_ADSL_PASSWORD_FLAGS << ": " << passwordFlags();
    qDebug() << NM_SETTING_ADSL_PROTOCOL << ": " << protocol();
    qDebug() << NM_SETTING_ADSL_ENCAPSULATION << ": " << encapsulation();
    qDebug() << NM_SETTING_ADSL_VPI << ": " << vpi();
    qDebug() << NM_SETTING_ADSL_VCI << ": " << vci();
}
