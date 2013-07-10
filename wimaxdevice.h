/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
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

#ifndef NMQT_WIMAXDEVICE_H
#define NMQT_WIMAXDEVICE_H

#include "NetworkManagerQt-export.h"

#include "device.h"
#include "wimaxnsp.h"

#include <QDBusObjectPath>

namespace NetworkManager
{

class WimaxDevicePrivate;

/**
 * A wireless network interface
 */
class NETWORKMANAGERQT_EXPORT WimaxDevice : public Device
{
    Q_OBJECT

public:
    typedef QSharedPointer<WimaxDevice> Ptr;
    typedef QList<Ptr> List;

    /**
     * Creates a new WimaxDevice object.
     *
     * @param path the DBus path of the device
     */
    explicit WimaxDevice(const QString &path, QObject *parent = 0);
    /**
     * Destroys a WimaxDevice object.
     */
    ~WimaxDevice();
    /**
     * Return the type
     */
    virtual Type type() const;
    /**
     * List of network service providers currently visible to the hardware
     */
    QStringList nsps() const;
    /**
     * Identifier of the NSP this interface is currently associated with
     */
    WimaxNsp::Ptr activeNsp() const;
    /**
     * The ID of the serving base station as received from the network.
     */
    QString bsid() const;
    /**
     * The hardware address currently used by the network interface
     */
    QString hardwareAddress() const;
    /**
     * Center frequency (in KHz) of the radio channel the device is using to communicate with the network when connected.
     */
    uint centerFrequency() const;
    /**
     * CINR (Carrier to Interference + Noise Ratio) of the current radio link in dB.
     */
    int cinr() const;
    /**
     * RSSI of the current radio link in dBm. This value indicates how strong the raw received RF signal from the base station is, but does not indicate the overall quality of the radio link.
     */
    int rssi() const;
    /**
     * Average power of the last burst transmitted by the device, in units of 0.5 dBm. i.e. a TxPower of -11 represents an actual device TX power of -5.5 dBm.
     */
    int txPower() const;

    /**
     * Finds NSP object given its Unique Network Identifier.
     *
     * @param uni the identifier of the AP to find from this network interface
     * @returns a valid WimaxNsp object if a network having the given UNI for this device is known to the system, 0 otherwise
     */
    NetworkManager::WimaxNsp::Ptr findNsp(const QString &uni) const;

Q_SIGNALS:
    /**
     * This signal is emitted when the bitrate of this network has changed.
     *
     * @param bitrate the new bitrate value for this network
     */
    void bitRateChanged(int bitrate);
    /**
     * The active NSP changed.
     */
    void activeNspChanged(const QString &);
    /**
     * The BSID changed.
     */
    void bsidChanged(const QString &);
    /**
     * The device changed its hardware address
     */
    void hardwareAddressChanged(const QString &);
    /**
     * The device changed its center frequency
     */
    void centerFrequencyChanged(uint);
    /**
     * The device changed its signal/noise ratio
     */
    void cinrChanged(int);
    /**
     * The device changed its RSSI
     */
    void rssiChanged(int);
    /**
     * The device changed its TxPower.
     */
    void txPowerChanged(int);
    /**
     * A new NSP appeared
     */
    void nspAppeared(const QString &);
    /**
     * A wireless access point disappeared
     */
    void nspDisappeared(const QString &);

protected Q_SLOTS:
    void nspAdded(const QDBusObjectPath &);
    void nspRemoved(const QDBusObjectPath &);

protected:
    /**
     * When subclassing make sure to call the parent class method
     * if the property was not useful to your new class
     */
    virtual void propertyChanged(const QString &property, const QVariant &value);

private:
    Q_DECLARE_PRIVATE(WimaxDevice)
};

} // namespace NetworkManager
#endif //NMQT_WIMAXDEVICE_H

