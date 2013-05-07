/*
Copyright 2008,2010 Will Stephenson <wstephenson@kde.org>
Copyright 2011-2013 Lamarque Souza <lamarque@kde.org>
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

#include "manager.h"
#include "manager_p.h"

#include "macros.h"

#include <NetworkManager.h>

#include "wireddevice.h"
#include "wirelessdevice.h"
#include "modemdevice.h"
#include "bluetoothdevice.h"
#include "wimaxdevice.h"
#include "olpcmeshdevice.h"
#include "adsldevice.h"
#include "infinibanddevice.h"
#include "vlandevice.h"
#include "bonddevice.h"
#include "bridgedevice.h"
#include "activeconnection.h"
#include "vpnconnection.h"

#include "nmdebug.h"

const QString NetworkManager::NetworkManagerPrivate::DBUS_SERVICE(QString::fromLatin1(NM_DBUS_SERVICE));
const QString NetworkManager::NetworkManagerPrivate::DBUS_DAEMON_PATH(QString::fromLatin1(NM_DBUS_PATH));
const QString NetworkManager::NetworkManagerPrivate::DBUS_SETTINGS_PATH(QString::fromLatin1(NM_DBUS_PATH_SETTINGS));

NM_GLOBAL_STATIC(NetworkManager::NetworkManagerPrivate, globalNetworkManager)

NetworkManager::NetworkManagerPrivate::NetworkManagerPrivate() : watcher(DBUS_SERVICE, QDBusConnection::systemBus(), QDBusServiceWatcher::WatchForOwnerChange, this),
    iface( NetworkManager::NetworkManagerPrivate::DBUS_SERVICE, NetworkManager::NetworkManagerPrivate::DBUS_DAEMON_PATH, QDBusConnection::systemBus())
{
    connect(&watcher, SIGNAL(serviceRegistered(QString)), SLOT(daemonRegistered()));
    connect(&watcher, SIGNAL(serviceUnregistered(QString)), SLOT(daemonUnregistered()));
    connect(&iface, SIGNAL(DeviceAdded(QDBusObjectPath)), SLOT(onDeviceAdded(QDBusObjectPath)));
    connect(&iface, SIGNAL(DeviceRemoved(QDBusObjectPath)), SLOT(onDeviceRemoved(QDBusObjectPath)));
    connect(&iface, SIGNAL(PropertiesChanged(QVariantMap)), SLOT(propertiesChanged(QVariantMap)));

    init();
}

void NetworkManager::NetworkManagerPrivate::parseVersion(const QString & version)
{
    QStringList sl = version.split('.');

    if (sl.size() > 2) {
        m_x = sl[0].toInt();
        m_y = sl[1].toInt();
        m_z = sl[2].toInt();
    } else {
        m_x = -1;
        m_y = -1;
        m_z = -1;
    }
}

void NetworkManager::NetworkManagerPrivate::init()
{
    qDBusRegisterMetaType<QList<QDBusObjectPath> >();
    qDBusRegisterMetaType<DeviceDBusStateReason>();
    qDBusRegisterMetaType<NMStringMap>();
    nmState = iface.state();
    m_version = iface.version();
    parseVersion(m_version);
    m_isWirelessHardwareEnabled = iface.wirelessHardwareEnabled();
    m_isWirelessEnabled = iface.wirelessEnabled();
    m_isWwanEnabled = iface.wwanEnabled();
    m_isWwanHardwareEnabled = iface.wwanHardwareEnabled();
    m_isWimaxEnabled = iface.wimaxEnabled();
    m_isWimaxHardwareEnabled = iface.wimaxHardwareEnabled();
    m_isNetworkingEnabled = iface.networkingEnabled();

    if (iface.isValid()) {
        QDBusReply< QList <QDBusObjectPath> > deviceList = iface.GetDevices();
        if (deviceList.isValid())
        {
            nmDebug() << "Device list";
            QList <QDBusObjectPath> devices = deviceList.value();
            foreach (const QDBusObjectPath &op, devices) {
                networkInterfaceMap.insert(op.path(), Device::Ptr());
                nmDebug() << "  " << op.path();
            }
        }
        else
            nmDebug() << "Error getting device list: " << deviceList.error().name() << ": " << deviceList.error().message();

        nmDebug() << "Active connections:";
        QList <QDBusObjectPath> activeConnections = iface.activeConnections();
        foreach (const QDBusObjectPath &ac, activeConnections)
        {
            m_activeConnections.insert(ac.path(), NetworkManager::ActiveConnection::Ptr());
            emit activeConnectionAdded(ac.path());
            nmDebug() << "    " << ac.path();
        }
        emit activeConnectionsChanged();
    }
}

NetworkManager::NetworkManagerPrivate::~NetworkManagerPrivate()
{

}

QString NetworkManager::NetworkManagerPrivate::version() const
{
    return m_version;
}

int NetworkManager::NetworkManagerPrivate::compareVersion(const QString & version)
{
    int x, y, z;

    QStringList sl = version.split('.');

    if (sl.size() > 2) {
        x = sl[0].toInt();
        y = sl[1].toInt();
        z = sl[2].toInt();
    } else {
        x = -1;
        y = -1;
        z = -1;
    }

    return compareVersion(x, y, z);
}

int NetworkManager::NetworkManagerPrivate::compareVersion(const int x, const int y, const int z) const
{
    if (m_x > x) {
        return 1;
    } else if (m_x < x) {
        return -1;
    } else if (m_y > y) {
        return 1;
    } else if (m_y < y) {
        return -1;
    } else if (m_z > z) {
        return 1;
    } else if (m_z < z) {
        return -1;
    }
    return 0;
}

NetworkManager::Device::Ptr NetworkManager::NetworkManagerPrivate::findRegisteredNetworkInterface(const QString &uni)
{
    NetworkManager::Device::Ptr networkInterface;
    if (networkInterfaceMap.contains(uni)) {
        if (networkInterfaceMap.value(uni)) {
            networkInterface = networkInterfaceMap.value(uni);
        } else {
            networkInterface = createNetworkInterface(uni);
            networkInterfaceMap[uni] = networkInterface;
        }
    }
    return networkInterface;
}

NetworkManager::ActiveConnection::Ptr NetworkManager::NetworkManagerPrivate::findRegisteredActiveConnection(const QString &uni)
{
    NetworkManager::ActiveConnection::Ptr activeConnection;
    if (!uni.isEmpty() && uni != QLatin1String("/")) {
        bool contains = m_activeConnections.contains(uni);
        if (contains && m_activeConnections.value(uni)) {
            activeConnection = m_activeConnections.value(uni);
        } else {
            activeConnection = NetworkManager::ActiveConnection::Ptr(new NetworkManager::VpnConnection(uni), &QObject::deleteLater);
            if (activeConnection->connection()) {
                m_activeConnections[uni] = activeConnection;
                if (!contains) {
                    emit activeConnectionAdded(uni);
                }
            } else {
                activeConnection.clear();
            }
        }
    }
    return activeConnection;
}

NetworkManager::Device::Ptr NetworkManager::NetworkManagerPrivate::createNetworkInterface(const QString &uni)
{
    //nmDebug();
    NetworkManager::Device::Ptr createdInterface;
    NetworkManager::Device device(uni);
    if (!device.isValid()) {
        qWarning() << Q_FUNC_INFO << "Failed to create device interface:" << uni;
        return createdInterface;
    }

    switch (device.type()) {
    case Device::Ethernet:
        createdInterface = Device::Ptr(new NetworkManager::WiredDevice(uni));
        break;
    case Device::Wifi:
        createdInterface = Device::Ptr(new NetworkManager::WirelessDevice(uni));
        break;
    case Device::Modem:
        createdInterface = Device::Ptr(new NetworkManager::ModemDevice(uni));
        break;
    case Device::Bluetooth:
        createdInterface = Device::Ptr(new NetworkManager::BluetoothDevice(uni));
        break;
    case Device::Wimax:
        createdInterface = Device::Ptr(new NetworkManager::WimaxDevice(uni));
        break;
    case Device::OlpcMesh:
        createdInterface = Device::Ptr(new NetworkManager::OlpcMeshDevice(uni));
        break;
    case Device::InfiniBand:
        createdInterface = Device::Ptr(new NetworkManager::InfinibandDevice(uni));
        break;
    case Device::Bond:
        createdInterface = Device::Ptr(new NetworkManager::BondDevice(uni));
        break;
    case Device::Vlan:
        createdInterface = Device::Ptr(new NetworkManager::VlanDevice(uni));
        break;
    case Device::Adsl:
        createdInterface = Device::Ptr(new NetworkManager::AdslDevice(uni));
        break;
    case Device::Bridge:
        createdInterface = Device::Ptr(new NetworkManager::BridgeDevice(uni));
        break;
    default:
        if (uni != QLatin1String("any")) { // VPN connections use "any" as uni for the network interface.
            nmDebug() << "libNetworkManagerQt: Can't create object of type " << device.type() << "for" << uni << device.isValid();
        }
        break;
    }

    return createdInterface;
}

NetworkManager::Status NetworkManager::NetworkManagerPrivate::status() const
{
    return convertNMState(nmState);
}

NetworkManager::Device::List NetworkManager::NetworkManagerPrivate::networkInterfaces()
{
    Device::List list;

    QMap<QString, Device::Ptr>::const_iterator i;
    for (i = networkInterfaceMap.constBegin(); i != networkInterfaceMap.constEnd(); ++i) {
        Device::Ptr networkInterface = findRegisteredNetworkInterface(i.key());
        if (!networkInterface.isNull()) {
            list.append(networkInterface);
        } else {
            qWarning() << "warning: null network Interface for" << i.key();
        }
    }

    return list;
}

NetworkManager::Device::Ptr NetworkManager::NetworkManagerPrivate::findDeviceByIpIface (const QString& iface)
{
    QMap<QString, Device::Ptr>::const_iterator i;
    for (i = networkInterfaceMap.constBegin(); i != networkInterfaceMap.constEnd(); ++i) {
        Device::Ptr networkInterface = findRegisteredNetworkInterface(i.key());
        if (networkInterface && networkInterface->udi() == iface) {
            return networkInterface;
        }
    }

    return Device::Ptr();
}

bool NetworkManager::NetworkManagerPrivate::isNetworkingEnabled() const
{
    return m_isNetworkingEnabled;
}

bool NetworkManager::NetworkManagerPrivate::isWirelessEnabled() const
{
    return m_isWirelessEnabled;
}

bool NetworkManager::NetworkManagerPrivate::isWirelessHardwareEnabled() const
{
    return m_isWirelessHardwareEnabled;
}

bool NetworkManager::NetworkManagerPrivate::isWwanEnabled() const
{
    return m_isWwanEnabled;
}

bool NetworkManager::NetworkManagerPrivate::isWwanHardwareEnabled() const
{
    return m_isWwanHardwareEnabled;
}

bool NetworkManager::NetworkManagerPrivate::isWimaxEnabled() const
{
    return m_isWimaxEnabled;
}

bool NetworkManager::NetworkManagerPrivate::isWimaxHardwareEnabled() const
{
    return m_isWimaxHardwareEnabled;
}

QDBusPendingReply<QDBusObjectPath> NetworkManager::NetworkManagerPrivate::activateConnection(const QString & connectionUni, const QString & interfaceUni, const QString & connectionParameter)
{
    QString extra_connection_parameter = connectionParameter;
    QString extra_interface_parameter = interfaceUni;
    if (extra_connection_parameter.isEmpty()) {
        extra_connection_parameter = QLatin1String("/");
    }
    if (extra_interface_parameter.isEmpty()) {
        extra_interface_parameter = QLatin1String("/");
    }
    // TODO store error code
    QDBusObjectPath connPath(connectionUni);
    QDBusObjectPath interfacePath(interfaceUni);
    nmDebug() << "Activating connection" << connPath.path() << "on interface" << interfacePath.path() << "with extra" << extra_connection_parameter;
    return iface.ActivateConnection(connPath, QDBusObjectPath(extra_interface_parameter), QDBusObjectPath(extra_connection_parameter));
}

QDBusPendingReply<QDBusObjectPath, QDBusObjectPath> NetworkManager::NetworkManagerPrivate::addAndActivateConnection(const NMVariantMapMap& connection, const QString & interfaceUni, const QString & connectionParameter)
{
    QString extra_connection_parameter = connectionParameter;
    if (extra_connection_parameter.isEmpty()) {
        extra_connection_parameter = QLatin1String("/");
    }
    // TODO store error code
    QDBusObjectPath interfacePath(interfaceUni);
    return iface.AddAndActivateConnection(connection, interfacePath, QDBusObjectPath(extra_connection_parameter));
}

void NetworkManager::NetworkManagerPrivate::deactivateConnection( const QString & activeConnectionPath )
{
    iface.DeactivateConnection(QDBusObjectPath(activeConnectionPath));
}

void NetworkManager::NetworkManagerPrivate::setNetworkingEnabled(bool enabled)
{
    QDBusPendingReply<> reply = iface.Enable(enabled);
}

void NetworkManager::NetworkManagerPrivate::setWirelessEnabled(bool enabled)
{
    iface.setWirelessEnabled(enabled);
}

void NetworkManager::NetworkManagerPrivate::setWwanEnabled(bool enabled)
{
    iface.setWwanEnabled(enabled);
}

void NetworkManager::NetworkManagerPrivate::setWimaxEnabled(bool enabled)
{
    iface.setWimaxEnabled(enabled);
}

void NetworkManager::NetworkManagerPrivate::sleep(bool sleep)
{
    iface.Sleep(sleep);
}

void NetworkManager::NetworkManagerPrivate::setLogging(NetworkManager::LogLevel level, NetworkManager::LogDomains domains)
{
    QString logLevel;
    QStringList logDomains;
    switch (level)
    {
        case NetworkManager::Error:
            logLevel = QLatin1String("ERR");
            break;
        case NetworkManager::Warning:
            logLevel = QLatin1String("WARN");
            break;
        case NetworkManager::Info:
            logLevel = QLatin1String("INFO");
            break;
        case NetworkManager::Debug:
            logLevel = QLatin1String("DEBUG");
            break;
    }
    if (!domains.testFlag(NoChange))
    {
        if (domains.testFlag(NetworkManager::None))
            logDomains << QLatin1String("NONE");
        if (domains.testFlag(NetworkManager::Hardware))
            logDomains << QLatin1String("HW");
        if (domains.testFlag(NetworkManager::RFKill))
            logDomains << QLatin1String("RFKILL");
        if (domains.testFlag(NetworkManager::Ethernet))
            logDomains << QLatin1String("ETHER");
        if (domains.testFlag(NetworkManager::WiFi))
            logDomains << QLatin1String("WIFI");
        if (domains.testFlag(NetworkManager::Bluetooth))
            logDomains << QLatin1String("BT");
        if (domains.testFlag(NetworkManager::MobileBroadBand))
            logDomains << QLatin1String("MB");
        if (domains.testFlag(NetworkManager::DHCP4))
            logDomains << QLatin1String("DHCP4");
        if (domains.testFlag(NetworkManager::DHCP6))
            logDomains << QLatin1String("DHCP6");
        if (domains.testFlag(NetworkManager::PPP))
            logDomains << QLatin1String("PPP");
        if (domains.testFlag(NetworkManager::WiFiScan))
            logDomains << QLatin1String("WIFI_SCAN");
        if (domains.testFlag(NetworkManager::IPv4))
            logDomains << QLatin1String("IP4");
        if (domains.testFlag(NetworkManager::IPv6))
            logDomains << QLatin1String("IP6");
        if (domains.testFlag(NetworkManager::AutoIPv4))
            logDomains << QLatin1String("AUTOIP4");
        if (domains.testFlag(NetworkManager::DNS))
            logDomains << QLatin1String("DNS");
        if (domains.testFlag(NetworkManager::VPN))
            logDomains << QLatin1String("VPN");
        if (domains.testFlag(NetworkManager::Sharing))
            logDomains << QLatin1String("SHARING");
        if (domains.testFlag(NetworkManager::Supplicant))
            logDomains << QLatin1String("SUPPLICANT");
        if (domains.testFlag(NetworkManager::UserSet))
            logDomains << QLatin1String("USER_SET");
        if (domains.testFlag(NetworkManager::SysSet))
            logDomains << QLatin1String("SYS_SET");
        if (domains.testFlag(NetworkManager::Suspend))
            logDomains << QLatin1String("SUSPEND");
        if (domains.testFlag(NetworkManager::Core))
            logDomains << QLatin1String("CORE");
        if (domains.testFlag(NetworkManager::Devices))
            logDomains << QLatin1String("DEVICE");
        if (domains.testFlag(NetworkManager::OLPC))
            logDomains << QLatin1String("OLPC");
        if (domains.testFlag(NetworkManager::Wimax))
            logDomains << QLatin1String("WIMAX");
        if (domains.testFlag(NetworkManager::Infiniband))
            logDomains << QLatin1String("INFINIBAND");
        if (domains.testFlag(NetworkManager::Firewall))
            logDomains << QLatin1String("FIREWALL");
        if (domains.testFlag(NetworkManager::Adsl))
            logDomains << QLatin1String("ADSL");
        if (domains.testFlag(NetworkManager::Bond))
            logDomains << QLatin1String("BOND");
        if (domains.testFlag(NetworkManager::Vlan))
            logDomains << QLatin1String("VLAN");
    }
    iface.SetLogging(logLevel, logDomains.join(QLatin1String(",")));
}

NMStringMap NetworkManager::NetworkManagerPrivate::permissions()
{
    return iface.GetPermissions();
}

void NetworkManager::NetworkManagerPrivate::onDeviceAdded(const QDBusObjectPath &objpath)
{
    nmDebug();
    if (!networkInterfaceMap.contains(objpath.path())) {
        networkInterfaceMap.insert(objpath.path(), Device::Ptr());
    }
    emit deviceAdded(objpath.path());
}

void NetworkManager::NetworkManagerPrivate::onDeviceRemoved(const QDBusObjectPath & objpath)
{
    nmDebug();
    networkInterfaceMap.remove(objpath.path());
    emit deviceRemoved(objpath.path());
}

void NetworkManager::NetworkManagerPrivate::stateChanged(uint state)
{
    if (nmState != state) {
        nmState = state;
        emit Notifier::statusChanged(convertNMState(nmState));
    }
}

void NetworkManager::NetworkManagerPrivate::propertiesChanged(const QVariantMap &changedProperties)
{
    QVariantMap::const_iterator it = changedProperties.constBegin();
    while (it != changedProperties.constEnd()) {
        QString property = it.key();
        if (property == QLatin1String("ActiveConnections")) {
            QList<QDBusObjectPath> activePaths = qdbus_cast< QList<QDBusObjectPath> >(*it);
            if (activePaths.isEmpty()) {
                foreach (const QString &path, m_activeConnections.keys()) {
                    emit activeConnectionRemoved(path);
                }
                m_activeConnections.clear();
            } else {
                QStringList knownConnections = m_activeConnections.keys();
                foreach (const QDBusObjectPath &ac, activePaths) {
                    if (!m_activeConnections.contains(ac.path())) {
                        m_activeConnections.insert(ac.path(), NetworkManager::ActiveConnection::Ptr());
                        emit activeConnectionAdded(ac.path());
                    } else {
                        knownConnections.removeOne(ac.path());
                    }
                    nmDebug() << "  " << ac.path();
                }
                foreach (const QString &path, knownConnections) {
                    m_activeConnections.remove(path);
                    emit activeConnectionRemoved(path);
                }
            }
            emit activeConnectionsChanged();
        } else if (property == QLatin1String("NetworkingEnabled")) {
            m_isNetworkingEnabled = it->toBool();
            nmDebug() << property << m_isNetworkingEnabled;
            emit networkingEnabledChanged(m_isNetworkingEnabled);
        } else if (property == QLatin1String("WirelessHardwareEnabled")) {
            m_isWirelessHardwareEnabled = it->toBool();
            nmDebug() << property << m_isWirelessHardwareEnabled;
            emit wirelessHardwareEnabledChanged(m_isWirelessHardwareEnabled);
        } else if (property == QLatin1String("WirelessEnabled")) {
            m_isWirelessEnabled = it->toBool();
            nmDebug() << property << m_isWirelessEnabled;
            emit wirelessEnabledChanged(m_isWirelessEnabled);
        } else if (property == QLatin1String("WwanHardwareEnabled")) {
            m_isWwanHardwareEnabled = it->toBool();
            nmDebug() << property << m_isWwanHardwareEnabled;
            emit wwanHardwareEnabledChanged(m_isWwanHardwareEnabled);
        } else if (property == QLatin1String("WwanEnabled")) {
            m_isWwanEnabled = it->toBool();
            nmDebug() << property << m_isWwanEnabled;
            emit wwanEnabledChanged(m_isWwanEnabled);
        } else if (property == QLatin1String("WimaxHardwareEnabled")) {
            m_isWimaxHardwareEnabled = it->toBool();
            nmDebug() << property << m_isWimaxHardwareEnabled;
            emit wimaxHardwareEnabledChanged(m_isWimaxHardwareEnabled);
        } else if (property == QLatin1String("WimaxEnabled")) {
            m_isWimaxEnabled = it->toBool();
            nmDebug() << property << m_isWimaxEnabled;
            emit wimaxEnabledChanged(m_isWimaxEnabled);
        } else if (property == QLatin1String("Version")) {
            m_version = it->toString();
            parseVersion(m_version);
        } else if (property == QLatin1String("State")) {
            stateChanged(it->toUInt());
        } else {
            qWarning() << Q_FUNC_INFO << "Unhandled property" << property;
        }
        ++it;
    }
}

NetworkManager::Status NetworkManager::NetworkManagerPrivate::convertNMState(uint state)
{
    NetworkManager::Status status = NetworkManager::Unknown;
    switch (state) {
        case NM_STATE_UNKNOWN:
            status = NetworkManager::Unknown;
            break;
        case NM_STATE_ASLEEP:
            status = NetworkManager::Asleep;
            break;
        case NM_STATE_DISCONNECTED:
            status = NetworkManager::Disconnected;
            break;
        case NM_STATE_DISCONNECTING:
            status = NetworkManager::Disconnecting;
            break;
        case NM_STATE_CONNECTING:
            status = NetworkManager::Connecting;
            break;
        case NM_STATE_CONNECTED_LOCAL:
            status = NetworkManager::ConnectedLinkLocal;
            break;
        case NM_STATE_CONNECTED_SITE:
            status = NetworkManager::ConnectedSiteOnly;
            break;
        case NM_STATE_CONNECTED_GLOBAL:
            status = NetworkManager::Connected;
            break;
    }
    return status;
}

void NetworkManager::NetworkManagerPrivate::daemonRegistered()
{
    init();
    emit serviceAppeared();
}

void NetworkManager::NetworkManagerPrivate::daemonUnregistered()
{
    stateChanged(NM_STATE_UNKNOWN);
    QMap<QString, Device::Ptr>::const_iterator i = networkInterfaceMap.constBegin();
    while (i != networkInterfaceMap.constEnd()) {
        emit deviceRemoved(i.key());
        ++i;
    }
    networkInterfaceMap.clear();

    foreach (const QString &path, m_activeConnections.keys()) {
        emit activeConnectionRemoved(path);
    }
    m_activeConnections.clear();
    emit activeConnectionsChanged();
    emit serviceDisappeared();
}

NetworkManager::ActiveConnection::List NetworkManager::NetworkManagerPrivate::activeConnections()
{
    NetworkManager::ActiveConnection::List list;
    foreach (const QString &uni, m_activeConnections.keys()) {
        NetworkManager::ActiveConnection::Ptr activeConnection = findRegisteredActiveConnection(uni);
        if (activeConnection) {
            list << activeConnection;
        }
    }
    return list;
}

QStringList NetworkManager::NetworkManagerPrivate::activeConnectionsPaths() const
{
    return m_activeConnections.keys();
}

QDBusPendingReply< QString, QString > NetworkManager::NetworkManagerPrivate::getLogging()
{
    return iface.GetLogging();
}

QString NetworkManager::version()
{
    return globalNetworkManager->version();
}

int NetworkManager::compareVersion(const QString & version)
{
    return globalNetworkManager->compareVersion(version);
}

int NetworkManager::compareVersion(const int x, const int y, const int z)
{
    return globalNetworkManager->compareVersion(x, y, z);
}

NetworkManager::Status NetworkManager::status()
{
    return globalNetworkManager->status();
}

NetworkManager::ActiveConnection::List NetworkManager::activeConnections()
{
    return globalNetworkManager->activeConnections();
}

QStringList NetworkManager::activeConnectionsPaths()
{
    return globalNetworkManager->activeConnectionsPaths();
}

NetworkManager::ActiveConnection::Ptr NetworkManager::findActiveConnection(const QString &uni)
{
    return globalNetworkManager->findRegisteredActiveConnection(uni);
}

NetworkManager::Device::List NetworkManager::networkInterfaces()
{
    return globalNetworkManager->networkInterfaces();
}

bool NetworkManager::isNetworkingEnabled()
{
    return globalNetworkManager->isNetworkingEnabled();
}

bool NetworkManager::isWirelessEnabled()
{
    return globalNetworkManager->isWirelessEnabled();
}

bool NetworkManager::isWirelessHardwareEnabled()
{
    return globalNetworkManager->isWirelessHardwareEnabled();
}

NetworkManager::Device::Ptr NetworkManager::findNetworkInterface(const QString &uni)
{
    return globalNetworkManager->findRegisteredNetworkInterface(uni);
}

NetworkManager::Device::Ptr NetworkManager::findDeviceByIpFace (const QString &iface)
{
    return globalNetworkManager->findDeviceByIpIface(iface);
}

QDBusPendingReply<QDBusObjectPath, QDBusObjectPath> NetworkManager::addAndActivateConnection(const NMVariantMapMap & connection, const QString & interfaceUni, const QString & connectionParameter)
{
    return globalNetworkManager->addAndActivateConnection(connection, interfaceUni, connectionParameter);
}

QDBusPendingReply<QDBusObjectPath> NetworkManager::activateConnection(const QString & connectionUni, const QString & interfaceUni, const QString & connectionParameter)
{
    return globalNetworkManager->activateConnection(connectionUni, interfaceUni, connectionParameter);
}

void NetworkManager::deactivateConnection( const QString & activeConnectionPath )
{
    globalNetworkManager->deactivateConnection(activeConnectionPath);
}

QDBusPendingReply< QString, QString > NetworkManager::getLogging()
{
    globalNetworkManager->getLogging();
}

void NetworkManager::setNetworkingEnabled(bool enabled)
{
    globalNetworkManager->setNetworkingEnabled(enabled);
}

void NetworkManager::setWirelessEnabled(bool enabled)
{
    globalNetworkManager->setWirelessEnabled(enabled);
}

bool NetworkManager::isWwanEnabled()
{
    return globalNetworkManager->isWwanEnabled();
}

bool NetworkManager::isWwanHardwareEnabled()
{
    return globalNetworkManager->isWwanHardwareEnabled();
}

void NetworkManager::setWwanEnabled(bool enabled)
{
    globalNetworkManager->setWwanEnabled(enabled);
}

bool NetworkManager::isWimaxEnabled()
{
    return globalNetworkManager->isWimaxEnabled();
}

bool NetworkManager::isWimaxHardwareEnabled()
{
    return globalNetworkManager->isWimaxHardwareEnabled();
}

void NetworkManager::setWimaxEnabled(bool enabled)
{
    globalNetworkManager->setWimaxEnabled(enabled);
}

void NetworkManager::sleep(bool sleep)
{
    globalNetworkManager->sleep(sleep);
}

void NetworkManager::setLogging(NetworkManager::LogLevel level, NetworkManager::LogDomains domains)
{
    globalNetworkManager->setLogging(level, domains);
}

NMStringMap NetworkManager::permissions()
{
    return globalNetworkManager->permissions();
}

NetworkManager::Device::Types NetworkManager::supportedInterfaceTypes()
{
    return (NetworkManager::Device::Types) (
           NetworkManager::Device::Ethernet |
           NetworkManager::Device::Wifi |
           NetworkManager::Device::Modem |
           NetworkManager::Device::Wimax |
           NetworkManager::Device::Bluetooth |
           NetworkManager::Device::OlpcMesh |
           NetworkManager::Device::InfiniBand |
           NetworkManager::Device::Bond |
           NetworkManager::Device::Vlan |
           NetworkManager::Device::Adsl |
           NetworkManager::Device::Bridge
           );
}

NetworkManager::Notifier * NetworkManager::notifier()
{
    return globalNetworkManager;
}

#include "manager.moc"
