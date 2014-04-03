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

#include "gsmsetting.h"
#include "gsmsetting_p.h"

#include <nm-setting-gsm.h>

#include <QtCore/QDebug>

NetworkManager::GsmSettingPrivate::GsmSettingPrivate():
    name(NM_SETTING_GSM_SETTING_NAME),
    passwordFlags(NetworkManager::GsmSetting::None),
    networkType(NetworkManager::GsmSetting::Any),
    pinFlags(NetworkManager::GsmSetting::None),
    allowedBand(1),
    homeOnly(false)
{ }

NetworkManager::GsmSetting::GsmSetting():
    Setting(Setting::Gsm),
    d_ptr(new GsmSettingPrivate())
{ }

NetworkManager::GsmSetting::GsmSetting(const Ptr &other):
    Setting(other),
    d_ptr(new GsmSettingPrivate())
{
    setNumber(other->number());
    setUsername(other->username());
    setPassword(other->password());
    setPasswordFlags(other->passwordFlags());
    setApn(other->apn());
    setNetworkId(other->networkId());
    setNetworkType(other->networkType());
    setPin(other->pin());
    setPinFlags(other->pinFlags());
    setAllowedBand(other->allowedBand());
    setHomeOnly(other->homeOnly());
}

NetworkManager::GsmSetting::~GsmSetting()
{
    delete d_ptr;
}

QString NetworkManager::GsmSetting::name() const
{
    Q_D(const GsmSetting);

    return d->name;
}

void NetworkManager::GsmSetting::setNumber(const QString &number)
{
    Q_D(GsmSetting);

    d->number = number;
}

QString NetworkManager::GsmSetting::number() const
{
    Q_D(const GsmSetting);

    return d->number;
}

void NetworkManager::GsmSetting::setUsername(const QString &username)
{
    Q_D(GsmSetting);

    d->username = username;
}

QString NetworkManager::GsmSetting::username() const
{
    Q_D(const GsmSetting);

    return d->username;
}

void NetworkManager::GsmSetting::setPassword(const QString &password)
{
    Q_D(GsmSetting);

    d->password = password;
}

QString NetworkManager::GsmSetting::password() const
{
    Q_D(const GsmSetting);

    return d->password;
}

void NetworkManager::GsmSetting::setPasswordFlags(GsmSetting::SecretFlags flags)
{
    Q_D(GsmSetting);

    d->passwordFlags = flags;
}

NetworkManager::GsmSetting::SecretFlags NetworkManager::GsmSetting::passwordFlags() const
{
    Q_D(const GsmSetting);

    return d->passwordFlags;
}

void NetworkManager::GsmSetting::setApn(const QString &apn)
{
    Q_D(GsmSetting);

    d->apn = apn;
}

QString NetworkManager::GsmSetting::apn() const
{
    Q_D(const GsmSetting);

    return d->apn;
}

void NetworkManager::GsmSetting::setNetworkId(const QString &id)
{
    Q_D(GsmSetting);

    d->networkId = id;
}

QString NetworkManager::GsmSetting::networkId() const
{
    Q_D(const GsmSetting);

    return d->networkId;
}

void NetworkManager::GsmSetting::setNetworkType(NetworkType type)
{
    Q_D(GsmSetting);

    d->networkType = type;
}

NetworkManager::GsmSetting::NetworkType NetworkManager::GsmSetting::networkType() const
{
    Q_D(const GsmSetting);

    return d->networkType;
}

void NetworkManager::GsmSetting::setPin(const QString &pin)
{
    Q_D(GsmSetting);

    d->pin = pin;
}

QString NetworkManager::GsmSetting::pin() const
{
    Q_D(const GsmSetting);

    return d->pin;
}

void NetworkManager::GsmSetting::setPinFlags(GsmSetting::SecretFlags flags)
{
    Q_D(GsmSetting);

    d->pinFlags = flags;
}

NetworkManager::GsmSetting::SecretFlags NetworkManager::GsmSetting::pinFlags() const
{
    Q_D(const GsmSetting);

    return d->pinFlags;
}

void NetworkManager::GsmSetting::setAllowedBand(quint32 band)
{
    Q_D(GsmSetting);

    d->allowedBand = band;
}

quint32 NetworkManager::GsmSetting::allowedBand() const
{
    Q_D(const GsmSetting);

    return d->allowedBand;
}

void NetworkManager::GsmSetting::setHomeOnly(bool homeOnly)
{
    Q_D(GsmSetting);

    d->homeOnly = homeOnly;
}

bool NetworkManager::GsmSetting::homeOnly() const
{
    Q_D(const GsmSetting);

    return d->homeOnly;
}

void NetworkManager::GsmSetting::secretsFromMap(const QVariantMap &secrets)
{
    if (secrets.contains(QLatin1String(NM_SETTING_GSM_PASSWORD))) {
        setPassword(secrets.value(QLatin1String(NM_SETTING_GSM_PASSWORD)).toString());
    }

    if (secrets.contains(QLatin1String(NM_SETTING_GSM_PIN))) {
        setPin(secrets.value(QLatin1String(NM_SETTING_GSM_PIN)).toString());
    }
}

