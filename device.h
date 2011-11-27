/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>

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

#ifndef NMQT_DEVICE_H
#define NMQT_DEVICE_H

#include <QObject>
#include <QtCore/QtGlobal>

#include "nmqt-export.h"

#include "generic-types.h"
#include "ipv4config.h"
#include "ipv6config.h"

namespace NetworkManager {
class DevicePrivate;
class ActiveConnection;

class NMQT_EXPORT Device : public QObject
{
    Q_OBJECT
    Q_ENUMS(ConnectionState Capability Type)
    Q_FLAGS(Capabilities)
    Q_FLAGS(Types)
    Q_DECLARE_PRIVATE(Device)

    Q_PROPERTY(QString uni READ uni WRITE setUni)
    Q_PROPERTY(QString interfaceName READ interfaceName WRITE setInterfaceName)
    Q_PROPERTY(QString ipInterfaceName READ ipInterfaceName)
    Q_PROPERTY(QString driver READ driver WRITE setDriver)
    Q_PROPERTY(QVariant genericCapabilities READ capabilitiesV WRITE setCapabilitiesV)
    Q_PROPERTY(int ipV4Address READ ipV4Address)
    Q_PROPERTY(bool managed READ managed WRITE setManaged)
    Q_PROPERTY(QString udi READ udi)
    Q_PROPERTY(bool firmwareMissing READ firmwareMissing)

    //Q_PROPERTY(Solid::Control::IPv4Config ipV4Config READ ipV4Config WRITE setIpV4Config)
    Q_PROPERTY(State State READ state)
    Q_FLAGS(Capabilities)

public:
    /**
     * Device connection states describe the possible states of a
     * network connection from the user's point of view.  For
     * simplicity, states from several different layers are present -
     * this is a high level view
     */
    enum State{ UnknownState = 0, Unmanaged = 10, Unavailable = 20, Disconnected = 30 , Preparing = 40,
                          ConfiguringHardware = 50 , NeedAuth = 60, ConfiguringIp = 70, CheckingIp = 80, WaitingForSecondaries = 90, Activated = 100, Deactivating = 110, Failed = 120};

    /**
     * Enums describing the reason for a connection state change
     */
    enum StateChangeReason{ UnknownReason=0, NoReason=1 , NowManagedReason=2, NowUnmanagedReason=3,
                                    ConfigFailedReason=4, ConfigUnavailableReason=5,
                                    ConfigExpiredReason=6, NoSecretsReason=7, AuthSupplicantDisconnectReason=8,
                                    AuthSupplicantConfigFailedReason=9, AuthSupplicantFailedReason=10,
                                    AuthSupplicantTimeoutReason=11, PppStartFailedReason=12, PppDisconnectReason=13,
                                    PppFailedReason=14, DhcpStartFailedReason=15, DhcpErrorReason=16, DhcpFailedReason=17,
                                    SharedStartFailedReason=18, SharedFailedReason=19,
                                    AutoIpStartFailedReason=20, AutoIpErrorReason=21, AutoIpFailedReason=22,
                                    ModemBusyReason=23, ModemNoDialToneReason=24, ModemNoCarrierReason=25, ModemDialTimeoutReason=26,
                                    ModemDialFailedReason=27, ModemInitFailedReason=28,
                                    GsmApnSelectFailedReason=29, GsmNotSearchingReason=30, GsmRegistrationDeniedReason=31,
                                    GsmRegistrationTimeoutReason=32, GsmRegistrationFailedReason=33,
                                    GsmPinCheckFailedReason=34, FirmwareMissingReason=35, DeviceRemovedReason=36,
                                    SleepingReason=37, ConnectionRemovedReason=38, UserRequestedReason=39, CarrierReason=40,
                                    ConnectionAssumedReason=41, SupplicantAvailableReason=42, ModemNotFoundReason=43, BluetoothFailedReason=44,
                                    Reserved = 65536 };
    /**
     * Possible Device capabilities
     * - IsManageable: denotes that the device can be controlled by this API
     * - SupportsCarrierDetect: the device informs us when it is plugged in to the medium
     * - SupportsWirelessScan: the device can scan for wireless networks
     */
    enum Capability { IsManageable = 0x1, SupportsCarrierDetect = 0x2 };

    /**
     * Device medium types
     * - Ethernet: Ieee8023 wired ethernet
     * - Wifi:  the Ieee80211 family of wireless networks
     * - Bluetooth: network bluetooth device (usually a cell phone)
     * - Modem: POTS, GSM, CDMA or LTE modems
     * - OlpcMesh: OLPC Mesh networking device
     * - Wimax: WiMax WWAN technology
     */
    enum Type { UnknownType = 0x0, Ethernet = 0x1, Wifi = 0x2, Unused1 = 0x3, Unused2 = 0x4, Bluetooth = 0x5, OlpcMesh = 0x6, Wimax = 0x7, Modem = 0x8 };

