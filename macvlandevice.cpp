/*
    Copyright 2013 Jan Grulich <jgrulich@redhat.com>

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

#include "macvlandevice.h"
#include "device_p.h"
#include "manager.h"
#include "manager_p.h"

#include "nm-device-macvlan.h"

namespace NetworkManager
{
class MacVlanDevicePrivate: public DevicePrivate
{
public:
    MacVlanDevicePrivate(const QString &path, MacVlanDevice *q);
    virtual ~MacVlanDevicePrivate();

    OrgFreedesktopNetworkManagerDeviceMacvlanInterface iface;
};
}

NetworkManager::MacVlanDevicePrivate::MacVlanDevicePrivate(const QString &path, MacVlanDevice *q):
    DevicePrivate(path, q), iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

NetworkManager::MacVlanDevicePrivate::~MacVlanDevicePrivate()
{
}

NetworkManager::MacVlanDevice::MacVlanDevice(const QString &path, QObject *parent):
    Device(*new MacVlanDevicePrivate(path, this), parent)
{
    Q_D(MacVlanDevice);

    connect(&d->iface, SIGNAL(PropertiesChanged(QVariantMap)),
            this, SLOT(propertiesChanged(QVariantMap)));
}

NetworkManager::MacVlanDevice::~MacVlanDevice()
{
}

NetworkManager::Device::Type NetworkManager::MacVlanDevice::type() const
{
    return NetworkManager::Device::MacVlan;
}

QString NetworkManager::MacVlanDevice::mode() const
{
    Q_D(const MacVlanDevice);
    return d->iface.mode();
}

bool NetworkManager::MacVlanDevice::noPromisc() const
{
    Q_D(const MacVlanDevice);
    return d->iface.noPromisc();
}

QString NetworkManager::MacVlanDevice::parent() const
{
    Q_D(const MacVlanDevice);
    return d->iface.parent().path();
}

void NetworkManager::MacVlanDevice::propertyChanged(const QString &property, const QVariant &value)
{
    Q_D(MacVlanDevice);

    if (property == QLatin1String("Mode")) {
        emit modeChanged();
    } else if (property == QLatin1String("NoPromisc")) {
        emit noPromiscChanged();
    } else if (property == QLatin1String("Parent")) {
        emit parentChanged();
    } else {
        Device::propertyChanged(property, value);
    }
}

#include "macvlandevice.moc"
