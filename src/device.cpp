/*
    Copyright 2008,2010 Will Stephenson <wstephenson@kde.org>
    Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>
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

#include "device_p.h"
#include "manager.h"
#include "manager_p.h"
#include "connection.h"
#include "settings.h"
#include "nmdebug.h"

#include <arpa/inet.h>

#include <QDBusObjectPath>

Q_LOGGING_CATEGORY(NMQT, "networkmanager-qt")

namespace NetworkManager
{
class DeviceStateReasonPrivate
{
public:
    DeviceStateReasonPrivate(Device::State st, Device::StateChangeReason rsn)
        : state(st)
        , reason(rsn)
    {}
    DeviceStateReasonPrivate()
        : state(Device::UnknownState)
        , reason(Device::UnknownReason)
    {}
    Device::State state;
    Device::StateChangeReason reason;
};
}

NetworkManager::DeviceStateReason::DeviceStateReason(Device::State state, Device::StateChangeReason reason)
    : d_ptr(new DeviceStateReasonPrivate(state, reason))
{
}

NetworkManager::DeviceStateReason::DeviceStateReason(const NetworkManager::DeviceStateReason &other)
    : d_ptr(new DeviceStateReasonPrivate(*other.d_ptr))
{
}

NetworkManager::DeviceStateReason::~DeviceStateReason()
{
    delete d_ptr;
}

NetworkManager::Device::State NetworkManager::DeviceStateReason::state() const
{
    Q_D(const DeviceStateReason);
    return d->state;
}

NetworkManager::Device::StateChangeReason NetworkManager::DeviceStateReason::reason() const
{
    Q_D(const DeviceStateReason);
    return d->reason;
}

NetworkManager::DeviceStateReason &NetworkManager::DeviceStateReason::operator=(const NetworkManager::DeviceStateReason &other)
{
    if (&other != this) {
        *d_ptr = *other.d_ptr;
    }
    return *this;
}

NetworkManager::DevicePrivate::DevicePrivate(const QString &path, NetworkManager::Device *q)
#ifdef NMQT_STATIC
    : deviceIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    : deviceIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , uni(path)
    , designSpeed(0)
    , dhcp4Config(0)
    , dhcp6Config(0)
#if NM_CHECK_VERSION(0, 9, 10)
    , mtu(0)
#endif
    , q_ptr(q)
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
    Q_FOREACH (const QDBusObjectPath & availableConnection, deviceIface.availableConnections()) {
        availableConnections << availableConnection.path();
    }
#if NM_CHECK_VERSION(0, 9, 10)
    physicalPortId = deviceIface.physicalPortId();
    mtu = deviceIface.mtu();
#endif
    QDBusObjectPath ip4ConfigObjectPath = deviceIface.ip4Config();
    if (!ip4ConfigObjectPath.path().isNull() || ip4ConfigObjectPath.path() != QLatin1String("/")) {
        ipV4ConfigPath = ip4ConfigObjectPath.path();
    }

    QDBusObjectPath ip6ConfigObjectPath = deviceIface.ip6Config();
    if (!ip6ConfigObjectPath.path().isNull() || ip6ConfigObjectPath.path() != QLatin1String("/")) {
        ipV6ConfigPath = ip6ConfigObjectPath.path();
    }

    QDBusObjectPath dhcp4ConfigObjectPath = deviceIface.dhcp4Config();
    if (!dhcp4ConfigObjectPath.path().isNull() && dhcp4ConfigObjectPath.path() != QLatin1String("/")) {
        dhcp4ConfigPath = dhcp4ConfigObjectPath.path();
    }

    QDBusObjectPath dhcp6ConfigObjectPath = deviceIface.dhcp6Config();
    if (!dhcp6ConfigObjectPath.path().isNull() && dhcp6ConfigObjectPath.path() != QLatin1String("/")) {
        dhcp6ConfigPath = dhcp6ConfigObjectPath.path();
    }
}

NetworkManager::DevicePrivate::~DevicePrivate()
{
}

void NetworkManager::DevicePrivate::init()
{
    qDBusRegisterMetaType<UIntList>();
    qDBusRegisterMetaType<UIntListList>();
    qDBusRegisterMetaType<IpV6DBusAddress>();
    qDBusRegisterMetaType<IpV6DBusAddressList>();
    qDBusRegisterMetaType<IpV6DBusNameservers>();
    qDBusRegisterMetaType<IpV6DBusRoute>();
    qDBusRegisterMetaType<IpV6DBusRouteList>();
    qDBusRegisterMetaType<DeviceDBusStateReason>();
    capabilities = convertCapabilities(deviceIface.capabilities());
    connectionState = convertState(deviceIface.state());
    deviceType = static_cast<Device::Type>(deviceIface.deviceType());

    QObject::connect(&deviceIface, &OrgFreedesktopNetworkManagerDeviceInterface::StateChanged, this, &DevicePrivate::deviceStateChanged);
}

NetworkManager::Device::Capabilities NetworkManager::DevicePrivate::convertCapabilities(uint theirCaps)
{
    NetworkManager::Device::Capabilities ourCaps
        = (NetworkManager::Device::Capabilities) theirCaps;
    return ourCaps;
}

NetworkManager::Device::State NetworkManager::DevicePrivate::convertState(uint theirState)
{
    NetworkManager::Device::State ourState = static_cast<NetworkManager::Device::State>(theirState);
    return ourState;
}

NetworkManager::Device::StateChangeReason NetworkManager::DevicePrivate::convertReason(uint theirReason)
{
    NetworkManager::Device::StateChangeReason ourReason = (NetworkManager::Device::StateChangeReason)theirReason;
    return ourReason;
}

NetworkManager::Device::Device(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new DevicePrivate(path, this))
{
    Q_D(Device);

    d->init();
}

NetworkManager::Device::Device(DevicePrivate &dd,  QObject *parent)
    : QObject(parent)
    , d_ptr(&dd)
{
    Q_D(Device);

    d->init();
}

void NetworkManager::DevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(Device);

    if (property == QLatin1String("ActiveConnection")) {
        // FIXME workaround, because NM doesn't Q_EMIT correct value
        // d->activeConnection = value.value<QDBusObjectPath>.path();
        activeConnection = deviceIface.activeConnection().path();
        Q_EMIT q->activeConnectionChanged();
    } else if (property == QLatin1String("Autoconnect")) {
        autoconnect = value.toBool();
        Q_EMIT q->autoconnectChanged();
    } else if (property == QLatin1String("AvailableConnections")) {
        QStringList newAvailableConnections;
        QList<QDBusObjectPath> availableConnectionsTmp = qdbus_cast< QList<QDBusObjectPath> >(value);
        Q_FOREACH (const QDBusObjectPath & availableConnection, availableConnectionsTmp) {
            newAvailableConnections << availableConnection.path();
            if (!availableConnections.contains(availableConnection.path())) {
                availableConnections << availableConnection.path();
                Q_EMIT q->availableConnectionAppeared(availableConnection.path());
            }
        }
        Q_FOREACH (const QString & availableConnection, availableConnections) {
            if (!newAvailableConnections.contains(availableConnection)) {
                availableConnections.removeOne(availableConnection);
                Q_EMIT q->availableConnectionDisappeared(availableConnection);
            }
        }
        Q_EMIT q->availableConnectionChanged();
    } else if (property == QLatin1String("Capabilities")) {
        capabilities = NetworkManager::DevicePrivate::convertCapabilities(value.toUInt());
        Q_EMIT q->capabilitiesChanged();
    } else if (property == QLatin1String("DeviceType")) {
        deviceType = static_cast<Device::Type>(value.toUInt());
    } else if (property == QLatin1String("Dhcp4Config")) {
        QDBusObjectPath dhcp4ConfigPathTmp = value.value<QDBusObjectPath>();
        if (dhcp4ConfigPathTmp.path().isNull()) {
            dhcp4Config.clear();
            dhcp4ConfigPath.clear();
        } else if (!dhcp4Config || dhcp4Config->path() != dhcp4ConfigPathTmp.path()) {
            dhcp4Config.clear();
            dhcp4ConfigPath = dhcp4ConfigPathTmp.path();
        }
        Q_EMIT q->dhcp4ConfigChanged();
    } else if (property == QLatin1String("Dhcp6Config")) {
        QDBusObjectPath dhcp6ConfigPathTmp = value.value<QDBusObjectPath>();
        if (dhcp6ConfigPathTmp.path().isNull()) {
            dhcp6Config.clear();
            dhcp6ConfigPath.clear();
        } else if (!dhcp6Config || dhcp6Config->path() != dhcp6ConfigPathTmp.path()) {
            dhcp6Config.clear();
            dhcp6ConfigPath = dhcp6ConfigPathTmp.path();
        }
        Q_EMIT q->dhcp6ConfigChanged();
    } else if (property == QLatin1String("Driver")) {
        driver = value.toString();
        Q_EMIT q->driverChanged();
    } else if (property == QLatin1String("DriverVersion")) {
        driverVersion = value.toString();
        Q_EMIT q->driverVersionChanged();
    } else if (property == QLatin1String("FirmwareMissing")) {
        firmwareMissing = value.toBool();
        Q_EMIT q->firmwareMissingChanged();
    } else if (property == QLatin1String("FirmwareVersion")) {
        firmwareVersion = value.toString();
        Q_EMIT q->firmwareVersionChanged();
    } else if (property == QLatin1String("Interface")) {
        interfaceName = value.toString();
        Q_EMIT q->interfaceNameChanged();
    } else if (property == QLatin1String("Ip4Address")) {
        ipV4Address = QHostAddress(ntohl(value.toUInt()));
        Q_EMIT q->ipV4AddressChanged();
    } else if (property == QLatin1String("Ip4Config")) {
        QDBusObjectPath ip4ConfigObjectPathTmp = value.value<QDBusObjectPath>();
        if (ip4ConfigObjectPathTmp.path().isNull() || ip4ConfigObjectPathTmp.path() == QLatin1String("/")) {
            ipV4ConfigPath.clear();
        } else {
            ipV4ConfigPath = ip4ConfigObjectPathTmp.path();
        }
        ipV4Config = IpConfig();
        Q_EMIT q->ipV4ConfigChanged();
    } else if (property == QLatin1String("Ip6Config")) {
        QDBusObjectPath ip6ConfigObjectPathTmp = value.value<QDBusObjectPath>();
        if (ip6ConfigObjectPathTmp.path().isNull() || ip6ConfigObjectPathTmp.path() == QLatin1String("/")) {
            ipV6ConfigPath.clear();
        } else {
            ipV6ConfigPath = ip6ConfigObjectPathTmp.path();
        }
        ipV6Config = IpConfig();
        Q_EMIT q->ipV6ConfigChanged();
    } else if (property == QLatin1String("IpInterface")) {
#if !NM_CHECK_VERSION(0, 9, 10)
        // FIXME workaround, because NM doesn't Q_EMIT correct value
        ipInterface = deviceIface.ipInterface();
#else
        ipInterface = value.toString();
#endif
        Q_EMIT q->ipInterfaceChanged();
    } else if (property == QLatin1String("Managed")) {
        managed = value.toBool();
        Q_EMIT q->managedChanged();
    } else if (property == QLatin1String("State")) {
        connectionState = NetworkManager::DevicePrivate::convertState(value.toUInt());
        // FIXME NetworkManager 0.9.8 (maybe greater) doesn't
        // update ActiveConnection when disconnected
        // This is fixed in NM 73d128bbd17120225bb4986e3f05566f10fab581
        if (connectionState == NetworkManager::Device::Disconnected && activeConnection != QLatin1String("/")) {
            activeConnection = QLatin1Char('/');
            Q_EMIT q->activeConnectionChanged();
        }
        Q_EMIT q->connectionStateChanged();
    } else if (property == QLatin1String("StateReason")) { // just extracting the reason
        reason = NetworkManager::DevicePrivate::convertReason(qdbus_cast<DeviceDBusStateReason>(value).reason);
        Q_EMIT q->stateReasonChanged();
    } else if (property == QLatin1String("Udi")) {
        udi = value.toString();
        Q_EMIT q->udiChanged();
#if NM_CHECK_VERSION(0, 9, 10)
    } else if (property == QLatin1String("PhysicalPortId")) {
        physicalPortId = value.toString();
        Q_EMIT q->physicalPortIdChanged();
    } else if (property == QLatin1String("Mtu")) {
        mtu = value.toUInt();
        Q_EMIT q->mtuChanged();
#endif
    } else {
        qCWarning(NMQT) << Q_FUNC_INFO << "Unhandled property" << property;
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
    Q_FOREACH (const QString & availableConnection, d->availableConnections) {
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

#if NM_CHECK_VERSION(0, 9, 10)
QString NetworkManager::Device::physicalPortId() const
{
    Q_D(const Device);
    return d->physicalPortId;
}
#endif

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
             || d->connectionState == NetworkManager::Device::Failed);
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

#if NM_CHECK_VERSION(0, 9, 10)
uint NetworkManager::Device::mtu() const
{
    Q_D(const Device);
    return d->mtu;
}
#endif

QDBusPendingReply<> NetworkManager::Device::disconnectInterface()
{
    Q_D(Device);
    return d->deviceIface.Disconnect();
}

#if NM_CHECK_VERSION(1, 0, 0)
QDBusPendingReply<> NetworkManager::Device::deleteInterface()
{
    Q_D(Device);
    return d->deviceIface.Delete();
}
#endif

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

void NetworkManager::DevicePrivate::deviceStateChanged(uint newState, uint oldState, uint reason)
{
    Q_Q(Device);
    connectionState = NetworkManager::DevicePrivate::convertState(newState);
    reason = NetworkManager::DevicePrivate::convertReason(reason);

    Q_EMIT q->stateChanged(connectionState, NetworkManager::DevicePrivate::convertState(oldState), NetworkManager::DevicePrivate::convertReason(reason));
}

void NetworkManager::DevicePrivate::propertiesChanged(const QVariantMap &properties)
{
    // qCDebug(NMQT) << Q_FUNC_INFO << properties;

    QVariantMap::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        propertyChanged(it.key(), it.value());
        ++it;
    }

    // FIXME workaround, we need to get a path to updated IPv[46]Config,
    // because NM doesn't Q_EMIT the updated value when the device is activated
    // BUG: https://bugzilla.gnome.org/show_bug.cgi?id=725657
    if (properties.contains(QLatin1String("State")) && connectionState == NetworkManager::Device::Activated) {
        propertyChanged(QLatin1String("Ip4Config"), QVariant::fromValue<QDBusObjectPath>(deviceIface.ip4Config()));
        propertyChanged(QLatin1String("Ip6Config"), QVariant::fromValue<QDBusObjectPath>(deviceIface.ip6Config()));
    }
}

NetworkManager::Device::Type NetworkManager::Device::type() const
{
    Q_D(const Device);
    return d->deviceType;
}
