/*
    SPDX-FileCopyrightText: 2008, 2011 Will Stephenson <wstephenson@kde.org>
    SPDX-FileCopyrightText: 2011-2013 Lamarque V. Souza <lamarque@kde.org>
    SPDX-FileCopyrightText: 2013 Daniel Nicoletti <dantti12@gmail.com>
    SPDX-FileCopyrightText: 2013 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_DEVICE_H
#define NETWORKMANAGERQT_DEVICE_H

#include <QObject>
#include <QSharedPointer>

#include "activeconnection.h"
#include "devicestatistics.h"
#include "dhcp4config.h"
#include "dhcp6config.h"
#include "generictypes.h"
#include "ipconfig.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

namespace NetworkManager
{
class DevicePrivate;
class DeviceStateReason;
class DeviceStateReasonPrivate;

/*!
 * \class NetworkManager::Device
 * \inheaderfile NetworkManagerQt/Device
 * \inmodule NetworkManagerQt
 *
 * \brief This class represents a common device interface.
 */
class NETWORKMANAGERQT_EXPORT Device : public QObject
{
    Q_OBJECT

    /*!
     * \property NetworkManager::Device::uni
     */
    Q_PROPERTY(QString uni READ uni)

    /*!
     * \property NetworkManager::Device::interfaceName
     */
    Q_PROPERTY(QString interfaceName READ interfaceName)

    /*!
     * \property NetworkManager::Device::ipInterfaceName
     */
    Q_PROPERTY(QString ipInterfaceName READ ipInterfaceName)

    /*!
     * \property NetworkManager::Device::driver
     */
    Q_PROPERTY(QString driver READ driver)

    /*!
     * \property NetworkManager::Device::driverVersion
     */
    Q_PROPERTY(QString driverVersion READ driverVersion)

    /*!
     * \property NetworkManager::Device::firmwareVersion
     */
    Q_PROPERTY(QString firmwareVersion READ firmwareVersion)

    /*!
     * \property NetworkManager::Device::genericCapabilities
     */
    Q_PROPERTY(QVariant genericCapabilities READ capabilitiesV)

    /*!
     * \property NetworkManager::Device::ipV4Address
     */
    Q_PROPERTY(QHostAddress ipV4Address READ ipV4Address)

    /*!
     * \property NetworkManager::Device::managed
     */
    Q_PROPERTY(bool managed READ managed WRITE setManaged)

    /*!
     * \property NetworkManager::Device::mtu
     */
    Q_PROPERTY(uint mtu READ mtu)

    /*!
     * \property NetworkManager::Device::InterfaceFlags
     */
    Q_PROPERTY(Interfaceflags InterfaceFlags READ interfaceFlags)

    /*!
     * \property NetworkManager::Device::nmPluginMissing
     */
    Q_PROPERTY(bool nmPluginMissing READ nmPluginMissing)

    /*!
     * \property NetworkManager::Device::metered
     */
    Q_PROPERTY(MeteredStatus metered READ metered)

    /*!
     * \property NetworkManager::Device::udi
     */
    Q_PROPERTY(QString udi READ udi)

    /*!
     * \property NetworkManager::Device::firmwareMissing
     */
    Q_PROPERTY(bool firmwareMissing READ firmwareMissing)

    /*!
     * \property NetworkManager::Device::autoconnect
     */
    Q_PROPERTY(bool autoconnect READ autoconnect WRITE setAutoconnect)

    /*!
     * \property NetworkManager::Device::stateReason
     */
    Q_PROPERTY(DeviceStateReason stateReason READ stateReason)

    /*!
     * \property NetworkManager::Device::state
     */
    Q_PROPERTY(State state READ state)

