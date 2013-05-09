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

NetworkManager::DevicePrivate::DevicePrivate(const QString &path) : deviceIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus()), uni(path), designSpeed(0), dhcp4Config(0), dhcp6Config(0)
{
    activeConnection = deviceIface.activeConnection().path();
    driver = deviceIface.driver();
    interfaceName = deviceIface.interface();
    ipInterface = deviceIface.ipInterface();
    ipV4Address = QHostAddress(ntohl(deviceIface.ip4Address()));
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

    QDBusObjectPath ip4ConfigObjectPath = deviceIface.ip4Config();
    if (!ip4ConfigObjectPath.path().isNull() || ip4ConfigObjectPath.path() != QLatin1String("/")) {
        ipV4ConfigPath = ip4ConfigObjectPath.path();
    }

    QDBusObjectPath ip6ConfigObjectPath = deviceIface.ip6Config();
    if (!ip6ConfigObjectPath.path().isNull() || ip6ConfigObjectPath.path() != QLatin1String("/")) {
        ipV6ConfigPath = ip6ConfigObjectPath.path();
    }

    QDBusObjectPath dhcp4ConfigObjectPath = deviceIface.dhcp4Config();
    if (!dhcp4ConfigObjectPath.path().isNull()) {
        dhcp4ConfigPath = dhcp4ConfigObjectPath.path();
    }

    QDBusObjectPath dhcp6ConfigObjectPath = deviceIface.dhcp6Config();
    if (!dhcp6ConfigObjectPath.path().isNull()) {
        dhcp6ConfigPath = dhcp6ConfigObjectPath.path();
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
        QStringList newAvailableConnections;
        QList<QDBusObjectPath> availableConnections = qdbus_cast< QList<QDBusObjectPath> >(value);
        foreach (const QDBusObjectPath &availableConnection, availableConnections) {
            newAvailableConnections << availableConnection.path();
            if (!d->availableConnections.contains(availableConnection.path())) {
                d->availableConnections << availableConnection.path();
                emit availableConnectionAppeared(availableConnection.path());
            }
        }
        foreach (const QString & availableConnection, d->availableConnections) {
            if (!newAvailableConnections.contains(availableConnection)) {
                emit availableConnectionDisappeared(availableConnection);
                d->availableConnections.removeOne(availableConnection);
            }
        }
        emit availableConnectionChanged();
    } else if (property == QLatin1String("Capabilities")) {
        d->capabilities = NetworkManager::DevicePrivate::convertCapabilities(value.toUInt());
        emit capabilitiesChanged();
    } else if (property == QLatin1String("DeviceType")) {
        d->deviceType = static_cast<Device::Type>(value.toUInt());
    } else if (property == QLatin1String("Dhcp4Config")) {
        QDBusObjectPath dhcp4ConfigPath = value.value<QDBusObjectPath>();
        if (dhcp4ConfigPath.path().isNull()) {
            d->dhcp4Config.clear();
            d->dhcp4ConfigPath.clear();
        } else if (!d->dhcp4Config || d->dhcp4Config->path() != dhcp4ConfigPath.path()) {
            d->dhcp4Config.clear();
            d->dhcp4ConfigPath = dhcp4ConfigPath.path();
        }
        emit dhcp4ConfigChanged();
    } else if (property == QLatin1String("Dhcp6Config")) {
        QDBusObjectPath dhcp6ConfigPath = value.value<QDBusObjectPath>();
        if (dhcp6ConfigPath.path().isNull()) {
            d->dhcp6Config.clear();
            d->dhcp6ConfigPath.clear();
        } else if (!d->dhcp6Config || d->dhcp6Config->path() != dhcp6ConfigPath.path()) {
            d->dhcp6Config.clear();
            d->dhcp6ConfigPath = dhcp6ConfigPath.path();
        }
        emit dhcp6ConfigChanged();
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
        d->ipV4Address = QHostAddress(ntohl(value.toUInt()));
        emit ipV4AddressChanged();
    } else if (property == QLatin1String("Ip4Config")) {
        QDBusObjectPath ip4ConfigObjectPath = value.value<QDBusObjectPath>();
        if (ip4ConfigObjectPath.path().isNull() || ip4ConfigObjectPath.path() == QLatin1String("/")) {
            d->ipV4ConfigPath.clear();
        } else {
            d->ipV4ConfigPath = ip4ConfigObjectPath.path();
        }
        d->ipV4Config = IpConfig();
        emit ipV4ConfigChanged();
    } else if (property == QLatin1String("Ip6Config")) {
        QDBusObjectPath ip6ConfigObjectPath = value.value<QDBusObjectPath>();
        if (ip6ConfigObjectPath.path().isNull() || ip6ConfigObjectPath.path() == QLatin1String("/")) {
            d->ipV6ConfigPath.clear();
        } else {
            d->ipV6ConfigPath = ip6ConfigObjectPath.path();
        }
        d->ipV6Config = IpConfig();
        emit ipV6ConfigChanged();
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

NetworkManager::Connection::List NetworkManager::Device::availableConnections()
{
    Q_D(const Device);

    NetworkManager::Connection::List list;
    foreach (const QString &availableConnection, d->availableConnections) {
        NetworkManager::Connection::Ptr connection = NetworkManager::findConnection(availableConnection);
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

void NetworkManager::Device::setAutoconnect(bool autoconnect)
{
    Q_D(Device);
    d->deviceIface.setAutoconnect(autoconnect);
}

QString NetworkManager::Device::udi() const
{
    Q_D(const Device);
    return d->udi;
}

QHostAddress NetworkManager::Device::ipV4Address() const
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
    if (!d->ipV4Config.isValid() && !d->ipV4ConfigPath.isNull()) {
        d->ipV4Config.setIPv4Path(d->ipV4ConfigPath);
    }
    return d->ipV4Config;
}

NetworkManager::IpConfig NetworkManager::Device::ipV6Config() const
{
    Q_D(const Device);
    if (!d->ipV6Config.isValid() && !d->ipV6ConfigPath.isNull()) {
        d->ipV6Config.setIPv6Path(d->ipV6ConfigPath);
    }
    return d->ipV6Config;
}

NetworkManager::Dhcp4Config::Ptr NetworkManager::Device::dhcp4Config() const
{
    Q_D(const Device);
    if (!d->dhcp4Config && !d->dhcp4ConfigPath.isNull()) {
        d->dhcp4Config = NetworkManager::Dhcp4Config::Ptr(new Dhcp4Config(d->dhcp4ConfigPath), &QObject::deleteLater);
    }
    return d->dhcp4Config;
}

NetworkManager::Dhcp6Config::Ptr NetworkManager::Device::dhcp6Config() const
{
    Q_D(const Device);
    if (!d->dhcp6Config && !d->dhcp6ConfigPath.isNull()) {
        d->dhcp6Config = NetworkManager::Dhcp6Config::Ptr(new Dhcp6Config(d->dhcp6ConfigPath), &QObject::deleteLater);
    }
    return d->dhcp6Config;
}

bool NetworkManager::Device::isActive() const
{
    Q_D(const Device);
    return !(d->connectionState == NetworkManager::Device::Unavailable
             || d->connectionState == NetworkManager::Device::Unmanaged
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
