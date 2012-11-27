/*
    Copyright 2012  Jan Grulich <jgrulich@redhat.com>

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

#include "gsmsetting.h"
#include "gsmsetting_p.h"

#include <nm-setting-gsm.h>

#include <QDebug>

NetworkManager::Settings::GsmSettingPrivate::GsmSettingPrivate():
    name(QString("gsm")),
    number(QString()),
    username(QString()),
    password(QString()),
    passwordFlags(NetworkManager::Settings::GsmSetting::None),
    apn(QString()),
    networkId(QString()),
    networkType(-1),
    pin(QString()),
    pinFlags(NetworkManager::Settings::GsmSetting::None),
    allowedBand(1),
    homeOnly(false)
{
}

NetworkManager::Settings::GsmSetting::GsmSetting():
    Setting(Setting::Gsm),
    d_ptr(new GsmSettingPrivate())
{ }

NetworkManager::Settings::GsmSetting::GsmSetting(GsmSetting * setting):
    Setting(setting),
    d_ptr(new GsmSettingPrivate())
{
    setNumber(setting->number());
    setUsername(setting->username());
    setPassword(setting->password());
    setPasswordFlags(setting->passwordFlags());
    setApn(setting->apn());
    setNetworkId(setting->networkId());
    setNetworkType(setting->networkType());
    setPin(setting->pin());
    setPinFlags(setting->pinFlags());
    setAllowedBand(setting->allowedBand());
    setHomeOnly(setting->homeOnly());
}

NetworkManager::Settings::GsmSetting::~GsmSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::GsmSetting::name() const
{
    Q_D(const GsmSetting);

    return d->name;
}

void NetworkManager::Settings::GsmSetting::setNumber(const QString& number)
{
    Q_D(GsmSetting);

    d->number = number;
}

QString NetworkManager::Settings::GsmSetting::number() const
{
    Q_D(const GsmSetting);

    return d->number;
}

void NetworkManager::Settings::GsmSetting::setUsername(const QString& username)
{
    Q_D(GsmSetting);

    d->username = username;
}

QString NetworkManager::Settings::GsmSetting::username() const
{
    Q_D(const GsmSetting);

    return d->username;
}

void NetworkManager::Settings::GsmSetting::setPassword(const QString& password)
{
    Q_D(GsmSetting);

    d->password = password;
}

QString NetworkManager::Settings::GsmSetting::password() const
{
    Q_D(const GsmSetting);

    return d->password;
}

void NetworkManager::Settings::GsmSetting::setPasswordFlags(const GsmSetting::SecretFlagType flags)
{
    Q_D(GsmSetting);

    d->passwordFlags = flags;
}

NetworkManager::Settings::GsmSetting::SecretFlagType NetworkManager::Settings::GsmSetting::passwordFlags() const
{
    Q_D(const GsmSetting);

    return d->passwordFlags;
}

void NetworkManager::Settings::GsmSetting::setApn(const QString& apn)
{
    Q_D(GsmSetting);

    d->apn = apn;
}

QString NetworkManager::Settings::GsmSetting::apn() const
{
    Q_D(const GsmSetting);

    return d->apn;
}

void NetworkManager::Settings::GsmSetting::setNetworkId(const QString& id)
{
    Q_D(GsmSetting);

    d->networkId = id;
}

QString NetworkManager::Settings::GsmSetting::networkId() const
{
    Q_D(const GsmSetting);

    return d->networkId;
}

void NetworkManager::Settings::GsmSetting::setNetworkType(const qint32 type)
{
    Q_D(GsmSetting);

    d->networkType = type;
}

qint32 NetworkManager::Settings::GsmSetting::networkType() const
{
    Q_D(const GsmSetting);

    return d->networkType;
}

void NetworkManager::Settings::GsmSetting::setPin(const QString& pin)
{
    Q_D(GsmSetting);

    d->pin = pin;
}

QString NetworkManager::Settings::GsmSetting::pin() const
{
    Q_D(const GsmSetting);

    return d->pin;
}

void NetworkManager::Settings::GsmSetting::setPinFlags(const GsmSetting::SecretFlagType flags)
{
    Q_D(GsmSetting);

    d->pinFlags = flags;
}

NetworkManager::Settings::GsmSetting::SecretFlagType NetworkManager::Settings::GsmSetting::pinFlags() const
{
    Q_D(const GsmSetting);

    return d->pinFlags;
}

void NetworkManager::Settings::GsmSetting::setAllowedBand(const quint32 band)
{
    Q_D(GsmSetting);

    d->allowedBand = band;
}

quint32 NetworkManager::Settings::GsmSetting::allowedBand() const
{
    Q_D(const GsmSetting);

    return d->allowedBand;
}

void NetworkManager::Settings::GsmSetting::setHomeOnly(const bool homeOnly)
{
    Q_D(GsmSetting);

    d->homeOnly = homeOnly;
}

bool NetworkManager::Settings::GsmSetting::homeOnly() const
{
    Q_D(const GsmSetting);

    return d->homeOnly;
}

void NetworkManager::Settings::GsmSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_GSM_NUMBER))) {
	setNumber(setting.value(QLatin1String(NM_SETTING_GSM_NUMBER)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_USERNAME))) {
	setUsername(setting.value(QLatin1String(NM_SETTING_GSM_USERNAME)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_PASSWORD))) {
	setPassword(setting.value(QLatin1String(NM_SETTING_GSM_PASSWORD)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_PASSWORD_FLAGS))) {
	setPasswordFlags((SecretFlagType)setting.value(QLatin1String(NM_SETTING_GSM_PASSWORD_FLAGS)).toInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_APN))) {
	setApn(setting.value(QLatin1String(NM_SETTING_GSM_APN)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_NETWORK_ID))) {
	setNetworkId(setting.value(QLatin1String(NM_SETTING_GSM_NETWORK_ID)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_NETWORK_TYPE))) {
	setNetworkType(setting.value(QLatin1String(NM_SETTING_GSM_NETWORK_TYPE)).toInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_ALLOWED_BANDS))) {
	setAllowedBand(setting.value(QLatin1String(NM_SETTING_GSM_ALLOWED_BANDS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_PIN))) {
	setPin(setting.value(QLatin1String(NM_SETTING_GSM_PIN)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_PIN_FLAGS))) {
	setPinFlags((SecretFlagType)setting.value(QLatin1String(NM_SETTING_GSM_PIN_FLAGS)).toInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_HOME_ONLY))) {
	setHomeOnly(setting.value(QLatin1String(NM_SETTING_GSM_HOME_ONLY)).toBool());
    }
}

void NetworkManager::Settings::GsmSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << "NUMBER - " << number();
    qDebug() << "USERNAME - " << username();
    qDebug() << "PASSWORD - " << password();
    qDebug() << "PASSWORD FLAGS - " << passwordFlags();
    qDebug() << "APN - " << apn();
    qDebug() << "NETWORK ID - " << networkId();
    qDebug() << "NETWORK TYPE - " << networkType();
    qDebug() << "ALLOWED BAND - " << allowedBand();
    qDebug() << "PIN - " << pin();
    qDebug() << "PIN FLAGS - " << pinFlags();
    qDebug() << "HOME ONLY - " << homeOnly();
}