    /*!
     * \property NetworkManager::Device::deviceStatistics
     */
    Q_PROPERTY(NetworkManager::DeviceStatistics::Ptr deviceStatistics READ deviceStatistics)

public:
    /*!
     * \typedef NetworkManager::Device::Ptr
     */
    typedef QSharedPointer<Device> Ptr;
    /*!
     * \typedef NetworkManager::Device::List
     */
    typedef QList<Ptr> List;
    /*!
     *
     * Device connection states describe the possible states of a
     * network connection from the user's point of view.  For
     * simplicity, states from several different layers are present -
     * this is a high level view
     *
     * \value UnknownState
     *        The device is in an unknown state
     * \value Unmanaged
     *        The device is recognized but not managed by NetworkManager
     * \value Unavailable
     *        The device cannot be used (carrier off, rfkill, etc)
     * \value Disconnected
     *        The device is not connected
     * \value Preparing
     *        The device is preparing to connect
     * \value ConfiguringHardware
     *        The device is being configured
     * \value NeedAuth
     *        The device is awaiting secrets necessary to continue connection
     * \value ConfiguringIp
     *        The IP settings of the device are being requested and configured
     * \value CheckingIp
     *        The device's IP connectivity ability is being determined
     * \value WaitingForSecondaries
     *        The device is waiting for secondary connections to be activated
     * \value Activated
     *        The device is active
     * \value Deactivating
     *        The device's network connection is being torn down
     * \value Failed
     *        The device is in a failure state following an attempt to activate it
     */
    enum State {
        UnknownState = 0,
        Unmanaged = 10,
        Unavailable = 20,
        Disconnected = 30,
        Preparing = 40,
        ConfiguringHardware = 50,
        NeedAuth = 60,
        ConfiguringIp = 70,
        CheckingIp = 80,
        WaitingForSecondaries = 90,
        Activated = 100,
        Deactivating = 110,
        Failed = 120,
    };
    Q_ENUM(State)

