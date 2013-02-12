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

#include "bridgeport.h"
#include "bridgeport_p.h"

#include <nm-setting-bridge-port.h>

#include <QtCore/QDebug>

NetworkManager::Settings::BridgePortSettingPrivate::BridgePortSettingPrivate():
    name(QString("bridge-port")),
    priority(0),
    pathCost(0),
    hairpinMode(false)
{ }

NetworkManager::Settings::BridgePortSetting::BridgePortSetting():
    Setting(Setting::BridgePort),
    d_ptr(new BridgePortSettingPrivate())
{ }

NetworkManager::Settings::BridgePortSetting::BridgePortSetting(BridgePortSetting * setting):
    Setting(setting),
    d_ptr(new BridgePortSettingPrivate())
{
    setPriority(setting->priority());
    setPathCost(setting->pathCost());
    setHairpinMode(setting->hairpinMode());
}

NetworkManager::Settings::BridgePortSetting::~BridgePortSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::BridgePortSetting::name() const
{
    Q_D(const BridgePortSetting);

    return d->name;
}

void NetworkManager::Settings::BridgePortSetting::setPriority(quint16 priority)
{
    Q_D(BridgePortSetting);

    d->priority = priority;
}

quint16 NetworkManager::Settings::BridgePortSetting::priority() const
{
    Q_D(const BridgePortSetting);

    return d->priority;
}

void NetworkManager::Settings::BridgePortSetting::setPathCost(quint16 cost)
{
    Q_D(BridgePortSetting);

    d->pathCost = cost;
}

quint16 NetworkManager::Settings::BridgePortSetting::pathCost() const
{
    Q_D(const BridgePortSetting);

    return d->pathCost;
}

void NetworkManager::Settings::BridgePortSetting::setHairpinMode(bool enable)
{
    Q_D(BridgePortSetting);

    d->hairpinMode = enable;
}

bool NetworkManager::Settings::BridgePortSetting::hairpinMode() const
{
    Q_D(const BridgePortSetting);

    return d->hairpinMode;
}

bool NetworkManager::Settings::BridgePortSetting::hasSecrets() const
{
    return false;
}

void NetworkManager::Settings::BridgePortSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_BRIDGE_PORT_PRIORITY))) {
        setPriority(setting.value(QLatin1String(NM_SETTING_BRIDGE_PORT_PRIORITY)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_BRIDGE_PORT_PATH_COST))) {
        setPathCost(setting.value(QLatin1String(NM_SETTING_BRIDGE_PORT_PATH_COST)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_BRIDGE_PORT_HAIRPIN_MODE))) {
        setHairpinMode(setting.value(QLatin1String(NM_SETTING_BRIDGE_PORT_HAIRPIN_MODE)).toBool());
    }
}

QVariantMap NetworkManager::Settings::BridgePortSetting::toMap() const
{
    QVariantMap setting;

    if (priority()) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_PORT_PRIORITY), priority());
    }

    if (pathCost()) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_PORT_PATH_COST), pathCost());
    }

    if (hairpinMode()) {
        setting.insert(QLatin1String(NM_SETTING_BRIDGE_PORT_PATH_COST), hairpinMode());
    }

    return setting;
}

void NetworkManager::Settings::BridgePortSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << NM_SETTING_BRIDGE_PORT_PRIORITY << ": " << priority();
    qDebug() << NM_SETTING_BRIDGE_PORT_PATH_COST << ": " << pathCost();
    qDebug() << NM_SETTING_BRIDGE_PORT_PATH_COST << ": " << hairpinMode();
}
