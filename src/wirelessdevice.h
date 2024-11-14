/*
    SPDX-FileCopyrightText: 2008, 2011 Will Stephenson <wstephenson@kde.org>
    SPDX-FileCopyrightText: 2011-2013 Lamarque V. Souza <lamarque@kde.org>
    SPDX-FileCopyrightText: 2013 Daniel Nicoletti <dantti12@gmail.com>
    SPDX-FileCopyrightText: 2013 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_WIRELESSDEVICE_H
#define NETWORKMANAGERQT_WIRELESSDEVICE_H

#include "accesspoint.h"
#include "device.h"
#include "wirelessnetwork.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

#include <QDBusPendingReply>

namespace NetworkManager
{
class WirelessDevicePrivate;

/*!
 * \class NetworkManager::WirelessDevice
 * \inheaderfile NetworkManagerQt/WirelessDevice
 * \inmodule NetworkManagerQt
 *
 * \brief A wireless network interface.
 */
class NETWORKMANAGERQT_EXPORT WirelessDevice : public Device
{
    Q_OBJECT

public:
    /*!
     * \typedef NetworkManager::WirelessDevice::Ptr
     */
    typedef QSharedPointer<WirelessDevice> Ptr;
    /*!
     * \typedef NetworkManager::WirelessDevice::List
     */
    typedef QList<Ptr> List;
    /*!
     * The device's current operating mode
     *
     * \value Unknown
     *        not associated with a network
     * \value Adhoc
     *        part of an adhoc network
     * \value Infra
     *        a station in an infrastructure wireless network
     * \value ApMode
     *        access point in an infrastructure network
     */
    enum OperationMode {
        Unknown = 0,
        Adhoc,
        Infra,
        ApMode,
    };
    Q_ENUM(OperationMode)
    /*!
     * Capabilities (currently all encryption/authentication related) of the device
     * \note FreqValid, Freq2Ghz, Freq5Ghz are available in runtime NM >= 1.0.2
     *
     * The device's current operating mode
     *
     * \value NoCapability
     *        Null capability
     * \value Wep40
     *        40 bit WEP cipher
     * \value Wep104
     *        104 bit WEP cipher
     * \value Tkip
     *        TKIP encryption cipher
     * \value Ccmp
     *        CCMP encryption cipher
     * \value WpaWpa
     *        WPA authentication protocol
     * \value Rsn
     *        RSN authethication protocol
     * \value ApCap
     *        The device supports Access Point mode.
     * \value AdhocCap
     *        The device supports Ad-Hoc mode.
     * \value FreqValid
     *        The device properly reports information about supported frequencies
     * \value Freq2Ghz
     *        The device supports 2.4Ghz frequencies
     * \value Freq5Ghz
     *        The device supports 5Ghz frequencies
     * \value Mesh
     *        The device supports acting as a mesh point
     * \value IBSSRsn
     *        device supports WPA2/RSN in an IBSS network
     */
    enum Capability {
        NoCapability = 0x0,
        Wep40 = 0x1,
        Wep104 = 0x2,
        Tkip = 0x4,
        Ccmp = 0x8,
        Wpa = 0x10,
        Rsn = 0x20,
        ApCap = 0x40,
        AdhocCap = 0x80,
        FreqValid = 0x100,
        Freq2Ghz = 0x200,
        Freq5Ghz = 0x400,
        Mesh = 0x1000,
        IBSSRsn = 0x2000,
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)
    /*!
     * Creates a new WirelessDevice object.
     *
     * \a path the DBus path of the devise
     */
    explicit WirelessDevice(const QString &path, QObject *parent = nullptr);
    /*!
     * Destroys a WirelessDevice object.
     */
    ~WirelessDevice() override;
    /*!
     * Return the type
     */
    Type type() const override;
    /*!
     * List of wireless networks currently visible to the hardware
     */
    QStringList accessPoints() const;
    /*!
     * Asks the device for a new scan of available wireless networks
     *
     * \a options Options of scan
     *
     * No documentation for options yet, see
     * https://projects.gnome.org/NetworkManager/developers/api/09/spec.html#org.freedesktop.NetworkManager.Device.Wireless
     */
    QDBusPendingReply<> requestScan(const QVariantMap &options = QVariantMap());
    /*!
     * AccessPoint pointer this interface is currently associated with
     */
    AccessPoint::Ptr activeAccessPoint() const;
    /*!
     * The permanent hardware address of the network interface
     */
    QString permanentHardwareAddress() const;
    /*!
     * The hardware address currently used by the network interface
     */
    QString hardwareAddress() const;