    /*!
     *
     * Enums describing the reason for a connection state change
     * \note StateChangeReasons NewActivation, ParentChanged, ParentManagedChanged are available in runtime NM >= 1.0.4
     *
     * \value UnknownReason
     * \value NoReason
     * \value NowManagedReason
     * \value NowUnmanagedReason
     * \value ConfigFailedReason
     * \value ConfigUnavailableReason
     * \value ConfigExpiredReason
     * \value NoSecretsReason
     * \value AuthSupplicantDisconnectReason
     * \value AuthSupplicantConfigFailedReason
     * \value AuthSupplicantFailedReason
     * \value AuthSupplicantTimeoutReason
     * \value PppStartFailedReason
     * \value PppDisconnectReason
     * \value PppFailedReason
     * \value DhcpStartFailedReason
     * \value DhcpErrorReason
     * \value DhcpFailedReason
     * \value SharedStartFailedReason
     * \value SharedFailedReason
     * \value AutoIpStartFailedReason
     * \value AutoIpErrorReason
     * \value AutoIpFailedReason
     * \value ModemBusyReason
     * \value ModemNoDialToneReason
     * \value ModemNoCarrierReason
     * \value ModemDialTimeoutReason
     * \value ModemDialFailedReason
     * \value ModemInitFailedReason
     * \value GsmApnSelectFailedReason
     * \value GsmNotSearchingReason
     * \value GsmRegistrationDeniedReason
     * \value GsmRegistrationTimeoutReason
     * \value GsmRegistrationFailedReason
     * \value GsmPinCheckFailedReason
     * \value FirmwareMissingReason
     * \value DeviceRemovedReason
     * \value SleepingReason
     * \value ConnectionRemovedReason
     * \value UserRequestedReason
     * \value CarrierReason
     * \value ConnectionAssumedReason
     * \value SupplicantAvailableReason
     * \value ModemNotFoundReason
     * \value BluetoothFailedReason
     * \value GsmSimNotInserted
     * \value GsmSimPinRequired
     * \value GsmSimPukRequired
     * \value GsmSimWrong
     * \value InfiniBandMode
     * \value DependencyFailed
     * \value Br2684Failed
     * \value ModemManagerUnavailable
     * \value SsidNotFound
     * \value SecondaryConnectionFailed
     * \value DcbFcoeFailed
     * \value TeamdControlFailed
     * \value ModemFailed
     * \value ModemAvailable
     * \value SimPinIncorrect
     * \value NewActivation
     * \value ParentChanged
     * \value ParentManagedChanged
     * \value Reserved
     */
    enum StateChangeReason {
        UnknownReason = 0,
        NoReason = 1,
        NowManagedReason = 2,
        NowUnmanagedReason = 3,
        ConfigFailedReason = 4,
        ConfigUnavailableReason = 5,
        ConfigExpiredReason = 6,
        NoSecretsReason = 7,
        AuthSupplicantDisconnectReason = 8,
        AuthSupplicantConfigFailedReason = 9,
        AuthSupplicantFailedReason = 10,
        AuthSupplicantTimeoutReason = 11,
        PppStartFailedReason = 12,
        PppDisconnectReason = 13,
        PppFailedReason = 14,
        DhcpStartFailedReason = 15,
        DhcpErrorReason = 16,
        DhcpFailedReason = 17,
        SharedStartFailedReason = 18,
        SharedFailedReason = 19,
        AutoIpStartFailedReason = 20,
        AutoIpErrorReason = 21,
        AutoIpFailedReason = 22,
        ModemBusyReason = 23,
        ModemNoDialToneReason = 24,
        ModemNoCarrierReason = 25,
        ModemDialTimeoutReason = 26,
        ModemDialFailedReason = 27,
        ModemInitFailedReason = 28,
        GsmApnSelectFailedReason = 29,
        GsmNotSearchingReason = 30,
        GsmRegistrationDeniedReason = 31,
        GsmRegistrationTimeoutReason = 32,
        GsmRegistrationFailedReason = 33,
        GsmPinCheckFailedReason = 34,
        FirmwareMissingReason = 35,
        DeviceRemovedReason = 36,
        SleepingReason = 37,
        ConnectionRemovedReason = 38,
        UserRequestedReason = 39,
        CarrierReason = 40,
        ConnectionAssumedReason = 41,
        SupplicantAvailableReason = 42,
        ModemNotFoundReason = 43,
        BluetoothFailedReason = 44,
        GsmSimNotInserted = 45,
        GsmSimPinRequired = 46,
        GsmSimPukRequired = 47,
        GsmSimWrong = 48,
        InfiniBandMode = 49,
        DependencyFailed = 50,
        Br2684Failed = 51,
        ModemManagerUnavailable = 52,
        SsidNotFound = 53,
        SecondaryConnectionFailed = 54,
        DcbFcoeFailed = 55,
        TeamdControlFailed = 56,
        ModemFailed = 57,
        ModemAvailable = 58,
        SimPinIncorrect = 59,
        NewActivation = 60,
        ParentChanged = 61,
        ParentManagedChanged = 62,
        Reserved = 65536,
    };
    Q_ENUM(StateChangeReason)

    /*!
     *
     * \value UnknownStatus
     *        The device metered status is unknown.
     * \value Yes
     *        The device is metered and the value was statically set.
     * \value No
     *        The device is not metered and the value was statically set.
     * \value GuessYes
     *        The device is metered and the value was guessed.
     * \value GuessNo
     *        The device is not metered and the value was guessed.
     */
    enum MeteredStatus {
        UnknownStatus = 0,
        Yes = 1,
        No = 2,
        GuessYes = 3,
        GuessNo = 4,
    };
    Q_ENUM(MeteredStatus)

    /*!
     *
     * Possible device capabilities
     *
     * \value IsManageable
     *        denotes that the device can be controlled by this API
     * \value SupportsCarrierDetect
     *        the device informs us when it is plugged in to the medium
     */
    enum Capability {
        IsManageable = 0x1,
        SupportsCarrierDetect = 0x2,
    };
    Q_ENUM(Capability)
    Q_DECLARE_FLAGS(Capabilities, Capability)
    Q_FLAG(Capabilities)

