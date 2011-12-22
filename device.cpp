/*
Copyright 2008,2010 Will Stephenson <wstephenson@kde.org>

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

#include "device.h"
#include "device_p.h"
#include "manager_p.h"

#include <arpa/inet.h>

#include "dbus/nm-ip4-configinterface.h"
#include "dbus/nm-ip6-configinterface.h"
#include "manager.h"

NetworkManager::DevicePrivate::DevicePrivate( const QString & path, QObject * owner ) : deviceIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus()), uni(path), designSpeed(0)
{
    Q_UNUSED(owner);
    driver = deviceIface.driver();
    interfaceName = deviceIface.interface();
    ipV4Address = deviceIface.ip4Address();
    managed = deviceIface.managed();
    udi = deviceIface.udi();
    firmwareMissing = deviceIface.firmwareMissing();
}

NetworkManager::DevicePrivate::~DevicePrivate()
{

}

NetworkManager::Device::Capabilities NetworkManager::DevicePrivate::convertCapabilities(uint theirCaps)
{
    NetworkManager::Device::Capabilities ourCaps
    = (NetworkManager::Device::Capabilities) theirCaps;
    return ourCaps;
}


NetworkManager::Device::State NetworkManager::DevicePrivate::convertState(uint theirState)
{
    NetworkManager::Device::State ourState = (NetworkManager::Device::State)theirState;
    return ourState;
}

NetworkManager::Device::StateChangeReason NetworkManager::DevicePrivate::convertReason(uint theirReason)
{
    NetworkManager::Device::StateChangeReason ourReason = (NetworkManager::Device::StateChangeReason)theirReason;
    return ourReason;
}


NetworkManager::Device::Device(const QString & path, QObject * parent) : QObject(parent), d_ptr(new DevicePrivate(path, this))
{
    init();
}

NetworkManager::Device::Device(DevicePrivate & dd,  QObject * parent) : QObject(parent), d_ptr(&dd)
{
    init();
}

void NetworkManager::Device::init()
{
    Q_D(Device);
    qDBusRegisterMetaType<UIntList>();
    qDBusRegisterMetaType<UIntListList>();
    qDBusRegisterMetaType<IpV6DBusAddress>();
    qDBusRegisterMetaType<IpV6DBusAddressList>();
    qDBusRegisterMetaType<IpV6DBusNameservers>();
    qDBusRegisterMetaType<IpV6DBusRoute>();
    qDBusRegisterMetaType<IpV6DBusRouteList>();
    d->capabilities = NetworkManager::DevicePrivate::convertCapabilities(d->deviceIface.capabilities());
    d->connectionState = NetworkManager::DevicePrivate::convertState(d->deviceIface.state());

    connect(&d->deviceIface, SIGNAL(StateChanged(uint,uint,uint)), this, SLOT(deviceStateChanged(uint,uint,uint)));
}

NetworkManager::Device::~Device()
{
    delete d_ptr;
}

QString NetworkManager::Device::uni() const
{
    Q_D(const Device);
    return d->uni;
}

void NetworkManager::Device::setUni(const QVariant & uni)
{
    Q_D(Device);
    d->uni = uni.toString();
}

QString NetworkManager::Device::interfaceName() const
{
    Q_D(const Device);
    return d->interfaceName;
}

void NetworkManager::Device::setInterfaceName(const QVariant & name)
{
    Q_D(Device);
    d->interfaceName = name.toString();
}

QString NetworkManager::Device::ipInterfaceName() const
{
    Q_D(const Device);
    return d->deviceIface.ipInterface();
}

QString NetworkManager::Device::driver() const
{
    Q_D(const Device);
    return d->driver;
}

NetworkManager::ActiveConnection *NetworkManager::Device::activeConnection()
{
    Q_D(const Device);
    return NetworkManager::findActiveConnection(d->deviceIface.activeConnection().path());
}

bool NetworkManager::Device::firmwareMissing() const
{
    Q_D(const Device);
    return d->firmwareMissing;
}

void NetworkManager::Device::setDriver(const QVariant & driver)
{
    Q_D(Device);
    d->driver = driver.toString();
}

QString NetworkManager::Device::udi() const
{
    Q_D(const Device);
    return d->udi;
}

int NetworkManager::Device::ipV4Address() const
{
    Q_D(const Device);
    return ntohl(d->ipV4Address);
}

NetworkManager::IPv4Config NetworkManager::Device::ipV4Config() const
{
    Q_D(const Device);
    if (d->connectionState != NetworkManager::Device::Activated) {
        return NetworkManager::IPv4Config();
    } else {
        // ask the daemon for the details
        QDBusObjectPath ipV4ConfigPath = d->deviceIface.ip4Config();
        OrgFreedesktopNetworkManagerIP4ConfigInterface iface(NetworkManagerPrivate::DBUS_SERVICE, ipV4ConfigPath.path(), QDBusConnection::systemBus());
        if (iface.isValid()) {
            //convert ipaddresses into object
            UIntListList addresses = iface.addresses();
            QList<NetworkManager::IPv4Address> addressObjects;
            foreach (const UIntList &addressList, addresses) {
                if ( addressList.count() == 3 ) {
                    NetworkManager::IPv4Address addr(ntohl(addressList[0]), ntohl(addressList[1]), ntohl(addressList[2]));
                    addressObjects.append(addr);
                }
            }
            //convert routes into objects
            UIntListList routes = iface.routes();
            QList<NetworkManager::IPv4Route> routeObjects;
            foreach (const UIntList &routeList, routes) {
                if ( routeList.count() == 4 ) {
                    NetworkManager::IPv4Route addr(ntohl(routeList[0]), ntohl(routeList[1]), ntohl(routeList[2]), ntohl(routeList[3]));
                    routeObjects.append(addr);
                }
            }
            // nameservers' IP addresses are always in network byte order
            return NetworkManager::IPv4Config(addressObjects,
                iface.nameservers(), iface.domains(),
                routeObjects);
        } else {
            return NetworkManager::IPv4Config();
        }
    }
}

NetworkManager::IPv6Config NetworkManager::Device::ipV6Config() const
{
    Q_D(const Device);
    if (d->connectionState != NetworkManager::Device::Activated) {
        return NetworkManager::IPv6Config();
    } else {
        // ask the daemon for the details
        QDBusObjectPath ipV6ConfigPath = d->deviceIface.ip6Config();
        OrgFreedesktopNetworkManagerIP6ConfigInterface iface(NetworkManagerPrivate::DBUS_SERVICE, ipV6ConfigPath.path(), QDBusConnection::systemBus());
        if (iface.isValid()) {
            IpV6DBusAddressList addresses = iface.addresses();
            QList<NetworkManager::IPv6Address> addressObjects;
            foreach (const IpV6DBusAddress &address, addresses) {
                Q_IPV6ADDR addr;
                Q_IPV6ADDR gateway;
                for (int i = 0; i < 16; i++) {
                    addr[i] = address.address[i];
                }
                for (int i = 0; i < 16; i++) {
                    gateway[i] = address.gateway[i];
                }
                NetworkManager::IPv6Address addressObject(addr, address.netMask, gateway);
                addressObjects.append(addressObject);
            }

            IpV6DBusRouteList routes = iface.routes();
            QList<NetworkManager::IPv6Route> routeObjects;
            foreach (const IpV6DBusRoute &route, routes) {
                Q_IPV6ADDR dest;
                Q_IPV6ADDR nexthop;
                for (int i = 0; i < 16; i++) {
                    dest[i] = route.destination[i];
                }
                for (int i = 0; i < 16; i++) {
                    nexthop[i] = route.nexthop[i];
                }
                NetworkManager::IPv6Route routeObject(dest, route.prefix, nexthop, route.metric);
                routeObjects.append(routeObject);
            }

            IpV6DBusNameservers nameservers = iface.nameservers();
            QList<Q_IPV6ADDR> nameserverList;
            foreach (const QByteArray &ns, nameservers) {
                Q_IPV6ADDR addr;
                for (int i = 0; i < 16; i++) {
                    addr[i] = (quint8)ns[i];
                }
                nameserverList.append(addr);
            }
            return NetworkManager::IPv6Config(addressObjects, nameserverList, iface.domains(), routeObjects);
        } else {
            return NetworkManager::IPv6Config();
        }
    }
}

bool NetworkManager::Device::isActive() const
{
    Q_D(const Device);
    return !(d->connectionState == NetworkManager::Device::Unavailable
            || d->connectionState == NetworkManager::Device::Disconnected
            || d->connectionState == NetworkManager::Device::Failed );
}

bool NetworkManager::Device::managed() const
{
    Q_D(const Device);
    return d->managed;
}

void NetworkManager::Device::disconnectInterface()
{
    Q_D(Device);
    d->deviceIface.Disconnect();
}

void NetworkManager::Device::setManaged(const QVariant & driver)
{
    Q_D(Device);
    d->driver = driver.toBool();
}

NetworkManager::Device::State NetworkManager::Device::state() const
{
    Q_D(const Device);
    return d->connectionState;
}

int NetworkManager::Device::designSpeed() const
{
    Q_D(const Device);
    return d->designSpeed;
}

NetworkManager::Device::Capabilities NetworkManager::Device::capabilities() const
{
    Q_D(const Device);
    return d->capabilities;
}

QVariant NetworkManager::Device::capabilitiesV() const
{
    Q_D(const Device);
    return QVariant(d->capabilities);
}

void NetworkManager::Device::setCapabilitiesV(const QVariant & caps)
{
    Q_D(Device);
    d->capabilities = NetworkManager::DevicePrivate::convertCapabilities(caps.toUInt());
}

void NetworkManager::Device::deviceStateChanged(uint new_state, uint old_state, uint reason)
{
    Q_D(Device);
    d->connectionState = NetworkManager::DevicePrivate::convertState(new_state);
    emit stateChanged(d->connectionState, NetworkManager::DevicePrivate::convertState(old_state), NetworkManager::DevicePrivate::convertReason(reason));
}

NetworkManager::Device::Type NetworkManager::Device::type() const
{
        return NetworkManager::Device::UnknownType;
}

