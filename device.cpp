/*
Copyright 2008,2010 Will Stephenson <wstephenson@kde.org>
Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>

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
#include "manager.h"
#include "manager_p.h"
#include "connection.h"
#include "settings.h"
#include "nmdebug.h"

#include <arpa/inet.h>

#include "dbus/nm-ip4-configinterface.h"
#include "dbus/nm-ip6-configinterface.h"

namespace NetworkManager
{
class DeviceStateReason::Private
{
public:
    Private(Device::State st, Device::StateChangeReason rsn):
	state(st),
	reason(rsn)
    {}
    Private():
	state(Device::UnknownState),
	reason(Device::UnknownReason)
    {}
    Device::State state;
    Device::StateChangeReason reason;
};
}

NetworkManager::DeviceStateReason::DeviceStateReason(Device::State state, Device::StateChangeReason reason)
: d(new Private(state, reason))
{
}

NetworkManager::DeviceStateReason::DeviceStateReason(const NetworkManager::DeviceStateReason &other)
: d(new Private(*other.d))
{
}

NetworkManager::DeviceStateReason::~DeviceStateReason()
{
    delete d;
}

NetworkManager::Device::State NetworkManager::DeviceStateReason::state() const
{
    return d->state;
}

NetworkManager::Device::StateChangeReason NetworkManager::DeviceStateReason::reason() const
{
    return d->reason;
}

NetworkManager::DeviceStateReason & NetworkManager::DeviceStateReason::operator=(const NetworkManager::DeviceStateReason &other)
{
    if (&other != this) {
	*d = *other.d;
    }
    return *this;
}

void NetworkManager::DeviceStateReason::setState(const Device::State state)
{
    d->state = state;
}

void NetworkManager::DeviceStateReason::setReason(const Device::StateChangeReason reason)
{
    d->reason = reason;
}

NetworkManager::DevicePrivate::DevicePrivate( const QString & path) : deviceIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus()), uni(path), designSpeed(0), dhcp4Config(0), dhcp6Config(0)
{
    activeConnection = deviceIface.activeConnection().path();
    driver = deviceIface.driver();
    interfaceName = deviceIface.interface();
    ipInterface = deviceIface.ipInterface();
    ipV4Address = deviceIface.ip4Address();
    managed = deviceIface.managed();
    udi = deviceIface.udi();
    firmwareMissing = deviceIface.firmwareMissing();
    driverVersion = deviceIface.driverVersion();
    firmwareVersion = deviceIface.firmwareVersion();
    autoconnect = deviceIface.autoconnect();
    reason = NetworkManager::DevicePrivate::convertReason(deviceIface.stateReason().reason);
    foreach (const QDBusObjectPath &availableConnection, deviceIface.availableConnections()) {
        availableConnections << availableConnection.path();
    }
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

NetworkManager::Device::Device(const QString & path, QObject * parent) : QObject(parent), d_ptr(new DevicePrivate(path))
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
    qDBusRegisterMetaType<DeviceDBusStateReason>();
    d->capabilities = NetworkManager::DevicePrivate::convertCapabilities(d->deviceIface.capabilities());
    d->connectionState = NetworkManager::DevicePrivate::convertState(d->deviceIface.state());
    d->deviceType = static_cast<Device::Type>(d->deviceIface.deviceType());

    connect(&d->deviceIface, SIGNAL(StateChanged(uint,uint,uint)), this, SLOT(deviceStateChanged(uint,uint,uint)));
}

void NetworkManager::Device::propertyChanged(const QString &property, const QVariant &value)
{
    Q_D(Device);

    if (property == QLatin1String("ActiveConnection")) {
        d->activeConnection = value.value<QDBusObjectPath>().path();
        emit activeConnectionChanged();
    } else if (property == QLatin1String("Autoconnect")) {
        d->autoconnect = value.toBool();
        emit autoconnectChanged();
    } else if (property == QLatin1String("AvailableConnections")) {
        d->availableConnections.clear();
        QList<QDBusObjectPath> availableConnections = qdbus_cast< QList<QDBusObjectPath> >(value);
        foreach (const QDBusObjectPath &availableConnection, availableConnections) {
            d->availableConnections << availableConnection.path();
        }
        emit availableConnectionChanged();
    } else if (property == QLatin1String("Capabilities")) {
        d->capabilities = NetworkManager::DevicePrivate::convertCapabilities(value.toUInt());
        emit capabilitiesChanged();
    } else if (property == QLatin1String("DeviceType")) {
        d->deviceType = static_cast<Device::Type>(value.toUInt());
    } else if (property == QLatin1String("Dhcp4Config")) {
//        d->dhcp4Config = it->toUInt() * 1000;
//        emit bitRateChanged(d->bitrate);
    } else if (property == QLatin1String("Dhcp6Config")) {
//        d->bitrate = it->toUInt() * 1000;
//        emit bitRateChanged(d->bitrate);
    } else if (property == QLatin1String("Driver")) {
        d->driver = value.toString();
        emit driverChanged();
    } else if (property == QLatin1String("DriverVersion")) {
        d->driverVersion = value.toString();
        emit driverVersionChanged();
    } else if (property == QLatin1String("FirmwareMissing")) {
        d->firmwareMissing = value.toBool();
        emit firmwareMissingChanged();
    } else if (property == QLatin1String("FirmwareVersion")) {
        d->firmwareVersion = value.toString();
        emit firmwareVersionChanged();
    } else if (property == QLatin1String("Interface")) {
        d->interfaceName = value.toString();
        emit interfaceNameChanged();
    } else if (property == QLatin1String("Ip4Address")) {
        d->ipV4Address = value.toUInt();
        emit ipV4AddressChanged();
    } else if (property == QLatin1String("Ip4Config")) {
//        d->ipV4Config = it->toUInt() * 1000;
//        emit bitRateChanged(d->bitrate);
    } else if (property == QLatin1String("Ip6Config")) {
//        d->bitrate = it->toUInt() * 1000;
//        emit bitRateChanged(d->bitrate);
    } else if (property == QLatin1String("IpInterface")) {
        d->ipInterface = value.toString();
        emit ipInterfaceChanged();
    } else if (property == QLatin1String("Managed")) {
        d->managed = value.toBool();
        emit managedChanged();
    } else if (property == QLatin1String("State")) {
        d->connectionState = NetworkManager::DevicePrivate::convertState(value.toUInt());
        emit connectionStateChanged();
    } else if (property == QLatin1String("StateReason")) { // just extracting the reason
        d->reason = NetworkManager::DevicePrivate::convertReason(qdbus_cast<DeviceDBusStateReason>(value).reason);
        emit stateReasonChanged();
    } else if (property == QLatin1String("Udi")) {
        d->udi = value.toString();
        emit udiChanged();
    } else {
        qWarning() << Q_FUNC_INFO << "Unhandled property" << property;
    }
}

NetworkManager::Device::~Device()
{
    Q_D(Device);
    delete d;
}

QString NetworkManager::Device::uni() const
{
    Q_D(const Device);
    return d->uni;
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
    return d->ipInterface;
}

QString NetworkManager::Device::driver() const
{
    Q_D(const Device);
    return d->driver;
}

QString NetworkManager::Device::driverVersion() const
{
    Q_D(const Device);
    return d->driverVersion;
}

QString NetworkManager::Device::firmwareVersion() const
{
    Q_D(const Device);
    return d->firmwareVersion;
}

NetworkManager::ActiveConnection::Ptr NetworkManager::Device::activeConnection() const
{
    Q_D(const Device);
    return NetworkManager::findActiveConnection(d->activeConnection);
}

NetworkManager::Settings::Connection::List NetworkManager::Device::availableConnections()
{
    Q_D(const Device);

    NetworkManager::Settings::Connection::List list;
    foreach (const QString &availableConnection, d->availableConnections) {
        NetworkManager::Settings::Connection::Ptr connection = NetworkManager::Settings::findConnection(availableConnection);
        if (connection) {
            list << connection;
        }
    }

    return list;
}

bool NetworkManager::Device::firmwareMissing() const
{
    Q_D(const Device);
    return d->firmwareMissing;
}

bool NetworkManager::Device::autoconnect() const
{
    Q_D(const Device);
    return d->autoconnect;
}

void NetworkManager::Device::setAutoconnect(const QVariant & autoconnect)
{
    Q_D(Device);
    d->autoconnect = autoconnect.toBool();
    d->deviceIface.setAutoconnect(d->autoconnect);
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
    return d->ipV4Address;
}

NetworkManager::DeviceStateReason NetworkManager::Device::stateReason() const
{
    Q_D(const Device);
    return DeviceStateReason(d->connectionState, d->reason);
}

NetworkManager::IpConfig NetworkManager::Device::ipV4Config() const
{
    Q_D(const Device);
    if (d->connectionState != NetworkManager::Device::Activated) {
        return NetworkManager::IpConfig();
    } else {
        // ask the daemon for the details
        QDBusObjectPath ipV4ConfigPath = d->deviceIface.ip4Config();
        OrgFreedesktopNetworkManagerIP4ConfigInterface iface(NetworkManagerPrivate::DBUS_SERVICE, ipV4ConfigPath.path(), QDBusConnection::systemBus());
        if (iface.isValid()) {
            //convert ipaddresses into object
            UIntListList addresses = iface.addresses();
            QList<NetworkManager::IpAddress> addressObjects;
            foreach (const UIntList &addressList, addresses) {
                if ( addressList.count() == 3 ) {
                    NetworkManager::IpAddress address;
                    address.setIp(QHostAddress(ntohl(addressList[0])));
                    address.setPrefixLength(addressList[1]);
                    address.setGateway(QHostAddress(ntohl(addressList[2])));
                    addressObjects << address;
                }
            }
            //convert routes into objects
            UIntListList routes = iface.routes();
            QList<NetworkManager::IpRoute> routeObjects;
            foreach (const UIntList &routeList, routes) {
                if ( routeList.count() == 4 ) {
                    NetworkManager::IpRoute route;
                    route.setIp(QHostAddress(ntohl(routeList[0])));
                    route.setPrefixLength(routeList[1]);
                    route.setNextHop(QHostAddress(ntohl(routeList[2])));
                    route.setMetric(ntohl(routeList[3]));
                    routeObjects << route;
                }
            }
            // nameservers' IP addresses are always in network byte order
            QList<QHostAddress> nameservers;
            foreach (uint nameserver, iface.nameservers()) {
                nameservers << QHostAddress(ntohl(nameserver));
            }
            return NetworkManager::IpConfig(addressObjects,
                nameservers, iface.domains(),
                routeObjects);
        } else {
            return NetworkManager::IpConfig();
        }
    }
}

NetworkManager::IpConfig NetworkManager::Device::ipV6Config() const
{
    Q_D(const Device);
    if (d->connectionState != NetworkManager::Device::Activated) {
        return NetworkManager::IpConfig();
    } else {
        // ask the daemon for the details
        QDBusObjectPath ipV6ConfigPath = d->deviceIface.ip6Config();
        OrgFreedesktopNetworkManagerIP6ConfigInterface iface(NetworkManagerPrivate::DBUS_SERVICE, ipV6ConfigPath.path(), QDBusConnection::systemBus());
        if (iface.isValid()) {
            IpV6DBusAddressList addresses = iface.addresses();
            QList<NetworkManager::IpAddress> addressObjects;
            foreach (const IpV6DBusAddress &address, addresses) {
                Q_IPV6ADDR addr;
                Q_IPV6ADDR gateway;
                for (int i = 0; i < 16; i++) {
                    addr[i] = address.address[i];
                }
                for (int i = 0; i < 16; i++) {
                    gateway[i] = address.gateway[i];
                }
                NetworkManager::IpAddress addressEntry;
                addressEntry.setIp(QHostAddress(addr));
                addressEntry.setPrefixLength(address.prefix);
                addressEntry.setGateway(QHostAddress(gateway));
                addressObjects << addressEntry;
            }

            IpV6DBusRouteList routes = iface.routes();
            QList<NetworkManager::IpRoute> routeObjects;
            foreach (const IpV6DBusRoute &route, routes) {
                Q_IPV6ADDR dest;
                Q_IPV6ADDR nexthop;
                for (int i = 0; i < 16; i++) {
                    dest[i] = route.destination[i];
                }
                for (int i = 0; i < 16; i++) {
                    nexthop[i] = route.nexthop[i];
                }
                NetworkManager::IpRoute routeEntry;
                routeEntry.setIp(QHostAddress(dest));
                routeEntry.setPrefixLength(route.prefix);
                routeEntry.setNextHop(QHostAddress(nexthop));
                routeEntry.setMetric(route.metric);
                routeObjects << routeEntry;
            }

            IpV6DBusNameservers nameservers = iface.nameservers();
            QList<QHostAddress> nameserverList;
            foreach (const QByteArray &ns, nameservers) {
                Q_IPV6ADDR addr;
                for (int i = 0; i < 16; i++) {
                    addr[i] = (quint8)ns[i];
                }
                nameserverList << QHostAddress(addr);
            }
            return NetworkManager::IpConfig(addressObjects, nameserverList, iface.domains(), routeObjects);
        } else {
            return NetworkManager::IpConfig();
        }
    }
}

NetworkManager::Dhcp4Config::Ptr NetworkManager::Device::dhcp4Config()
{
    Q_D(Device);
    QDBusObjectPath objPath = d->deviceIface.dhcp4Config();
    if (d->connectionState != NetworkManager::Device::Activated ||
        objPath.path().isEmpty()) {
        d->dhcp4Config.clear();
        return d->dhcp4Config;
    }

    if (!d->dhcp4Config || d->dhcp4Config->path() != objPath.path()) {
        d->dhcp4Config = NetworkManager::Dhcp4Config::Ptr(new Dhcp4Config(objPath.path()));
    }
    return d->dhcp4Config;
}

NetworkManager::Dhcp6Config::Ptr NetworkManager::Device::dhcp6Config()
{
    Q_D(Device);
    QDBusObjectPath objPath = d->deviceIface.dhcp6Config();
    if (d->connectionState != NetworkManager::Device::Activated ||
        objPath.path().isEmpty()) {
        d->dhcp6Config.clear();
        return d->dhcp6Config;
    }

    if (!d->dhcp6Config || d->dhcp6Config->path() != objPath.path()) {
        d->dhcp6Config = NetworkManager::Dhcp6Config::Ptr(new Dhcp6Config(objPath.path()));
    }
    return d->dhcp6Config;
}

bool NetworkManager::Device::isActive() const
{
    Q_D(const Device);
    return !(d->connectionState == NetworkManager::Device::Unavailable
            || d->connectionState == NetworkManager::Device::Disconnected
             || d->connectionState == NetworkManager::Device::Failed );
}

bool NetworkManager::Device::isValid() const
{
    Q_D(const Device);
    return d->deviceIface.isValid();
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

void NetworkManager::Device::setManaged(const QVariant & managed)
{
    Q_D(Device);
    d->managed = managed.toBool();
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
    d->reason = NetworkManager::DevicePrivate::convertReason(reason);

    emit stateChanged(d->connectionState, NetworkManager::DevicePrivate::convertState(old_state), NetworkManager::DevicePrivate::convertReason(reason));
}

void NetworkManager::Device::propertiesChanged(const QVariantMap &properties)
{
    QVariantMap::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        propertyChanged(it.key(), it.value());
        ++it;
    }
}

NetworkManager::Device::Type NetworkManager::Device::type() const
{
    Q_D(const Device);
    return d->deviceType;
}

#include "device.moc"