    /*!
     *
     * Possible device interfaceflags
     *
     * \value None
     *        no flags set
     * \value Up
     *        Corresponds to kernel IFF_UP
     * \value LowerUp
     *        Corresponds to kernel IFF_LOWER_UP
     * \value Carrier
     *        the interface has carrier
     */
    enum Interfaceflag {
        None = NM_DEVICE_INTERFACE_FLAG_NONE,
        Up = NM_DEVICE_INTERFACE_FLAG_UP,
        LowerUp = NM_DEVICE_INTERFACE_FLAG_LOWER_UP,
        Carrier = NM_DEVICE_INTERFACE_FLAG_CARRIER
    };
    Q_ENUM(Interfaceflag)
    Q_DECLARE_FLAGS(Interfaceflags, Interfaceflag)
    Q_FLAG(Interfaceflags)

    /*!
     *
     * Device type
     *
     * \value UnknownType
     *        Unknown device type
     * \value Ethernet
     *        Ieee8023 wired ethernet
     * \value Wifi
     *        the Ieee80211 family of wireless networks
     * \value Unused1
     *        Currently unused
     * \value Unused2
     *        Currently unused
     * \value Bluetooth
     *        network bluetooth device (usually a cell phone)
     * \value OlpcMesh
     *        OLPC Mesh networking device
     * \value Wimax
     *        WiMax WWAN technology
     * \value Modem
     *        POTS, GSM, CDMA or LTE modems
     * \value InfiniBand
     *        Infiniband network device
     * \value Bond
     *        Bond virtual device
     * \value Vlan
     *        Vlan virtual device
     * \value Adsl
     *        ADSL modem device
     * \value Bridge
     *        Bridge virtual device
     * \value Generic
     *        Generic device
     * \value Team
     *        Team master device
     * \value Gre
     *        Gre virtual device. Deprecated, use IpTunnel instead
     * \value MacVlan
     *        MacVlan virtual device
     * \value Tun
     *        Tun virtual device
     * \value Veth
     *        Veth virtual device
     * \value IpTunnel
     *        IP Tunneling Device
     * \value VxLan
     *        Vxlan Device
     * \value MacSec
     *        MacSec Device
     * \value Dummy
     *        Dummy Device
     * \value Ppp
     *        Ppp Device
     * \value OvsInterface
     *        OvsInterface Device
     * \value OvsPort
     *        OvsPort Device
     * \value OvsBridge
     *        OvsBridge Device
     * \value Wpan
     *        Wpan Device
     * \value Lowpan
     *        Lowpan Device
     * \value WireGuard
     *        WireGuard Device
     * \value WifiP2P
     *        WifiP2P Device
     * \value VRF
     *        VRF (Virtual Routing and Forwarding) Device
     * \value Loopback
     *        Loopback Device
     */
    enum Type {
        UnknownType = NM_DEVICE_TYPE_UNKNOWN,
        Ethernet = NM_DEVICE_TYPE_ETHERNET,
        Wifi = NM_DEVICE_TYPE_WIFI,
        Unused1 = NM_DEVICE_TYPE_UNUSED1,
        Unused2 = NM_DEVICE_TYPE_UNUSED2,
        Bluetooth = NM_DEVICE_TYPE_BT,
        OlpcMesh = NM_DEVICE_TYPE_OLPC_MESH,
        Wimax = NM_DEVICE_TYPE_WIMAX,
        Modem = NM_DEVICE_TYPE_MODEM,
        InfiniBand = NM_DEVICE_TYPE_INFINIBAND,
        Bond = NM_DEVICE_TYPE_BOND,
        Vlan = NM_DEVICE_TYPE_VLAN,
        Adsl = NM_DEVICE_TYPE_ADSL,
        Bridge = NM_DEVICE_TYPE_BRIDGE,
        Generic = NM_DEVICE_TYPE_GENERIC,
        Team = NM_DEVICE_TYPE_TEAM,
        Gre,
        MacVlan,
        Tun,
        Veth,
        IpTunnel,
        VxLan,
        MacSec,
        Dummy,
        Ppp,
        OvsInterface,
        OvsPort,
        OvsBridge,
        Wpan,
        Lowpan,
        WireGuard,
        WifiP2P,
        VRF,
        Loopback,
    };
    Q_ENUM(Type)
    Q_DECLARE_FLAGS(Types, Type)
    Q_FLAG(Types)

