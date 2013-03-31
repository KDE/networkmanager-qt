/*
Copyright 2011 Lamarque Souza <lamarque@kde.org>
Copyright 2011 Will Stephenson <wstephenson@kde.org>

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

#include "bluetoothdevice.h"
#include "bluetoothdevice_p.h"
#include "manager_p.h"

#include "nmdebug.h"

NetworkManager::BluetoothDevicePrivate::BluetoothDevicePrivate(const QString & path, QObject * owner)
    : ModemDevicePrivate(path, owner), btIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
    btCapabilities = static_cast<QFlags<NetworkManager::BluetoothDevice::Capability> >(btIface.btCapabilities());
    hardwareAddress = btIface.hwAddress();
    name = btIface.name();
}

NetworkManager::BluetoothDevice::BluetoothDevice(const QString & path, QObject * parent)
    : ModemDevice(*new BluetoothDevicePrivate(path, this), parent)
{
    Q_D(BluetoothDevice);
    connect(&d->btIface, SIGNAL(PropertiesChanged(QVariantMap)),
            this, SLOT(btPropertiesChanged(QVariantMap)));
}

NetworkManager::BluetoothDevice::~BluetoothDevice()
{
}

NetworkManager::Device::Type NetworkManager::BluetoothDevice::type() const
{
    return NetworkManager::Device::Bluetooth;
}

void NetworkManager::BluetoothDevice::btPropertiesChanged(const QVariantMap & changedProperties)
{
    nmDebug() << changedProperties.keys();
    Q_D(BluetoothDevice);
    QLatin1String hwAddrKey("HwAddress"),
                  name("Name"),
                  capabilities("BtCapabilities");
    QVariantMap::const_iterator it = changedProperties.find(hwAddrKey);
    it = changedProperties.find(hwAddrKey);
    if (it != changedProperties.end()) {
        d->hardwareAddress = it->toString();
    }
    it = changedProperties.find(name);
    if (it != changedProperties.end()) {
        d->name = it->toString();
        emit nameChanged(d->name);
    }
    if (it != changedProperties.end()) {
        d->btCapabilities = static_cast<NetworkManager::BluetoothDevice::Capabilities>(it->toUInt());
    }
}

NetworkManager::BluetoothDevice::Capabilities NetworkManager::BluetoothDevice::bluetoothCapabilities() const
{
    Q_D(const BluetoothDevice);
    return d->btCapabilities;
}

QString NetworkManager::BluetoothDevice::hardwareAddress() const
{
    Q_D(const BluetoothDevice);
    return d->hardwareAddress;
}

QString NetworkManager::BluetoothDevice::name() const
{
    Q_D(const BluetoothDevice);
    return d->name;
}

// vim: sw=4 sts=4 et tw=100

#include "bluetoothdevice.moc"
