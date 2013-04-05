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

#include "802-11-olpc-mesh.h"
#include "802-11-olpc-mesh_p.h"

#include <nm-setting-olpc-mesh.h>

#include <QtCore/QDebug>

NetworkManager::Settings::OlpcMeshSettingPrivate::OlpcMeshSettingPrivate():
    name(QString("802-11-olpc-mesh")),
    channel(0)
{ }

NetworkManager::Settings::OlpcMeshSetting::OlpcMeshSetting():
    Setting(Setting::Bond),
    d_ptr(new OlpcMeshSettingPrivate())
{ }

NetworkManager::Settings::OlpcMeshSetting::OlpcMeshSetting(const Ptr &other):
    Setting(other),
    d_ptr(new OlpcMeshSettingPrivate())
{
    setSsid(other->ssid());
    setChannel(other->channel());
    setDhcpAnycastAddress(other->dhcpAnycastAddress());
}

NetworkManager::Settings::OlpcMeshSetting::~OlpcMeshSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::OlpcMeshSetting::name() const
{
    Q_D(const OlpcMeshSetting);

    return d->name;
}

void NetworkManager::Settings::OlpcMeshSetting::setSsid(const QByteArray& ssid)
{
    Q_D(OlpcMeshSetting);

    d->ssid = ssid;
}

QByteArray NetworkManager::Settings::OlpcMeshSetting::ssid() const
{
    Q_D(const OlpcMeshSetting);

    return d->ssid;
}

void NetworkManager::Settings::OlpcMeshSetting::setChannel(quint32 channel)
{
    Q_D(OlpcMeshSetting);

    d->channel = channel;
}

quint32 NetworkManager::Settings::OlpcMeshSetting::channel() const
{
    Q_D(const OlpcMeshSetting);

    return d->channel;
}

void NetworkManager::Settings::OlpcMeshSetting::setDhcpAnycastAddress(const QByteArray& address)
{
    Q_D(OlpcMeshSetting);

    d->dhcpAnycastAddress = address;
}

QByteArray NetworkManager::Settings::OlpcMeshSetting::dhcpAnycastAddress() const
{
    Q_D(const OlpcMeshSetting);

    return d->dhcpAnycastAddress;
}

void NetworkManager::Settings::OlpcMeshSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_OLPC_MESH_SSID))) {
        setSsid(setting.value(QLatin1String(NM_SETTING_OLPC_MESH_SSID)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_OLPC_MESH_CHANNEL))) {
        setChannel(setting.value(QLatin1String(NM_SETTING_OLPC_MESH_CHANNEL)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_OLPC_MESH_DHCP_ANYCAST_ADDRESS))) {
        setDhcpAnycastAddress(setting.value(QLatin1String(NM_SETTING_OLPC_MESH_DHCP_ANYCAST_ADDRESS)).toByteArray());
    }
}

QVariantMap NetworkManager::Settings::OlpcMeshSetting::toMap() const
{
    QVariantMap setting;

    if (!ssid().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_OLPC_MESH_SSID), ssid());
    }

    if (channel()) {
        setting.insert(QLatin1String(NM_SETTING_OLPC_MESH_CHANNEL), channel());
    }

    if (!dhcpAnycastAddress().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_OLPC_MESH_DHCP_ANYCAST_ADDRESS), dhcpAnycastAddress());
    }

    return setting;
}

void NetworkManager::Settings::OlpcMeshSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << NM_SETTING_OLPC_MESH_SSID << ": " << ssid();
    qDebug() << NM_SETTING_OLPC_MESH_CHANNEL << ": " << channel();
    qDebug() << NM_SETTING_OLPC_MESH_DHCP_ANYCAST_ADDRESS << ": " << dhcpAnycastAddress();
}