    /*!
     * Creates a new device object.
     *
     * \a path UNI of the device
     */
    explicit Device(const QString &path, QObject *parent = nullptr);
    /*!
     * Destroys a device object.
     */
    ~Device() override;
    /*!
     * Retrieves the interface type.  This is a virtual function that will return the
     * proper type of all sub-classes.
     *
     * Returns the NetworkManager::Device::Type that corresponds to this device.
     */
    virtual Type type() const;
    /*!
     * Retrieves the Unique Network Identifier (UNI) of the device.
     * This identifier is unique for each network and network interface in the system.
     *
     * Returns the Unique Network Identifier of the current device
     */
    QString uni() const;
    /*!
     * The current active connection for this device
     *
     * Returns A valid ActiveConnection object or NULL if no active connection was found
     */
    NetworkManager::ActiveConnection::Ptr activeConnection() const;
    /*!
     * Returns available connections for this device
     *
     * Returns List of availables connection
     */
    Connection::List availableConnections();
    /*!
     * The system name for the network device
     */
    QString interfaceName() const;
    /*!
     * The name of the device's data interface when available. This property
     * may not refer to the actual data interface until the device has
     * successfully established a data connection, indicated by the device's
     * state() becoming ACTIVATED.
     */
    QString ipInterfaceName() const;
    /*!
     * Handle for the system driver controlling this network interface
     */
    QString driver() const;
    /*!
     * The driver version.
     */
    QString driverVersion() const;
    /*!
     * The firmware version.
     */
    QString firmwareVersion() const;
    /*!
     * Reapplies connection settings on the interface.
     */
    QDBusPendingReply<> reapplyConnection(const NMVariantMapMap &connection, qulonglong version_id, uint flags);
    /*!
     * Disconnects a device and prevents the device from automatically
     * activating further connections without user intervention.
     */
    QDBusPendingReply<> disconnectInterface();
    /*!
     * Deletes a software device from NetworkManager and removes the interface from the system.
     * The method returns an error when called for a hardware device.
     *
     * \since 5.8.0
     *
     */
    QDBusPendingReply<> deleteInterface();
    /*!
     * returns the current IPv4 address without the prefix
     * \sa ipV4Config()
     * \sa ipV6Config()
     * \deprecated
     */
    QHostAddress ipV4Address() const;
    /*!
     * Get the current IPv4 configuration of this device.
     * Only valid when device is Activated.
     */
    IpConfig ipV4Config() const;
    /*!
     * Get the current IPv6 configuration of this device.
     * Only valid when device is Activated.
     */
    IpConfig ipV6Config() const;

    /*!
     * Get the DHCP options returned by the DHCP server
     * or a null pointer if the device is not Activated or does not
     * use DHCP configuration.
     */
    Dhcp4Config::Ptr dhcp4Config() const;

    /*!
     * Get the DHCP options returned by the DHCP server
     * or a null pointer if the device is not Activated or does not
     * use DHCP configuration.
     */
    Dhcp6Config::Ptr dhcp6Config() const;

    /*!
     * Retrieves the activation status of this network interface.
     *
     * Returns true if this network interface is active, false otherwise
     */
    bool isActive() const;

    /*!
     * Retrieves the device is valid.
     *
     * Returns true if this device interface is valid, false otherwise
     */
    bool isValid() const;

