/*
Copyright 2008,2010,2011 Will Stephenson <wstephenson@kde.org>

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

#ifndef NM_NETWORKMANAGER_H
#define NM_NETWORKMANAGER_H

#include "QtNetworkManager-export.h"

#include <QtCore/QtGlobal>
#include <Qt/QtDBus>

#include "device.h"
//#include "notifier.h"

class ActiveConnection;

/**
 * This class allows querying the underlying system to discover the available
 * network interfaces and reachable networks. It has also the
 * responsibility to notify when a network interface appears or disappears.
 *
 * It is the unique entry point for network management. Applications should use
 * it to find network interfaces, or to be notified about network related changes.
 *
 * Note that it is implemented as a singleton
 */
namespace NetworkManager {
    enum Status {
        Unknown, /**< the networking system is not active or unable to report its status - proceed with caution */
        Asleep, /**< Networking is inactive and all devices are disabled. */
        Disconnected,/**< the system is not connected to any network */
        Disconnecting, /**< the system is breaking the connection */
        Connecting, /**< the system is not connected to any network */
        ConnectedLinkLocal, /**< A network device is connected, but there is only link-local connectivity. */
        ConnectedSiteOnly, /**< A network device is connected, but there is only site-local connectivity. */
        Connected /**< the system is currently connected to a network */
    };
    enum LogLevel {Error, Warning, Info, Debug};
    Q_FLAGS(LogDomains)
    enum LogDomain {NoChange, None, Hardware, RFKill, Ethernet, WiFi, Bluetooth, MobileBroadBand, DHCP4, DHCP6, PPP, WiFiScan, IPv4, IPv6, AutoIPv4, DNS, VPN, Sharing, Supplicant, UserSet, SysSet, Suspend, Core, Devices, OLPC};
    Q_DECLARE_FLAGS(LogDomains, LogDomain)
    class NMQT_EXPORT Notifier : public QObject
    {
    Q_OBJECT
    Q_SIGNALS:
    /**
     * This signal is emitted when the system's connection state changes
     */
    void statusChanged(NetworkManager::Status status);
    /**
     * This signal is emitted when a new network interface is available.
     *
     * @param uni the network interface identifier
     */
    void deviceAdded(const QString & uni);
    /**
     * This signal is emitted when a network interface is not available anymore.
     *
     * @param uni the network interface identifier
     */
    void deviceRemoved(const QString & uni);
    /**
     * This signal is emitted when the status of the wireless changed
     */
    void wirelessEnabledChanged(bool);
    /**
     * This signal is emitted when the status of the wireless changed
     */
    void wwanEnabledChanged(bool);
    /**
     * This signal is emitted when teh status of the wimax changed
     */
    void wimaxEnabledChanged(bool);
    /**
     * This signal is emitted when the status of the wireless changed
     */
    void wirelessHardwareEnabledChanged(bool);
    /**
     * This signal is emitted when the status of the wireless changed
     */
    void wwanHardwareEnabledChanged(bool);
    /**
     * This signal is emitted when the status of the wimax hardware changed
     */
    void wimaxHardwareEnabledChanged(bool);
    /**
     * This signal is emitted when the status of overall networking changed
     */
    void networkingEnabledChanged(bool);
    /**
     * This signal is emitted when the set of active connections changes
     */
     void activeConnectionsChanged();
     /**
      * This signal is emitted when the NetworkManager DBus service goes away
      */
     void serviceDisappeared();
     /**
      * This signal is emitted when the NetworkManager DBus service appears
      */
     void serviceAppeared();
    };

