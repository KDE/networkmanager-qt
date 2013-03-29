/*
Copyright 2008,2010 Will Stephenson <wstephenson@kde.org>
Copyright 2011-2012 Lamarque Souza <lamarque@kde.org>

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

#include "dbus/nm-deviceinterface.h"
//#include "networkmanagerdefinitions.h"
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
    connect( &iface, SIGNAL(DeviceAdded(QDBusObjectPath)),
             this, SLOT(onDeviceAdded(QDBusObjectPath)));
    connect( &iface, SIGNAL(DeviceRemoved(QDBusObjectPath)),
             this, SLOT(onDeviceRemoved(QDBusObjectPath)));
    connect( &iface, SIGNAL(PropertiesChanged(QVariantMap)),
             this, SLOT(propertiesChanged(QVariantMap)));
    connect( &iface, SIGNAL(StateChanged(uint)),
             this, SLOT(stateChanged(uint)));
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
    qDBusRegisterMetaType<QStringMap>();
    nmState = iface.state();
    parseVersion(iface.version());
    m_isWirelessHardwareEnabled = iface.wirelessHardwareEnabled();
    m_isWirelessEnabled = iface.wirelessEnabled();
    m_isWwanEnabled = iface.wwanEnabled();
    m_isWwanHardwareEnabled = iface.wwanHardwareEnabled();
    m_isWimaxEnabled = iface.wimaxEnabled();
    m_isWimaxHardwareEnabled = iface.wimaxHardwareEnabled();
    m_isNetworkingEnabled = iface.networkingEnabled();

    QDBusReply< QList <QDBusObjectPath> > deviceList = iface.GetDevices();
    if (deviceList.isValid())
    {
        nmDebug() << "Device list";
        QList <QDBusObjectPath> devices = deviceList.value();
        foreach (const QDBusObjectPath &op, devices) {
            networkInterfaceMap.insert(op.path(), 0);
            nmDebug() << "  " << op.path();
        }
    }
    else
        nmDebug() << "Error getting device list: " << deviceList.error().name() << ": " << deviceList.error().message();

    nmDebug() << "Active connections:";
    QList <QDBusObjectPath> activeConnections = iface.activeConnections();
    foreach (const QDBusObjectPath &ac, activeConnections)
    {
        m_activeConnections.insert(ac.path(), 0);
        nmDebug() << "    " << ac.path();
    }
    emit activeConnectionsChanged();
}

NetworkManager::NetworkManagerPrivate::~NetworkManagerPrivate()
{

}

QString NetworkManager::NetworkManagerPrivate::version() const
{
    return iface.version();
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

NetworkManager::Device * NetworkManager::NetworkManagerPrivate::findRegisteredNetworkInterface(const QString &uni)
{
    NetworkManager::Device * networkInterface = 0;
    if (networkInterfaceMap.contains(uni) && networkInterfaceMap.value(uni) != 0) {
        networkInterface = networkInterfaceMap.value(uni);
    } else {
        networkInterface = createNetworkInterface(uni);
        if (networkInterface) {
            networkInterfaceMap.insert(uni, networkInterface);
        } else {
            return 0;
        }
    }
    return networkInterface;
}

NetworkManager::ActiveConnection * NetworkManager::NetworkManagerPrivate::findRegisteredActiveConnection(const QString &uni)
{
    NetworkManager::ActiveConnection * ac = 0;
    if (m_activeConnections.contains(uni) && m_activeConnections.value(uni) != 0) {
        ac = m_activeConnections.value(uni);
    } else {
        ac = new NetworkManager::VpnConnection(uni, this);
        m_activeConnections.insert(uni, ac);
    }
    return ac;
}

NetworkManager::Device *NetworkManager::NetworkManagerPrivate::createNetworkInterface(const QString &uni)
{
    //nmDebug();
    OrgFreedesktopNetworkManagerDeviceInterface devIface(NetworkManagerPrivate::DBUS_SERVICE, uni, QDBusConnection::systemBus());
    uint deviceType = devIface.deviceType();
    NetworkManager::Device * createdInterface = 0;
    switch ( deviceType ) {
        case NM_DEVICE_TYPE_ETHERNET:
            createdInterface = new NetworkManager::WiredDevice(uni, this);
            break;
        case NM_DEVICE_TYPE_WIFI:
            createdInterface = new NetworkManager::WirelessDevice(uni, this);
            break;
        case NM_DEVICE_TYPE_MODEM:
            createdInterface = new NetworkManager::ModemDevice(uni, this);
            break;
        case NM_DEVICE_TYPE_BT:
            createdInterface = new NetworkManager::BluetoothDevice(uni, this);
            break;
        case NM_DEVICE_TYPE_WIMAX:
            createdInterface = new NetworkManager::WimaxDevice(uni, this);
            break;
        case NM_DEVICE_TYPE_OLPC_MESH:
            createdInterface = new NetworkManager::OlpcMeshDevice(uni, this);
            break;
        case NM_DEVICE_TYPE_INFINIBAND:
            createdInterface = new NetworkManager::AdslDevice(uni, this);
            break;
        case NM_DEVICE_TYPE_BOND:
            createdInterface = new NetworkManager::BondDevice(uni, this);
            break;
        case NM_DEVICE_TYPE_VLAN:
            createdInterface = new NetworkManager::VlanDevice(uni, this);
            break;
        case NM_DEVICE_TYPE_ADSL:
            createdInterface = new NetworkManager::AdslDevice(uni, this);
            break;
        case NM_DEVICE_TYPE_BRIDGE:
            createdInterface = new NetworkManager::BridgeDevice(uni, this);
            break;
        default:
            if (uni != QLatin1String("any")) { // VPN connections use "any" as uni for the network interface.
                nmDebug() << "libQtNetworkManager: Can't create object of type " << deviceType << "for" << uni;
            }
            break;
    }

    return createdInterface;
}

NetworkManager::Status NetworkManager::NetworkManagerPrivate::status() const
{
    return convertNMState(nmState);
}

NetworkManager::DeviceList NetworkManager::NetworkManagerPrivate::networkInterfaces()
{
    DeviceList list;

    QMap<QString, Device *>::const_iterator i;
    for (i = networkInterfaceMap.constBegin(); i != networkInterfaceMap.constEnd(); ++i) {
        Device * networkInterface = findRegisteredNetworkInterface(i.key());
        if (networkInterface) {
            list.append(networkInterface);
        } else {
            qWarning() << "warning: null network Interface for" << i.key();
        }
    }

    return list;
}

NetworkManager::Device* NetworkManager::NetworkManagerPrivate::findDeviceByIpIface (const QString& iface)
{
    QMap<QString, Device *>::const_iterator i;
    for (i = networkInterfaceMap.constBegin(); i != networkInterfaceMap.constEnd(); ++i) {
        Device * networkInterface = findRegisteredNetworkInterface(i.key());
        if (networkInterface && networkInterface->interfaceName() == iface) {
            return networkInterface;
        }
    }

    return 0;
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

QDBusPendingReply<QDBusObjectPath, QDBusObjectPath> NetworkManager::NetworkManagerPrivate::addAndActivateConnection(const QVariantMapMap& connection, const QString & interfaceUni, const QString & connectionParameter)
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

QStringMap NetworkManager::NetworkManagerPrivate::permissions()
{
    return iface.GetPermissions();
}

void NetworkManager::NetworkManagerPrivate::onDeviceAdded(const QDBusObjectPath & objpath)
{
    nmDebug();
    if (!networkInterfaceMap.contains(objpath.path())) {
        networkInterfaceMap.insert(objpath.path(), 0);
    }
    emit deviceAdded(objpath.path());
}

void NetworkManager::NetworkManagerPrivate::onDeviceRemoved(const QDBusObjectPath & objpath)
{
    nmDebug();
    NetworkManager::Device * device = networkInterfaceMap.take(objpath.path());
    device->deleteLater();
    emit deviceRemoved(objpath.path());
}

void NetworkManager::NetworkManagerPrivate::stateChanged(uint state)
{
    if ( nmState != state ) {
        // set new state
        nmState = state;
        emit Notifier::statusChanged( convertNMState( state ) );
    }
}

void NetworkManager::NetworkManagerPrivate::propertiesChanged(const QVariantMap &properties)
{
    nmDebug() << properties.keys();
    QLatin1String activeConnKey("ActiveConnections"),
                  netEnabledKey("NetworkingEnabled"),
                  wifiHwKey("WirelessHardwareEnabled"),
                  wifiEnabledKey("WirelessEnabled"),
                  wwanHwKey("WwanHardwareEnabled"),
                  wwanEnabledKey("WwanEnabled"),
                  wimaxHwKey("WimaxHardwareEnabled"),
                  wimaxEnabledKey("WimaxEnabled");
    QVariantMap::const_iterator it = properties.find(activeConnKey);
    if ( it != properties.end()) {
        QList<QDBusObjectPath> activePaths = qdbus_cast< QList<QDBusObjectPath> >(*it);
        m_activeConnections.clear();
        if ( activePaths.count() ) {
            nmDebug() << activeConnKey;
        }
        QList<QString> knownConnections = m_activeConnections.keys();
        foreach (const QDBusObjectPath &ac, activePaths)
        {
            if (!m_activeConnections.contains(ac.path()))
                m_activeConnections.insert(ac.path(), 0);
            else
                knownConnections.removeOne(ac.path());
            nmDebug() << "  " << ac.path();
        }
        foreach (const QString &path, knownConnections)
        {
            NetworkManager::ActiveConnection *ac = m_activeConnections.take(path);
            if (ac)
                delete ac;
        }
        emit activeConnectionsChanged();
    }
    it = properties.find(wifiHwKey);
    if ( it != properties.end()) {
        m_isWirelessHardwareEnabled = it->toBool();
        nmDebug() << wifiHwKey << m_isWirelessHardwareEnabled;
        emit wirelessHardwareEnabledChanged(m_isWirelessHardwareEnabled);
    }
    it = properties.find(wifiEnabledKey);
    if ( it != properties.end()) {
        m_isWirelessEnabled = it->toBool();
        nmDebug() << wifiEnabledKey << m_isWirelessEnabled;
        emit wirelessEnabledChanged(m_isWirelessEnabled);
    }
    it = properties.find(wwanHwKey);
    if ( it != properties.end()) {
        m_isWwanHardwareEnabled = it->toBool();
        nmDebug() << wwanHwKey << m_isWwanHardwareEnabled;
        emit wwanHardwareEnabledChanged(m_isWwanHardwareEnabled);
    }
    it = properties.find(wwanEnabledKey);
    if ( it != properties.end()) {
        m_isWwanEnabled = it->toBool();
        nmDebug() << wwanEnabledKey << m_isWwanEnabled;
        emit wwanEnabledChanged(m_isWwanEnabled);
    }
    it = properties.find(wimaxHwKey);
    if ( it != properties.end()) {
        m_isWimaxHardwareEnabled = it->toBool();
        nmDebug() << wimaxHwKey << m_isWimaxHardwareEnabled;
        emit wimaxHardwareEnabledChanged(m_isWimaxHardwareEnabled);
    }
    it = properties.find(wimaxEnabledKey);
    if ( it != properties.end()) {
        m_isWimaxEnabled = it->toBool();
        nmDebug() << wimaxEnabledKey << m_isWimaxEnabled;
        emit wimaxEnabledChanged(m_isWimaxEnabled);
    }
    it = properties.find(netEnabledKey);
    if ( it != properties.end()) {
        m_isNetworkingEnabled = it->toBool();
        nmDebug() << netEnabledKey << m_isNetworkingEnabled;
        emit networkingEnabledChanged(m_isNetworkingEnabled);
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
    QMap<QString, Device *>::const_iterator i;
    for (i = networkInterfaceMap.constBegin(); i != networkInterfaceMap.constEnd(); ++i) {
        deviceRemoved(i.key());
        if (i.value()) {
            i.value()->deleteLater();
        }
    }
    networkInterfaceMap.clear();
    qDeleteAll(m_activeConnections);
    m_activeConnections.clear();
    emit activeConnectionsChanged();
    emit serviceDisappeared();
}

QList<NetworkManager::ActiveConnection*> NetworkManager::NetworkManagerPrivate::activeConnections()
{
    QList<NetworkManager::ActiveConnection*> list;
    QMap<QString, ActiveConnection*>::const_iterator i;
    for (i = m_activeConnections.constBegin(); i != m_activeConnections.constEnd(); ++i) {
        list.append(findRegisteredActiveConnection(i.key()));
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

QList<NetworkManager::ActiveConnection*> NetworkManager::activeConnections()
{
    return globalNetworkManager->activeConnections();
}

QStringList NetworkManager::activeConnectionsPaths()
{
    return globalNetworkManager->activeConnectionsPaths();
}

NetworkManager::ActiveConnection* NetworkManager::findActiveConnection(const QString &uni)
{
    return globalNetworkManager->findRegisteredActiveConnection(uni);
}

NetworkManager::DeviceList NetworkManager::networkInterfaces()
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

NetworkManager::Device * NetworkManager::findNetworkInterface(const QString & uni)
{
    return globalNetworkManager->findRegisteredNetworkInterface(uni);
}

NetworkManager::Device* NetworkManager::findDeviceByIpFace (const QString& iface)
{
    return globalNetworkManager->findDeviceByIpIface(iface);
}

QDBusPendingReply<QDBusObjectPath, QDBusObjectPath> NetworkManager::addAndActivateConnection(const QVariantMapMap & connection, const QString & interfaceUni, const QString & connectionParameter)
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

QStringMap NetworkManager::permissions()
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
