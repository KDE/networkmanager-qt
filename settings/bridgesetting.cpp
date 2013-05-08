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

#include "bridgesetting.h"
#include "bridgesetting_p.h"

#include <nm-setting-bridge.h>

#include <QtCore/QDebug>

NetworkManager::Settings::BridgeSettingPrivate::BridgeSettingPrivate():
    name(NM_SETTING_BRIDGE_SETTING_NAME),
    stp(true),
    priority(128),
    forwardDelay(15),
    helloTime(2),
    maxAge(20),
    agingTime(300)
{ }

NetworkManager::Settings::BridgeSetting::BridgeSetting():
    Setting(Setting::Bridge),
    d_ptr(new BridgeSettingPrivate())
{ }

NetworkManager::Settings::BridgeSetting::BridgeSetting(const Ptr &other):
    Setting(other),
    d_ptr(new BridgeSettingPrivate())
{
    setInterfaceName(other->interfaceName());
    setStp(other->stp());
    setPriority(other->priority());
    setForwardDelay(other->forwardDelay());
    setHelloTime(other->helloTime());
    setMaxAge(other->maxAge());
    setAgingTime(other->agingTime());
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

void NetworkManager::Settings::BridgeSetting::setPriority(quint32 priority)
{
    Q_D(BridgeSetting);

    d->priority = priority;
}

quint32 NetworkManager::Settings::BridgeSetting::priority() const
{
    Q_D(const BridgeSetting);

    return d->priority;
}

void NetworkManager::Settings::BridgeSetting::setForwardDelay(quint32 delay)
{
    Q_D(BridgeSetting);

    d->forwardDelay = delay;
}

quint32 NetworkManager::Settings::BridgeSetting::forwardDelay() const
{
    Q_D(const BridgeSetting);

    return d->forwardDelay;
}

void NetworkManager::Settings::BridgeSetting::setHelloTime(quint32 time)
{
    Q_D(BridgeSetting);

    d->helloTime = time;
}

quint32 NetworkManager::Settings::BridgeSetting::helloTime() const
{
    Q_D(const BridgeSetting);

    return d->helloTime;
}

void NetworkManager::Settings::BridgeSetting::setMaxAge(quint32 age)
{
    Q_D(BridgeSetting);

    d->maxAge = age;
}

quint32 NetworkManager::Settings::BridgeSetting::maxAge() const
{
    Q_D(const BridgeSetting);

    return d->maxAge;
}

void NetworkManager::Settings::BridgeSetting::setAgingTime(quint32 time)
{
    Q_D(BridgeSetting);

    d->agingTime = time;
}

quint32 NetworkManager::Settings::BridgeSetting::agingTime() const
{
    Q_D(const BridgeSetting);

    return d->agingTime;
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
        setAgingTime(setting.value(QLatin1String(NM_SETTING_BRIDGE_AGEING_TIME)).toUInt());
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

    if (priority() != 128) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_PRIORITY), priority());
    }

    if (forwardDelay() != 15) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_FORWARD_DELAY), forwardDelay());
    }

    if (helloTime() != 2) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_HELLO_TIME), helloTime());
    }

    if (maxAge() != 20) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_MAX_AGE), maxAge());
    }

    if (agingTime() != 300) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_AGEING_TIME), agingTime());
    }

    return setting;
}

QDebug NetworkManager::Settings::operator <<(QDebug dbg, const NetworkManager::Settings::BridgeSetting &setting)
{
    dbg.nospace() << static_cast<NetworkManager::Settings::Setting>(setting);

    dbg.nospace() << NM_SETTING_BRIDGE_INTERFACE_NAME << ": " << setting.interfaceName() << '\n';
    dbg.nospace() << NM_SETTING_BRIDGE_STP << ": " << setting.stp() << '\n';
    dbg.nospace() << NM_SETTING_BRIDGE_PRIORITY << ": " << setting.priority() << '\n';
    dbg.nospace() << NM_SETTING_BRIDGE_FORWARD_DELAY << ": " << setting.forwardDelay() << '\n';
    dbg.nospace() << NM_SETTING_BRIDGE_HELLO_TIME << ": " << setting.helloTime() << '\n';
    dbg.nospace() << NM_SETTING_BRIDGE_MAX_AGE << ": " << setting.maxAge() << '\n';
    dbg.nospace() << NM_SETTING_BRIDGE_AGEING_TIME << ": " << setting.agingTime() << '\n';

    return dbg.maybeSpace();
}
