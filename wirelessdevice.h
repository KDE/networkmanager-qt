/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>

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

#include "QtNetworkManager-export.h"
#include "device.h"

#include <QDBusObjectPath>
#include <QDBusPendingReply>

namespace NetworkManager
{

class WirelessDevicePrivate;
class AccessPoint;
typedef QStringList AccessPointList;

/**
 * A wireless network interface
 */
class NMQT_EXPORT WirelessDevice : public Device
{
Q_OBJECT
Q_ENUMS(OperationMode DeviceInterface)
Q_DECLARE_PRIVATE(WirelessDevice)

public:
    /**
     * The device's current operating mode
     * Unassociated: not associated with a network
     * Adhoc: part of an adhoc network
     * Managed: a station in an infrastructure wireless network
     * Master: access point in an infrastructure network
     * Repeater: dumb repeater
     */
    enum OperationMode { Unassociated, Adhoc, Managed, Master, Repeater };
    /**
     * Capabilities (currently all encryption/authentication related) of the device
     * NoCapability = 0x0,
     * Wep40: 40 bit WEP cipher
     * Wep104: 104 bit WEP cipher
     * Tkip: TKIP encryption cipher
     * Ccmp: CCMP encryption cipher
     * Wpa: WPA authentication protocol
     * Rsn: RSN authethication protocol
     */
    enum Capability { NoCapability = 0x0, Wep40 = 0x1, Wep104 = 0x2, Tkip = 0x4, Ccmp = 0x8, Wpa = 0x10, Rsn = 0x20 };
    Q_DECLARE_FLAGS(Capabilities, Capability)
    /**
     * Creates a new WirelessNetworkInterface object.
     *
     * @param path the DBus path of the devise
     */
    WirelessDevice(const QString & path, QObject * parent = 0);
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
    AccessPointList accessPoints() const;
    /**
     * @param options Options of scan
     * No documentation for options yet, see http://projects.gnome.org/NetworkManager/developers/api/09/spec.html#org.freedesktop.NetworkManager.Device.Wireless
     */
    QDBusPendingReply<> requestScan(QVariantMap & options);
    /**
     * Identifier of the network this interface is currently associated with
     */
    QString activeAccessPoint() const;
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
     * @see Solid::Control::WirelessNetworkInterface::OperationMode
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
     * @see Solid::Control::WirelessNetworkInterface::DeviceInterface
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
    AccessPoint *findAccessPoint(const QString & uni) const;

protected Q_SLOTS:
    void wirelessPropertiesChanged(const QVariantMap &);
    void accessPointAdded(const QDBusObjectPath &);
    void accessPointRemoved(const QDBusObjectPath &);
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
    void accessPointAppeared(const QString &);
    /**
     * A wireless access point disappeared
     */
    void accessPointDisappeared(const QString &);
};

} // namespace NetworkManager
#endif //NMQT_WIRELESSDEVICE_H