    /*!
     * Retrieves the current state of the device.
     * This is a high level view of the device. It is user oriented, so
     * actually it provides state coming from different layers.
     *
     * Returns the current connection state
     * \sa Device::State
     */
    State state() const;
    /*!
     * Retrieves the maximum speed as reported by the device.
     * Note that this is only a design related piece of information, and that
     * the device might not reach this maximum.
     *
     * Returns the device's maximum speed
     */
    int designSpeed() const;
    /*!
     * Retrieves the capabilities supported by this device.
     *
     * Returns the capabilities of the device
     */
    Capabilities capabilities() const;
    /*!
     *
     */
    QVariant capabilitiesV() const;
    /*!
     * Is the device currently being managed by NetworkManager?
     */
    bool managed() const;
    /*!
     * Sets the managed flag for the device
     *
     * \since 6.18.0
     */
    void setManaged(bool managed);
    /*!
     * The up or down flag for the device
     */
    Interfaceflags interfaceFlags() const;
    /*!
     * Is the firmware needed by the device missing?
     */
    bool firmwareMissing() const;
    /*!
     * If the device is allowed to autoconnect.
     */
    bool autoconnect() const;
    /*!
     * The current state and reason for changing to that state.
     */
    DeviceStateReason stateReason() const;
    /*!
     * Retrieves the Unique Device Identifier (UDI) of the device.
     * This identifier is unique for each device in the system.
     */
    QString udi() const;

    /*!
     * Returns If non-empty, an (opaque) indicator of the physical network
     * port associated with the device. This can be used to recognize
     * when two seemingly-separate hardware devices are actually just
     * different virtual interfaces to the same physical port.
     *
     * \since 0.9.9.0
     */
    QString physicalPortId() const;
    /*!
     * The device MTU (maximum transmission unit)
     * \since 0.9.9.0
     */
    uint mtu() const;

    /*!
     * Returns If TRUE, indicates the NetworkManager plugin for the device is likely
     * missing or misconfigured.
     * \since 5.14.0
     */
    bool nmPluginMissing() const;

    /*!
     * Returns Whether the amount of traffic flowing through the device is
     * subject to limitations, for example set by service providers.
     * \since 5.14.0
     */
    MeteredStatus metered() const;

    /*!
     * If true, indicates the device is allowed to autoconnect.
     * If false, manual intervention is required before the device
     * will automatically connect to a known network, such as activating
     * a connection using the device, or setting this property to \a true.
     */
    void setAutoconnect(bool autoconnect);

    /*!
     * Returns Device Statistics interface
     */
    DeviceStatistics::Ptr deviceStatistics() const;

    /*!
     * Retrieves a specialized interface to interact with the device corresponding
     * to a given device interface.
     *
     * Returns a pointer to the device interface if it exists, \a 0 otherwise
     */
    template<class DevIface>
    DevIface *as()
    {
        return qobject_cast<DevIface *>(this);
    }

    /*!
     * Retrieves a specialized interface to interact with the device corresponding
     * to a given device interface.
     *
     * Returns a pointer to the device interface if it exists, 0 otherwise
     */
    template<class DevIface>
    const DevIface *as() const
    {
        return qobject_cast<const DevIface *>(this);
    }

Q_SIGNALS:
    /*!
     * This signal is emitted when the device's link status changed.
     *
     * \a newstate the new state of the connection
     *
     * \a oldstate the previous state of the connection
     *
     * \a reason the reason for the state change, if any.  ReasonNone where the backend
     * provides no reason.
     *
     * \sa Device::State
     * \sa Device::StateChangeReason
     */
    void stateChanged(NetworkManager::Device::State newstate, NetworkManager::Device::State oldstate, NetworkManager::Device::StateChangeReason reason);

    /*!
     * Emitted when the autoconnect of this network has changed.
     */
    void activeConnectionChanged();

    /*!
     * Emitted when the autoconnect of this network has changed.
     */
    void autoconnectChanged();

    /*!
     * Emitted when the list of avaiable connections of this network has changed.
     */
    void availableConnectionChanged();

    /*!
     * Emitted when a new connection is available
     */
    void availableConnectionAppeared(const QString &connection);

