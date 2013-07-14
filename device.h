/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
Copyright 2011-2013 Lamarque V. Souza <lamarque@kde.org>
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

#ifndef NMQT_DEVICE_H
#define NMQT_DEVICE_H

#include <QObject>
#include <QtCore/QtGlobal>
#include <QSharedPointer>

#include "NetworkManagerQt-export.h"
#include "generic-types.h"
#include "ipconfig.h"
#include "dhcp4config.h"
#include "dhcp6config.h"
#include "activeconnection.h"

namespace NetworkManager
{

class DevicePrivate;
class DeviceStateReason;
class DeviceStateReasonPrivate;

class NETWORKMANAGERQT_EXPORT Device : public QObject
{
    Q_OBJECT
    Q_ENUMS(State Capability Type)
    Q_FLAGS(Capabilities)
    Q_FLAGS(Types)

    Q_PROPERTY(QString uni READ uni)
    Q_PROPERTY(QString interfaceName READ interfaceName)
    Q_PROPERTY(QString ipInterfaceName READ ipInterfaceName)
    Q_PROPERTY(QString driver READ driver)
    Q_PROPERTY(QString driverVersion READ driverVersion)
    Q_PROPERTY(QString firmwareVersion READ firmwareVersion)
    Q_PROPERTY(QVariant genericCapabilities READ capabilitiesV)
    Q_PROPERTY(QHostAddress ipV4Address READ ipV4Address)
    Q_PROPERTY(bool managed READ managed)
    Q_PROPERTY(QString udi READ udi)
    Q_PROPERTY(bool firmwareMissing READ firmwareMissing)
    Q_PROPERTY(bool autoconnect READ autoconnect WRITE setAutoconnect)
    Q_PROPERTY(DeviceStateReason stateReason READ stateReason)

    //Q_PROPERTY(Solid::Control::IPv4Config ipV4Config READ ipV4Config WRITE setIpV4Config)
    Q_PROPERTY(State state READ state)
    Q_FLAGS(Capabilities)

public:
    typedef QSharedPointer<Device> Ptr;
    typedef QList<Ptr> List;
    /**
     * Device connection states describe the possible states of a
     * network connection from the user's point of view.  For
     * simplicity, states from several different layers are present -
     * this is a high level view
     */
    enum State { UnknownState = 0, Unmanaged = 10, Unavailable = 20, Disconnected = 30 , Preparing = 40,
                 ConfiguringHardware = 50 , NeedAuth = 60, ConfiguringIp = 70, CheckingIp = 80, WaitingForSecondaries = 90, Activated = 100, Deactivating = 110, Failed = 120
               };

    /**
     * Enums describing the reason for a connection state change
     */
    enum StateChangeReason { UnknownReason = 0, NoReason = 1 , NowManagedReason = 2, NowUnmanagedReason = 3,
                             ConfigFailedReason = 4, ConfigUnavailableReason = 5,
                             ConfigExpiredReason = 6, NoSecretsReason = 7, AuthSupplicantDisconnectReason = 8,
                             AuthSupplicantConfigFailedReason = 9, AuthSupplicantFailedReason = 10,
                             AuthSupplicantTimeoutReason = 11, PppStartFailedReason = 12, PppDisconnectReason = 13,
                             PppFailedReason = 14, DhcpStartFailedReason = 15, DhcpErrorReason = 16, DhcpFailedReason = 17,
                             SharedStartFailedReason = 18, SharedFailedReason = 19,
                             AutoIpStartFailedReason = 20, AutoIpErrorReason = 21, AutoIpFailedReason = 22,
                             ModemBusyReason = 23, ModemNoDialToneReason = 24, ModemNoCarrierReason = 25, ModemDialTimeoutReason = 26,
                             ModemDialFailedReason = 27, ModemInitFailedReason = 28,
                             GsmApnSelectFailedReason = 29, GsmNotSearchingReason = 30, GsmRegistrationDeniedReason = 31,
                             GsmRegistrationTimeoutReason = 32, GsmRegistrationFailedReason = 33,
                             GsmPinCheckFailedReason = 34, FirmwareMissingReason = 35, DeviceRemovedReason = 36,
                             SleepingReason = 37, ConnectionRemovedReason = 38, UserRequestedReason = 39, CarrierReason = 40,
                             ConnectionAssumedReason = 41, SupplicantAvailableReason = 42, ModemNotFoundReason = 43, BluetoothFailedReason = 44,
                             GsmSimNotInserted = 45, GsmSimPinRequired = 46, GsmSimPukRequired = 47, GsmSimWrong = 48 , InfiniBandMode = 49,
                             DependencyFailed = 50, Br2684Failed = 51, ModemManagerUnavailable = 52, SsidNotFound = 53, SecondaryConnectionFailed = 54,
                             Reserved = 65536
                           };
    /**
     * Possible Device capabilities
     * - IsManageable: denotes that the device can be controlled by this API
     * - SupportsCarrierDetect: the device informs us when it is plugged in to the medium
     */
    enum Capability {
        IsManageable = 0x1,
        SupportsCarrierDetect = 0x2
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)