    Q_DECLARE_FLAGS(Capabilities, Capability)
    Q_DECLARE_FLAGS(Types, Type)

    /**
     * Creates a new NetworkInterface object.
     *
     * @param backendObject the network object provided by the backend
     */
    explicit Device( const QString & path, QObject * parent );
    Device( DevicePrivate &dd, QObject * parent );
    /**
     * Destroys a NetworkInterface object.
     */
    virtual ~Device();
    /**
     * Retrieves the interface type.  This is a virtual function that will return the
     * proper type of all sub-classes.
     *
     * @returns the NetworkInterface::Type that corresponds to this device.
     */
    virtual Type type() const;
    /**
     * Retrieves the Unique Network Identifier (UNI) of the NetworkInterface.
     * This identifier is unique for each network and network interface in the system.
     *
     * @returns the Unique Network Identifier of the current network
     */
    QString uni() const;
    /**
     * The current active connection for this interface.
     *
     * @returns A valid ActiveConnection object or 0 if no active connection was found
     */
    ActiveConnection* activeConnection();
    /**
     * The system name for the network interface
     */
    QString interfaceName() const;
    /**
     * The name of the device's data interface when available. This property
     * may not refer to the actual data interface until the device has
     * successfully established a data connection, indicated by the device's
     * State becoming ACTIVATED.
     */
    QString ipInterfaceName() const;
    /**
     * Handle for the system driver controlling this network interface
     */
    QString driver() const;
    /**
     * Disconnects a device and prevents the device from automatically
     * activating further connections without user intervention.
     */
    void disconnectInterface();
    int ipV4Address() const;
    /**
     * Get the current IPv4 configuration of this device.
     * Only valid when device is Activated.
     */
    IPv4Config ipV4Config() const;
    /**
     * Get the current IPv6 configuration of this device.
     * Only valid when device is Activated.
     */
    IPv6Config ipV6Config() const;
    // willtodo: implement Dhcp4,6Config
    /**
     * Retrieves the activation status of this network interface.
     *
     * @return true if this network interface is active, false otherwise
     */
    bool isActive() const;
    /**
     * Retrieves the current state of the device.
     * This is a high level view of the device. It is user oriented, so
     * actually it provides state coming from different layers.
     *
     * @return the current connection state
     * @see Device::State
     */
    State state() const;
    /**
     * Retrieves the maximum speed as reported by the device.
     * Note that this is only a design related piece of information, and that
     * the device might not reach this maximum.
     *
     * @return the device's maximum speed
     */
    int designSpeed() const;
    /**
     * Retrieves the capabilities supported by this device.
     *
     * @return the capabilities of the device
     */
    Capabilities capabilities() const;
    QVariant capabilitiesV() const;
    /**
     * Is the device currently being managed by NetworkManager?
     */
    bool managed() const;
    /**
     * Is the firmware needed by the device missing?
     */
    bool firmwareMissing() const;
    // willtodo: check if this can be removed/merged with uni()
    QString udi() const;
    //willtodo: remove
    void setUni(const QVariant&);
    void setInterfaceName(const QVariant&);
    void setDriver(const QVariant&);
    void setConnectionState(const QVariant&);
    void setCapabilities(const QVariant&);
    void setCapabilitiesV(const QVariant&);
    void setManaged(const QVariant&);

Q_SIGNALS:
    /**
     * This signal is emitted when the IP settings of this network have changed.
     */
    void ipDetailsChanged();
    /**
     * This signal is emitted when the device's link status changed.
     *
     * @param newstate the new state of the connection
     * @param oldstate the previous state of the connection
     * @param reason the reason for the state change, if any.  ReasonNone where the backend
     * provides no reason.
     * @see Device::State
     * @see Device::StateChangeReason
     */
    void stateChanged(NetworkManager::Device::State newstate, NetworkManager::Device::State oldstate, NetworkManager::Device::StateChangeReason reason);
protected Q_SLOTS:
    void deviceStateChanged(uint,uint,uint);
private:
    void init();
protected:
    DevicePrivate * d_ptr;
};

typedef QList<Device*> DeviceList;

Q_DECLARE_OPERATORS_FOR_FLAGS(Device::Capabilities)
Q_DECLARE_OPERATORS_FOR_FLAGS(Device::Types)

}

#endif