    /*!
     * Emitted when the connection is no longer available
     */
    void availableConnectionDisappeared(const QString &connection);

    /*!
     * Emitted when the capabilities of this network has changed.
     */
    void capabilitiesChanged();

    /*!
     * Emitted when the DHCP configuration for IPv4 of this network has changed.
     */
    void dhcp4ConfigChanged();

    /*!
     * Emitted when the DHCP configuration for IPv6 of this network has changed.
     */
    void dhcp6ConfigChanged();

    /*!
     * Emitted when the driver of this network has changed.
     */
    void driverChanged();

    /*!
     * Emitted when the driver version of this network has changed.
     */
    void driverVersionChanged();

    /*!
     * Emitted when the firmware missing state of this network has changed.
     */
    void firmwareMissingChanged();

    /*!
     * Emitted when the firmware version of this network has changed.
     */
    void firmwareVersionChanged();

    /*!
     * Emitted when the interface name of this network has changed.
     */
    void interfaceNameChanged();

    /*!
     * Emitted when the IPv4 address of this network has changed.
     */
    void ipV4AddressChanged();

    /*!
     * Emitted when the IPv4 configuration of this network has changed.
     */
    void ipV4ConfigChanged();

    /*!
     * Emitted when the IPv6 configuration of this network has changed.
     */
    void ipV6ConfigChanged();

    /*!
     * Emitted when the ip interface name of this network has changed.
     */
    void ipInterfaceChanged();

    /*!
     * Emitted when the managed state of this network has changed.
     */
    void managedChanged();

    /*!
     * Emitted when the up or down state of the device
     * \since 1.22
     * \note will always return NM_DEVICE_INTERFACE_FLAG_NONE when runtime NM < 1.22
     */
    void interfaceFlagsChanged();

    /*!
     * Emitted when the physical port ID changes.
     * \sa physicalPortId()
     * \since 0.9.9.0
     */
    void physicalPortIdChanged();

    /*!
     * Emitted when the maximum transmission unit has changed
     * \since 0.9.9.0
     */
    void mtuChanged();

    /*!
     * Emitted when NmPluginMissing property has changed
     * \since 5.14.0
     * \sa nmPluginMissing
     */
    void nmPluginMissingChanged(bool nmPluginMissing);

    /*!
     * Emitted when metered property has changed
     * \since 5.14.0
     * \sa metered
     */
    void meteredChanged(MeteredStatus metered);

    /*!
     * Emitted when the connection state of this network has changed.
     */
    void connectionStateChanged();

    /*!
     * Emitted when the state reason of this network has changed.
     */
    void stateReasonChanged();

    /*!
     * Emitted when the Unique Device Identifier of this device has changed.
     */
    void udiChanged();

protected:
    NETWORKMANAGERQT_NO_EXPORT Device(DevicePrivate &dd, QObject *parent);

    DevicePrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(Device)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Device::Capabilities)
Q_DECLARE_OPERATORS_FOR_FLAGS(Device::Types)
Q_DECLARE_OPERATORS_FOR_FLAGS(Device::Interfaceflags)

/*!
 * \class NetworkManager::DeviceStateReason
 * \inheaderfile NetworkManagerQt/Device
 * \inmodule NetworkManagerQt
 */
class NETWORKMANAGERQT_EXPORT DeviceStateReason
{
public:
    /*!
     */
    DeviceStateReason(Device::State state, Device::StateChangeReason reason);
    /*!
     */
    DeviceStateReason(const DeviceStateReason &);
    /*!
     */
    ~DeviceStateReason();
    /*!
     */
    Device::State state() const;
    /*!
     */
    Device::StateChangeReason reason() const;
    /*!
     */
    DeviceStateReason &operator=(const DeviceStateReason &);

private:
    Q_DECLARE_PRIVATE(DeviceStateReason)

    DeviceStateReasonPrivate *const d_ptr;
};

}

#endif