    /**
     * Device medium types
     * - Ethernet: Ieee8023 wired ethernet
     * - Wifi:  the Ieee80211 family of wireless networks
     * - Bluetooth: network bluetooth device (usually a cell phone)
     * - Modem: POTS, GSM, CDMA or LTE modems
     * - OlpcMesh: OLPC Mesh networking device
     * - Wimax: WiMax WWAN technology
     */
    enum Type {
        UnknownType,
        Ethernet,
        Wifi,
        Unused1,
        Unused2,
        Bluetooth,
        OlpcMesh,
        Wimax,
        Modem,
        InfiniBand,
        Bond,
        Vlan,
        Adsl,
        Bridge
    };
    Q_DECLARE_FLAGS(Types, Type)

    /**
     * Creates a new NetworkInterface object.
     *
     * @param path UNI of the device
     */
    explicit Device(const QString &path, QObject *parent = 0);
    Device(DevicePrivate &dd, QObject *parent);
    /**
     * Destroys a NetworkInterface object.
     */
    virtual ~Device();
    /**
     * Retrieves the interface type.  This is a virtual function that will return the
     * proper type of all sub-classes.
     *
     * @returns the NetworkManager::Device::Type that corresponds to this device.
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
     * @returns A valid ActiveConnection object or NULL if no active connection was found
     */
    NetworkManager::ActiveConnection::Ptr activeConnection() const;
    /**
     * Returns available connections for this device
     *
     * @returns List of availables connection
     */
    Connection::List availableConnections();
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
     * The driver version.
     */
    QString driverVersion() const;
    /**
     * The firmware version.
     */
    QString firmwareVersion() const;
    /**
     * Disconnects a device and prevents the device from automatically
     * activating further connections without user intervention.
     */
    void disconnectInterface();
    /**
     * returns the current IPv4 address without the prefix
     * \sa ipV4Config()
     */
    QHostAddress ipV4Address() const;
    /**
     * Get the current IPv4 configuration of this device.
     * Only valid when device is Activated.
     */
    IpConfig ipV4Config() const;
    /**
     * Get the current IPv6 configuration of this device.
     * Only valid when device is Activated.
     */
    IpConfig ipV6Config() const;

    /**
     * Get the DHCP options returned by the DHCP server
     * or a null pointer if the device is not Activated or does not
     * use DHCP configuration.
     */
    Dhcp4Config::Ptr dhcp4Config() const;

    /**
     * Get the DHCP options returned by the DHCP server
     * or a null pointer if the device is not Activated or does not
     * use DHCP configuration.
     */
    Dhcp6Config::Ptr dhcp6Config() const;

    /**
     * Retrieves the activation status of this network interface.
     *
     * @return true if this network interface is active, false otherwise
     */
    bool isActive() const;

    /**
     * Retrieves the device is valid.
     *
     * @return true if this device interface is valid, false otherwise
     */
    bool isValid() const;

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
    /**
     * If the device is allowed to autoconnect.
     */
    bool autoconnect() const;
    /**
     * The current state and reason for changing to that state.
     */
    DeviceStateReason stateReason() const;
    /**
     * Retrieves the Unique Device Identifier (UDI) of the NetworkInterface.
     * This identifier is unique for each device in the system.
     */
    QString udi() const;

