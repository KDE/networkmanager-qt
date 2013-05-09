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

#include "bluetoothsetting.h"
#include "bluetoothsetting_p.h"

#include <nm-setting-bluetooth.h>

#include <QtCore/QDebug>

NetworkManager::BluetoothSettingPrivate::BluetoothSettingPrivate():
    name(NM_SETTING_BLUETOOTH_SETTING_NAME),
    profileType(BluetoothSetting::Unknown)
{ }

NetworkManager::BluetoothSetting::BluetoothSetting():
    Setting(Setting::Bluetooth),
    d_ptr(new BluetoothSettingPrivate())
{ }

NetworkManager::BluetoothSetting::BluetoothSetting(const Ptr &other):
    Setting(other),
    d_ptr(new BluetoothSettingPrivate())
{
    setBluetoothAddress(other->bluetoothAddress());
    setProfileType(other->profileType());
}

NetworkManager::BluetoothSetting::~BluetoothSetting()
{
    delete d_ptr;
}

QString NetworkManager::BluetoothSetting::name() const
{
    Q_D(const BluetoothSetting);

    return d->name;
}

void NetworkManager::BluetoothSetting::setBluetoothAddress(const QByteArray& address)
{
    Q_D(BluetoothSetting);

    d->bdaddr = address;
}

QByteArray NetworkManager::BluetoothSetting::bluetoothAddress() const
{
    Q_D(const BluetoothSetting);

    return d->bdaddr;
}

void NetworkManager::BluetoothSetting::setProfileType(NetworkManager::BluetoothSetting::ProfileType type)
{
    Q_D(BluetoothSetting);

    d->profileType = type;
}

NetworkManager::BluetoothSetting::ProfileType NetworkManager::BluetoothSetting::profileType() const
{
    Q_D(const BluetoothSetting);

    return d->profileType;
}

void NetworkManager::BluetoothSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_BLUETOOTH_BDADDR))) {
        setBluetoothAddress(setting.value(QLatin1String(NM_SETTING_BLUETOOTH_BDADDR)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_BLUETOOTH_TYPE))) {
        QString type = setting.value(QLatin1String(NM_SETTING_BLUETOOTH_TYPE)).toString();

        if (type == QLatin1String(NM_SETTING_BLUETOOTH_TYPE_DUN)) {
            setProfileType(Dun);
        } else if (type == QLatin1String(NM_SETTING_BLUETOOTH_TYPE_PANU)) {
            setProfileType(Panu);
        }
    }
}

QVariantMap NetworkManager::BluetoothSetting::toMap() const
{
    QVariantMap setting;

    if (!bluetoothAddress().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_BLUETOOTH_BDADDR), bluetoothAddress());
    }

    switch (profileType()) {
    case Dun:
        setting.insert(QLatin1String(NM_SETTING_BLUETOOTH_TYPE), QLatin1String(NM_SETTING_BLUETOOTH_TYPE_DUN));
        break;
    case Panu:
        setting.insert(QLatin1String(NM_SETTING_BLUETOOTH_TYPE), QLatin1String(NM_SETTING_BLUETOOTH_TYPE_PANU));
        break;
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::BluetoothSetting &setting)
{
    dbg.nospace() << static_cast<NetworkManager::Setting>(setting);

    dbg.nospace() << NM_SETTING_BLUETOOTH_BDADDR << ": " << setting.bluetoothAddress() << '\n';
    dbg.nospace() << NM_SETTING_BLUETOOTH_TYPE << ": " << setting.profileType() << '\n';

    return dbg.maybeSpace();
}
