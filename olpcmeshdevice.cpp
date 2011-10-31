/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "olpcmeshdevice.h"
#include "olpcmeshdevice_p.h"
#include "manager_p.h"

#include "wimaxnsp.h"

NetworkManager::OlpcMeshDevicePrivate::OlpcMeshDevicePrivate(const QString & path, QObject * owner)
    : DevicePrivate(path, owner), iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
{

}

NetworkManager::OlpcMeshDevice::OlpcMeshDevice(const QString & path, QObject * parent)
    : Device(*new OlpcMeshDevicePrivate(path, this), parent)
{
    Q_D(OlpcMeshDevice);
    d->hardwareAddress = d->iface.hwAddress();
    d->activeChannel = d->iface.activeChannel();
    d->companion = d->iface.companion().path();

    connect( &d->iface, SIGNAL(PropertiesChanged(const QVariantMap &)),
                this, SLOT(propertiesChanged(const QVariantMap &)));
}

NetworkManager::OlpcMeshDevice::~OlpcMeshDevice()
{

}

NetworkManager::Device::Type NetworkManager::OlpcMeshDevice::type() const
{
    return NetworkManager::Device::OlpcMesh;
}

QString NetworkManager::OlpcMeshDevice::hardwareAddress() const
{
    Q_D(const OlpcMeshDevice);
    return d->hardwareAddress;
}

uint NetworkManager::OlpcMeshDevice::activeChannel() const
{
    Q_D(const OlpcMeshDevice);
    return d->activeChannel;
}

NetworkManager::Device* NetworkManager::OlpcMeshDevice::companionDevice() const
{
    Q_D(const OlpcMeshDevice);
    return NetworkManager::findNetworkInterface(d->companion);
}

void NetworkManager::OlpcMeshDevice::propertiesChanged(const QVariantMap & changedProperties)
{
    //nmDebug() << changedProperties.keys();
    QStringList propKeys = changedProperties.keys();
    Q_D(OlpcMeshDevice);
    QLatin1String activeChannelKey("ActiveChannel"),
                  hwAddrKey("HwAddress"),
                  companionKey("Companion");
    QVariantMap::const_iterator it = changedProperties.find(activeChannelKey);
    if (it != changedProperties.end()) {
        d->activeChannel = it->toUInt();
        emit activeChannelChanged(d->activeChannel);
        propKeys.removeOne(activeChannelKey);
    }
    it = changedProperties.find(hwAddrKey);
    if (it != changedProperties.end()) {
        d->hardwareAddress = it->toString();
        emit hardwareAddressChanged(d->hardwareAddress);
        propKeys.removeOne(hwAddrKey);
    }
    it = changedProperties.find(companionKey);
    if (it != changedProperties.end()) {
        d->companion = qdbus_cast<QDBusObjectPath>(*it).path();
        emit companionChanged(NetworkManager::findNetworkInterface(d->companion));
        propKeys.removeOne(companionKey);
    }
    //if (propKeys.count()) {
    //    nmDebug() << "Unhandled properties: " << propKeys;
    //}
}
