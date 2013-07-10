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

#ifndef NMQT_WIRELESSDEVICE_H
#define NMQT_WIRELESSDEVICE_H

#include "NetworkManagerQt-export.h"
#include "device.h"
#include "accesspoint.h"
#include "wirelessnetwork.h"

#include <QDBusObjectPath>
#include <QDBusPendingReply>

namespace NetworkManager
{

class WirelessDevicePrivate;

/**
 * A wireless network interface
 */
class NETWORKMANAGERQT_EXPORT WirelessDevice : public Device
{
    Q_OBJECT
    Q_ENUMS(OperationMode DeviceInterface)

public:
    typedef QSharedPointer<WirelessDevice> Ptr;
    typedef QList<Ptr> List;

    /**
     * The device's current operating mode
     * Unknown: not associated with a network
     * Adhoc: part of an adhoc network
     * Infra: a station in an infrastructure wireless network
     * APMode: access point in an infrastructure network
     */
    enum OperationMode { Unknown = 0, Adhoc, Infra, ApMode };
    /**
     * Capabilities (currently all encryption/authentication related) of the device
     * NoCapability = 0x0,
     * Wep40: 40 bit WEP cipher
     * Wep104: 104 bit WEP cipher
     * Tkip: TKIP encryption cipher
     * Ccmp: CCMP encryption cipher
     * Wpa: WPA authentication protocol
     * Rsn: RSN authethication protocol
     * ApCap: The device supports Access Point mode.
     * AdhocCap: The device supports Ad-Hoc mode.
     */
    enum Capability { NoCapability = 0x0, Wep40 = 0x1, Wep104 = 0x2, Tkip = 0x4, Ccmp = 0x8, Wpa = 0x10, Rsn = 0x20, ApCap = 0x40, AdhocCap = 0x80};
    Q_DECLARE_FLAGS(Capabilities, Capability)
    /**
     * Creates a new WirelessNetworkInterface object.
     *
     * @param path the DBus path of the devise
     */
    explicit WirelessDevice(const QString &path, QObject *parent = 0);
    /**
     * Destroys a WirelessNetworkInterface object.
     */
    ~WirelessDevice();
    /**
     * Return the type
     */
    virtual Type type() const;
    /**
     * List of wireless networks currently visible to the hardware
     */
    QStringList accessPoints() const;
    /**
     * Asks the device for a new scan of available wireless networks
     * @param options Options of scan
     * No documentation for options yet, see http://projects.gnome.org/NetworkManager/developers/api/09/spec.html#org.freedesktop.NetworkManager.Device.Wireless
     */
    QDBusPendingReply<> requestScan(const QVariantMap &options = QVariantMap());
    /**
     * Identifier (path) of the network this interface is currently associated with
     */
    AccessPoint::Ptr activeAccessPoint() const;
    /**
     * The permanent hardware address of the network interface
     */
    QString permanentHardwareAddress() const;
    /**
     * The hardware address currently used by the network interface
     */
    QString hardwareAddress() const;

    /**
     * Retrieves the operation mode of this network.
     *
     * @return the current mode
     * @see OperationMode
     */
    WirelessDevice::OperationMode mode() const;
    /**
     * Retrieves the effective bit rate currently attainable by this device.
     *
     * @return the bitrate in Kbit/s
     */
    int bitRate() const;
    /**
     * Retrieves the capabilities of this wifi network.
     *
     * @return the flag set describing the capabilities
     * @see Capabilities
     */
    WirelessDevice::Capabilities wirelessCapabilities() const;

    /**
     * Helper method to convert wire representation of operation mode to enum
     */
    static WirelessDevice::OperationMode convertOperationMode(uint);
    /**
     * Helper method to convert wire representation of capabilities to enum
     */
    static WirelessDevice::Capabilities convertCapabilities(uint);
    /**
     * Finds access point object given its Unique Network Identifier.
     *
     * @param uni the identifier of the AP to find from this network interface
     * @returns a valid AccessPoint object if a network having the given UNI for this device is known to the system, 0 otherwise
     */
    AccessPoint::Ptr findAccessPoint(const QString &uni);

    /**
     * Return the current list of networks
     */
    WirelessNetwork::List networks() const;

    /**
     * Find a network with the given ssid, a Null object is
     * returned if it can not be found
     */
    WirelessNetwork::Ptr findNetwork(const QString &ssid) const;

Q_SIGNALS:
    /**
     * This signal is emitted when the bitrate of this network has changed.
     *
     * @param bitrate the new bitrate value for this network
     */
    void bitRateChanged(int bitrate);
    /**
     * The active network changed.
     */
    void activeAccessPointChanged(const QString &);
    /**
     * The device switched operating mode.
     */
    void modeChanged(WirelessDevice::OperationMode);
    /**
     * The device changed its capabilities
     */
    void wirelessCapabilitiesChanged(Capabilities);
    /**
     * The device changed its hardware address
     */
    void hardwareAddressChanged(const QString &);
    /**
     * The device changed its permanent hardware address
     */
    void permanentHardwareAddressChanged(const QString &);
    /**
     * The device changed its properties
     */
    void wirelessPropertiesChanged(uint);
    /**
     * A new wireless access point appeared
     */
    void accessPointAppeared(const QString &uni);
    /**
     * A wireless access point disappeared
     */
    void accessPointDisappeared(const QString &uni);
    /**
     * A wireless network appeared
     */
    void networkAppeared(const QString &ssid);
    /**
     * A wireless network disappeared
     */
    void networkDisappeared(const QString &ssid);

protected:
    /**
     * When subclassing make sure to call the parent class method
     * if the property was not useful to your new class
     */
    virtual void propertyChanged(const QString &property, const QVariant &value);

protected Q_SLOTS:
    void accessPointAdded(const QDBusObjectPath &);
    void accessPointRemoved(const QDBusObjectPath &);
    void removeNetwork(const QString &network);

private:
    Q_DECLARE_PRIVATE(WirelessDevice)
};

} // namespace NetworkManager
#endif //NMQT_WIRELESSDEVICE_H