    /**
     * If true, indicates the device is allowed to autoconnect.
     * If false, manual intervention is required before the device
     * will automatically connect to a known network, such as activating
     * a connection using the device, or setting this property to true.
     */
    void setAutoconnect(bool autoconnect);

    /**
      * Retrieves a specialized interface to interact with the device corresponding
      * to a given device interface.
      *
      * @returns a pointer to the device interface if it exists, 0 otherwise
      */
    template <class DevIface> DevIface *as() {
        return qobject_cast<DevIface *>(this);
    }

    /**
      * Retrieves a specialized interface to interact with the device corresponding
      * to a given device interface.
      *
      * @returns a pointer to the device interface if it exists, 0 otherwise
      */
    template <class DevIface> const DevIface *as() const {
        return qobject_cast<const DevIface *>(this);
    }

Q_SIGNALS:
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

    /**
     * Emitted when the autoconnect of this network has changed.
     */
    void activeConnectionChanged();

    /**
     * Emitted when the autoconnect of this network has changed.
     */
    void autoconnectChanged();

    /**
     * Emitted when the list of avaiable connections of this network has changed.
     */
    void availableConnectionChanged();

    /**
     * Emitted when a new connection is available
     */
    void availableConnectionAppeared(const QString &connection);

    /**
     * Emitted when the connection is no longer available
     */
    void availableConnectionDisappeared(const QString &connection);

    /**
     * Emitted when the capabilities of this network has changed.
     */
    void capabilitiesChanged();

    /**
     * Emitted when the DHCP configuration for IPv4 of this network has changed.
     */
    void dhcp4ConfigChanged();

    /**
     * Emitted when the DHCP configuration for IPv6 of this network has changed.
     */
    void dhcp6ConfigChanged();

    /**
     * Emitted when the driver of this network has changed.
     */
    void driverChanged();

    /**
     * Emitted when the driver version of this network has changed.
     */
    void driverVersionChanged();

    /**
     * Emitted when the firmware missing state of this network has changed.
     */
    void firmwareMissingChanged();

    /**
     * Emitted when the firmware version of this network has changed.
     */
    void firmwareVersionChanged();

    /**
     * Emitted when the interface name of this network has changed.
     */
    void interfaceNameChanged();

    /**
     * Emitted when the IPv4 address of this network has changed.
     */
    void ipV4AddressChanged();

    /**
     * Emitted when the IPv4 configuration of this network has changed.
     */
    void ipV4ConfigChanged();

    /**
     * Emitted when the IPv6 configuration of this network has changed.
     */
    void ipV6ConfigChanged();

    /**
     * Emitted when the ip interface name of this network has changed.
     */
    void ipInterfaceChanged();

    /**
     * Emitted when the managed state of this network has changed.
     */
    void managedChanged();

    /**
     * Emitted when the connection state of this network has changed.
     */
    void connectionStateChanged();

    /**
     * Emitted when the state reason of this network has changed.
     */
    void stateReasonChanged();

    /**
     * Emitted when the Unique Device Identifier of this device has changed.
     */
    void udiChanged();

protected:
    /**
     * When subclassing make sure to call the parent class method
     * if the property was not useful to your new class
     */
    virtual void propertyChanged(const QString &property, const QVariant &value);

    DevicePrivate *d_ptr;

protected Q_SLOTS:
    void deviceStateChanged(uint, uint, uint);
    void propertiesChanged(const QVariantMap &properties);

private:
    Q_DECLARE_PRIVATE(Device)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Device::Capabilities)
Q_DECLARE_OPERATORS_FOR_FLAGS(Device::Types)

class NETWORKMANAGERQT_EXPORT DeviceStateReason
{
public:
    DeviceStateReason(Device::State state, Device::StateChangeReason reason);
    DeviceStateReason(const DeviceStateReason &);
    ~DeviceStateReason();
    Device::State state() const;
    Device::StateChangeReason reason() const;
    DeviceStateReason &operator=(const DeviceStateReason &);

private:
    Q_DECLARE_PRIVATE(DeviceStateReason)

    DeviceStateReasonPrivate *d_ptr;
};

}

#endif