    /**
     * Get the NetworkManager version
     */
    NMQT_EXPORT QString version();
    /**
     * Get the manager connection state
     */
    NMQT_EXPORT NetworkManager::Status status();
    /**
     * Retrieves the list of all the network interfaces in the system.
     * It includes both hardware and virtual devices.
     *
     * @return the list of network interfaces available in this system
     */
    NMQT_EXPORT DeviceList networkInterfaces();
    /**
     * Find a new NetworkInterface object given its UNI.  This pointer is owned by the Solid
     * infrastructure.
     *
     * @param uni the identifier of the network interface to find
     * @return a valid NetworkInterface object if there's a device having the given UNI, an invalid one otherwise
     */
    NMQT_EXPORT Device * findNetworkInterface(const QString &uni);
    /**
     * Retrieves the status of networking (as a whole) in the system.
     * This is distinct from whether the system's networking is online or offline.
     * To check that, see @ref NetworkStatus.
     *
     * @return true if this networking is enabled, false otherwise
     */
    NMQT_EXPORT bool isNetworkingEnabled();
    /**
     * Retrieves the activation status of wireless networking in the system.
     *
     * @return true if this wireless networking is enabled, false otherwise
     */
    NMQT_EXPORT bool isWirelessEnabled();
    /**
     * Retrieves the status of wireless hardware in the system.  This is typically
     * controlled by a physical switch so there is no way to set this in software.
     *
     * @return true if this wireless networking is enabled, false otherwise
     */
    NMQT_EXPORT bool isWirelessHardwareEnabled();
    /**
     * Retrieves the status of wireless broadband (Wireless WAN) in the system.e.
     *
     * @return true if this type of wireless networking is enabled, false otherwise
     */
    NMQT_EXPORT bool isWwanEnabled();
    /**
     * Retrieves the status of wireless broadband (Wireless WAN) hardware in the system.  This is typically
     * controlled by a physical switch so there is no way to set this in software.
     *
     * @return true if this type of wireless hardware is enabled, false otherwise
     */
    NMQT_EXPORT bool isWwanHardwareEnabled();
    /**
     * Retrieves the activation status of wimax networking in the system.
     *
     * @return true if this wimax networking is enabled, false otherwise
     */
    NMQT_EXPORT bool isWimaxEnabled();
    /**
     * Retrieves the status of wimax hardware in the system.  This is typically
     * controlled by a physical switch so there is no way to set this in software.
     *
     * @return true if this wireless networking is enabled, false otherwise
     */
    NMQT_EXPORT bool isWimaxHardwareEnabled();
    /**
     * @param interfaceUni unique identifier of the network interface to be activated
     * @param connectionUni unique identifier for the connection to be activated
     * @param connectionParameter can be used to specify extra parameters not specific to the NetworkInterface or the connection, eg which AP to use when several present with same ESSID in range (because ESSID does not guarantee that the AP is part of the network you want to join!)
     */
    NMQT_EXPORT void activateConnection(const QString & interfaceUni, const QString & connectionUni, const QString & connectionParameter);
    /**
     * @param interfaceUni unique identifier of the network interface to be activated
     * @param connection connection to be added and activated
     * @param connectionParameter can be used to specify extra parameters not specific to the NetworkInterface or the connection, eg which AP to use when several present with same ESSID in range (because ESSID does not guarantee that the AP is part of the network you want to join!)
     */
    NMQT_EXPORT void addAndActivateConnection(const QString & interfaceUni, const QVariantMapMap & connection, const QString & connectionParameter);
    /**
     * Deactivate this network interface, if active
     * @param activeConnection identifer of the connection to deactivate
     */
    NMQT_EXPORT void deactivateConnection(const QString & activeConnection);
    /**
     * Access the list of any active connections
     *
     * @return a list of valid ActiveConnection objects
     */
    NMQT_EXPORT QList<ActiveConnection*> activeConnections();
    /**
     * Access the list of any active connections paths
     *
     * @return a list of valid ActiveConnection paths
     */
    NMQT_EXPORT QStringList activeConnectionsPaths();
    /**
     * find an ActiveConnection object for an active connection id
     *
     * @param uni the id of the ActiveConnection
     * @return a valid ActiveConnection object
     */
    NMQT_EXPORT ActiveConnection* findActiveConnection(const QString &uni);
    /**
     * Retrieves the interface types supported by this network manager.
     *
     * @return the interface types supported by the network manager
     */
    NMQT_EXPORT Device::Types supportedInterfaceTypes();
    NMQT_EXPORT void setNetworkingEnabled(bool enabled);
    // implement in Notifier
    NMQT_EXPORT void setWirelessEnabled(bool enabled);
    NMQT_EXPORT void setWwanEnabled(bool enabled);
    NMQT_EXPORT void setWimaxEnabled(bool enabled);
    NMQT_EXPORT void sleep(bool sleep);
    NMQT_EXPORT void setLogging(LogLevel, LogDomains);
    NMQT_EXPORT QStringMap permissions();
    NMQT_EXPORT Notifier * notifier();

}

#endif

