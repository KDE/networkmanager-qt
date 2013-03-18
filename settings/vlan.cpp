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

#include "vlan.h"
#include "vlan_p.h"

#include <nm-setting-vlan.h>

#include <QtCore/QDebug>

NetworkManager::Settings::VlanSettingPrivate::VlanSettingPrivate():
    name(QString("vlan")),
    interfaceName(QString()),
    parent(QString()),
    id(0),
    flags(VlanSetting::None),
    ingressPriorityMap(QStringList()),
    egressPriorityMap(QStringList())
{ }

NetworkManager::Settings::VlanSetting::VlanSetting():
    Setting(Setting::Bond),
    d_ptr(new VlanSettingPrivate())
{ }

NetworkManager::Settings::VlanSetting::VlanSetting(VlanSetting * setting):
    Setting(setting),
    d_ptr(new VlanSettingPrivate())
{
    setInterfaceName(setting->interfaceName());
    setParent(setting->parent());
    setId(setting->id());
    setFlags(setting->flags());
    setIngressPriorityMap(setting->ingressPriorityMap());
    setEgressPriorityMap(setting->egressPriorityMap());
}

NetworkManager::Settings::VlanSetting::~VlanSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::VlanSetting::name() const
{
    Q_D(const VlanSetting);

    return d->name;
}

void NetworkManager::Settings::VlanSetting::setInterfaceName(const QString& name)
{
    Q_D(VlanSetting);

    d->interfaceName = name;
}

QString NetworkManager::Settings::VlanSetting::interfaceName() const
{
    Q_D(const VlanSetting);

    return d->interfaceName;
}

void NetworkManager::Settings::VlanSetting::setParent(const QString& parent)
{
    Q_D(VlanSetting);

    d->parent = parent;
}

QString NetworkManager::Settings::VlanSetting::parent() const
{
    Q_D(const VlanSetting);

    return d->parent;
}

void NetworkManager::Settings::VlanSetting::setId(quint32 id)
{
    Q_D(VlanSetting);

    d->id = id;
}

quint32 NetworkManager::Settings::VlanSetting::id() const
{
    Q_D(const VlanSetting);

    return d->id;
}

void NetworkManager::Settings::VlanSetting::setFlags(NetworkManager::Settings::VlanSetting::Flag flags)
{
    Q_D(VlanSetting);

    d->flags = flags;
}

NetworkManager::Settings::VlanSetting::Flag NetworkManager::Settings::VlanSetting::flags() const
{
    Q_D(const VlanSetting);

    return d->flags;
}

void NetworkManager::Settings::VlanSetting::setIngressPriorityMap(const QStringList& map)
{
    Q_D(VlanSetting);

    d->ingressPriorityMap = map;
}

QStringList NetworkManager::Settings::VlanSetting::ingressPriorityMap() const
{
    Q_D(const VlanSetting);

    return d->ingressPriorityMap;
}

void NetworkManager::Settings::VlanSetting::setEgressPriorityMap(const QStringList& map)
{
    Q_D(VlanSetting);

    d->egressPriorityMap = map;
}

QStringList NetworkManager::Settings::VlanSetting::egressPriorityMap() const
{
    Q_D(const VlanSetting);

    return d->egressPriorityMap;
}

void NetworkManager::Settings::VlanSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_VLAN_INTERFACE_NAME))) {
        setInterfaceName(setting.value(QLatin1String(NM_SETTING_VLAN_INTERFACE_NAME)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_VLAN_PARENT))) {
        setParent(setting.value(QLatin1String(NM_SETTING_VLAN_PARENT)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_VLAN_ID))) {
        setId(setting.value(QLatin1String(NM_SETTING_VLAN_ID)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_VLAN_FLAGS))) {
        setFlags((Flag)setting.value(QLatin1String(NM_SETTING_VLAN_FLAGS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_VLAN_INGRESS_PRIORITY_MAP))) {
        setIngressPriorityMap(setting.value(QLatin1String(NM_SETTING_VLAN_INGRESS_PRIORITY_MAP)).toStringList());
    }

    if (setting.contains(QLatin1String(NM_SETTING_VLAN_EGRESS_PRIORITY_MAP))) {
        setEgressPriorityMap(setting.value(QLatin1String(NM_SETTING_VLAN_EGRESS_PRIORITY_MAP)).toStringList());
    }
}

QVariantMap NetworkManager::Settings::VlanSetting::toMap() const
{
    QVariantMap setting;

    if (!interfaceName().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_VLAN_INTERFACE_NAME), interfaceName());
    }

    if (!parent().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_VLAN_PARENT), parent());
    }

    if (id()) {
        setting.insert(QLatin1String(NM_SETTING_VLAN_ID), id());
    }

    if (flags() != None) {
        setting.insert(QLatin1String(NM_SETTING_VLAN_FLAGS), flags());
    }

    if (!ingressPriorityMap().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_VLAN_INGRESS_PRIORITY_MAP), ingressPriorityMap());
    }

    if (!egressPriorityMap().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_VLAN_EGRESS_PRIORITY_MAP), egressPriorityMap());
    }

    return setting;
}

void NetworkManager::Settings::VlanSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << NM_SETTING_VLAN_INTERFACE_NAME << ": " << interfaceName();
    qDebug() << NM_SETTING_VLAN_PARENT << ": " << parent();
    qDebug() << NM_SETTING_VLAN_ID << ": " << id();
    qDebug() << NM_SETTING_VLAN_FLAGS << ": " << flags();
    qDebug() << NM_SETTING_VLAN_INGRESS_PRIORITY_MAP << ": " << ingressPriorityMap();
    qDebug() << NM_SETTING_VLAN_EGRESS_PRIORITY_MAP << ": " << egressPriorityMap();
}