QVariantMap NetworkManager::GsmSetting::secretsToMap() const
{
    QVariantMap secrets;

    if (!password().isEmpty()) {
        secrets.insert(QLatin1String(NM_SETTING_GSM_PASSWORD), password());
    }

    if (!pin().isEmpty()) {
        secrets.insert(QLatin1String(NM_SETTING_GSM_PIN), pin());
    }

    return secrets;
}

QStringList NetworkManager::GsmSetting::needSecrets(bool requestNew) const
{
    QStringList list;

    if ((password().isEmpty() || requestNew) && !passwordFlags().testFlag(NotRequired)) {
        list << QLatin1String(NM_SETTING_GSM_PASSWORD);
    }

    if ((pin().isEmpty() || requestNew) && !pinFlags().testFlag(NotRequired)) {
        list << QLatin1String(NM_SETTING_GSM_PIN);
    }

    return list;
}

void NetworkManager::GsmSetting::fromMap(const QVariantMap &setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_GSM_NUMBER))) {
        setNumber(setting.value(QLatin1String(NM_SETTING_GSM_NUMBER)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_USERNAME))) {
        setUsername(setting.value(QLatin1String(NM_SETTING_GSM_USERNAME)).toString());
    }

    // Secrets
    if (setting.contains(QLatin1String(NM_SETTING_GSM_PASSWORD))) {
        setPassword(setting.value(QLatin1String(NM_SETTING_GSM_PASSWORD)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_PASSWORD_FLAGS))) {
        setPasswordFlags((SecretFlags)setting.value(QLatin1String(NM_SETTING_GSM_PASSWORD_FLAGS)).toInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_APN))) {
        setApn(setting.value(QLatin1String(NM_SETTING_GSM_APN)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_NETWORK_ID))) {
        setNetworkId(setting.value(QLatin1String(NM_SETTING_GSM_NETWORK_ID)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_NETWORK_TYPE))) {
        setNetworkType((NetworkType)setting.value(QLatin1String(NM_SETTING_GSM_NETWORK_TYPE)).toInt());
    }

    // Secrets
    if (setting.contains(QLatin1String(NM_SETTING_GSM_PIN))) {
        setPin(setting.value(QLatin1String(NM_SETTING_GSM_PIN)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_PIN_FLAGS))) {
        setPinFlags((SecretFlags)setting.value(QLatin1String(NM_SETTING_GSM_PIN_FLAGS)).toInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_ALLOWED_BANDS))) {
        setAllowedBand(setting.value(QLatin1String(NM_SETTING_GSM_ALLOWED_BANDS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_GSM_HOME_ONLY))) {
        setHomeOnly(setting.value(QLatin1String(NM_SETTING_GSM_HOME_ONLY)).toBool());
    }
}

QVariantMap NetworkManager::GsmSetting::toMap() const
{
    QVariantMap setting;

    if (!number().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_GSM_NUMBER), number());
    }

    if (!username().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_GSM_USERNAME), username());
    }

    // Secrets
    if (!password().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_GSM_PASSWORD), password());
    }

    setting.insert(QLatin1String(NM_SETTING_GSM_PASSWORD_FLAGS), (int)passwordFlags());

    if (!apn().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_GSM_APN), apn());
    }

    if (!networkId().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_GSM_NETWORK_ID), networkId());
    }

    if (networkType() != -1) {
        setting.insert(QLatin1String(NM_SETTING_GSM_NETWORK_TYPE), networkType());
    }

    // Secrets
    if (!pin().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_GSM_PIN), pin());
    }

    setting.insert(QLatin1String(NM_SETTING_GSM_PIN_FLAGS), (int)pinFlags());

    if (allowedBand() != 1) {
        setting.insert(QLatin1String(NM_SETTING_GSM_ALLOWED_BANDS), allowedBand());
    }

    if (homeOnly()) {
        setting.insert(QLatin1String(NM_SETTING_GSM_HOME_ONLY), homeOnly());
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::GsmSetting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    dbg.nospace() << NM_SETTING_GSM_NUMBER << ": " << setting.number() << '\n';
    dbg.nospace() << NM_SETTING_GSM_USERNAME << ": " << setting.username() << '\n';
    dbg.nospace() << NM_SETTING_GSM_PASSWORD << ": " << setting.password() << '\n';
    dbg.nospace() << NM_SETTING_GSM_PASSWORD_FLAGS << ": " << setting.passwordFlags() << '\n';
    dbg.nospace() << NM_SETTING_GSM_APN << ": " << setting.apn() << '\n';
    dbg.nospace() << NM_SETTING_GSM_NETWORK_ID << ": " << setting.networkId() << '\n';
    dbg.nospace() << NM_SETTING_GSM_NETWORK_TYPE << ": " << setting.networkType() << '\n';
    dbg.nospace() << NM_SETTING_GSM_ALLOWED_BANDS << ": " << setting.allowedBand() << '\n';
    dbg.nospace() << NM_SETTING_GSM_PIN << ": " << setting.pin() << '\n';
    dbg.nospace() << NM_SETTING_GSM_PIN_FLAGS << ": " << setting.pinFlags() << '\n';
    dbg.nospace() << NM_SETTING_GSM_HOME_ONLY << ": " << setting.homeOnly() << '\n';

    return dbg.maybeSpace();
}