    /*!
     * Retrieves the operation mode of this network.
     *
     * Returns the current mode
     * \sa OperationMode
     */
    WirelessDevice::OperationMode mode() const;
    /*!
     * Retrieves the effective bit rate currently attainable by this device.
     *
     * Returns the bitrate in Kbit/s
     */
    int bitRate() const;
    /*!
     * The LastScan property value, converted to QDateTime
     * \since 5.62.0
     * \note will always return invalid QDateTime when runtime NM < 1.12
     * Returns
     */
    QDateTime lastScan() const;
    /*!
     * The time the last RequestScan function was called
     * \since 5.62.0
     * Returns
     */
    QDateTime lastRequestScan() const;
    /*!
     * Retrieves the capabilities of this wifi network.
     *
     * Returns the flag set describing the capabilities
     * \sa Capabilities
     */
    WirelessDevice::Capabilities wirelessCapabilities() const;

    /*!
     * Helper method to convert wire representation of operation mode to enum
     */
    static WirelessDevice::OperationMode convertOperationMode(uint);
    /*!
     * Helper method to convert wire representation of capabilities to enum
     */
    static WirelessDevice::Capabilities convertCapabilities(uint);
    /*!
     * Finds access point object given its Unique Network Identifier.
     *
     * \a uni the identifier of the AP to find from this network interface
     * Returns a valid AccessPoint object if a network having the given UNI for this device is known to the system, 0 otherwise
     */
    AccessPoint::Ptr findAccessPoint(const QString &uni);

    /*!
     * Return the current list of networks
     */
    WirelessNetwork::List networks() const;

    /*!
     * Find a network with the given \a ssid, a Null object is
     * returned if it can not be found
     */
    WirelessNetwork::Ptr findNetwork(const QString &ssid) const;

Q_SIGNALS:
    /*!
     * This signal is emitted when the bitrate of this network has changed.
     *
     * \a bitrate the new bitrate value for this network
     */
    void bitRateChanged(int bitrate);
    /*!
     * The active network changed.
     */
    void activeAccessPointChanged(const QString &);
    /*!
     * The device switched operating mode.
     */
    void modeChanged(WirelessDevice::OperationMode);
    /*!
     * The device changed its capabilities
     */
    void wirelessCapabilitiesChanged(Capabilities);
    /*!
     * The device changed its hardware address
     */
    void hardwareAddressChanged(const QString &);
    /*!
     * The device changed its permanent hardware address
     */
    void permanentHardwareAddressChanged(const QString &);
    /*!
     * The device changed its properties
     */
    void wirelessPropertiesChanged(uint); // TODO this is bogus, remove
    /*!
     * A new wireless access point appeared
     */
    void accessPointAppeared(const QString &uni);
    /*!
     * A wireless access point disappeared
     */
    void accessPointDisappeared(const QString &uni);
    /*!
     * A wireless network appeared
     */
    void networkAppeared(const QString &ssid);
    /*!
     * A wireless network disappeared
     */
    void networkDisappeared(const QString &ssid);
    /*!
     * The LastScan property has changed, meaning a scan has just finished
     * \since 5.62.0
     * \
ote will never be emitted when runtime NM < 1.12
     * \sa lastScanTime
     */
    void lastScanChanged(const QDateTime &dateTime);

private:
    Q_DECLARE_PRIVATE(WirelessDevice)
};

} // namespace NetworkManager
#endif // NETWORKMANAGERQT_WIRELESSDEVICE_H
