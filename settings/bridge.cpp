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

#include "bridge.h"
#include "bridge_p.h"

#include <nm-setting-bridge.h>

#include <QtCore/QDebug>

NetworkManager::Settings::BridgeSettingPrivate::BridgeSettingPrivate():
    name(QString("bridge")),
    interfaceName(QString()),
    stp(true),
    priority(0),
    forwardDelay(0),
    helloTime(0),
    maxAge(0),
    ageintTime(0)
{ }

NetworkManager::Settings::BridgeSetting::BridgeSetting():
    Setting(Setting::Bridge),
    d_ptr(new BridgeSettingPrivate())
{ }

NetworkManager::Settings::BridgeSetting::BridgeSetting(BridgeSetting * setting):
    Setting(setting),
    d_ptr(new BridgeSettingPrivate())
{
    setInterfaceName(setting->interfaceName());
    setStp(setting->stp());
    setPriority(setting->priority());
    setForwardDelay(setting->forwardDelay());
    setHelloTime(setting->helloTime());
    setMaxAge(setting->maxAge());
    setAgeintTime(setting->ageintTime());
}

NetworkManager::Settings::BridgeSetting::~BridgeSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::BridgeSetting::name() const
{
    Q_D(const BridgeSetting);

    return d->name;
}

void NetworkManager::Settings::BridgeSetting::setInterfaceName(const QString& name)
{
    Q_D(BridgeSetting);

    d->interfaceName = name;
}

QString NetworkManager::Settings::BridgeSetting::interfaceName() const
{
    Q_D(const BridgeSetting);

    return d->interfaceName;
}

void NetworkManager::Settings::BridgeSetting::setStp(bool enabled)
{
    Q_D(BridgeSetting);

    d->stp = enabled;
}

bool NetworkManager::Settings::BridgeSetting::stp() const
{
    Q_D(const BridgeSetting);

    return d->stp;
}

void NetworkManager::Settings::BridgeSetting::setPriority(quint16 priority)
{
    Q_D(BridgeSetting);

    d->priority = priority;
}

quint16 NetworkManager::Settings::BridgeSetting::priority() const
{
    Q_D(const BridgeSetting);

    return d->priority;
}

void NetworkManager::Settings::BridgeSetting::setForwardDelay(quint16 delay)
{
    Q_D(BridgeSetting);

    d->forwardDelay = delay;
}

quint16 NetworkManager::Settings::BridgeSetting::forwardDelay() const
{
    Q_D(const BridgeSetting);

    return d->forwardDelay;
}

void NetworkManager::Settings::BridgeSetting::setHelloTime(quint16 time)
{
    Q_D(BridgeSetting);

    d->helloTime = time;
}

quint16 NetworkManager::Settings::BridgeSetting::helloTime() const
{
    Q_D(const BridgeSetting);

    return d->helloTime;
}

void NetworkManager::Settings::BridgeSetting::setMaxAge(quint16 age)
{
    Q_D(BridgeSetting);

    d->maxAge = age;
}

quint16 NetworkManager::Settings::BridgeSetting::maxAge() const
{
    Q_D(const BridgeSetting);

    return d->maxAge;
}

void NetworkManager::Settings::BridgeSetting::setAgeintTime(quint32 time)
{
    Q_D(BridgeSetting);

    d->ageintTime = time;
}

quint32 NetworkManager::Settings::BridgeSetting::ageintTime() const
{
    Q_D(const BridgeSetting);

    return d->ageintTime;
}

bool NetworkManager::Settings::BridgeSetting::hasSecrets() const
{
    return false;
}

void NetworkManager::Settings::BridgeSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_BRIDGE_INTERFACE_NAME))) {
        setInterfaceName(setting.value(QLatin1String(NM_SETTING_BRIDGE_INTERFACE_NAME)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_BRIDGE_STP))) {
        setStp(setting.value(QLatin1String(NM_SETTING_BRIDGE_STP)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_BRIDGE_PRIORITY))) {
        setPriority(setting.value(QLatin1String(NM_SETTING_BRIDGE_PRIORITY)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_BRIDGE_FORWARD_DELAY))) {
        setForwardDelay(setting.value(QLatin1String(NM_SETTING_BRIDGE_FORWARD_DELAY)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_BRIDGE_HELLO_TIME))) {
        setHelloTime(setting.value(QLatin1String(NM_SETTING_BRIDGE_HELLO_TIME)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_BRIDGE_MAX_AGE))) {
        setMaxAge(setting.value(QLatin1String(NM_SETTING_BRIDGE_MAX_AGE)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_BRIDGE_AGEING_TIME))) {
        setAgeintTime(setting.value(QLatin1String(NM_SETTING_BRIDGE_AGEING_TIME)).toUInt());
    }
}

QVariantMap NetworkManager::Settings::BridgeSetting::toMap() const
{
    QVariantMap setting;

    if (!interfaceName().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_INTERFACE_NAME), interfaceName());
    }

    if (!stp()) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_STP), stp());
    }

    if (priority()) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_PRIORITY), priority());
    }

    if (forwardDelay()) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_FORWARD_DELAY), forwardDelay());
    }

    if (helloTime()) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_HELLO_TIME), helloTime());
    }

    if (maxAge()) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_MAX_AGE), maxAge());
    }

    if (ageintTime()) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_AGEING_TIME), ageintTime());
    }

    return setting;
}

void NetworkManager::Settings::BridgeSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << "INTERFACE NAME - " << interfaceName();
    qDebug() << "STP - " << stp();
    qDebug() << "PRIORITY - " << priority();
    qDebug() << "FORWARD DELAY - " << forwardDelay();
    qDebug() << "HELLO TIME - " << helloTime();
    qDebug() << "MAX AGE - " << maxAge();
    qDebug() << "AGEINT TIME - " << ageintTime();
}
