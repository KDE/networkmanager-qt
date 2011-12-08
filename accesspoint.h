/*
Copyright 2008 Will Stephenson <wstephenson@kde.org>

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

#ifndef NM_ACCESSPOINT_H
#define NM_ACCESSPOINT_H

#include "wirelessdevice.h"
#include "QtNetworkManager-export.h"

namespace NetworkManager {

class AccessPointPrivate;

class NMQT_EXPORT AccessPoint : public QObject
{
Q_OBJECT
Q_DECLARE_PRIVATE(AccessPoint)
Q_FLAGS(Capabilities WpaFlags)
public:
    /**
     * General capabilities of an access point
     */
    enum Capability { Privacy = 0x1 };
    /**
     * Flags describing the access point's capabilities according to WPA (Wifi Protected Access)
     */
    enum WpaFlag { PairWep40 = 0x1, PairWep104 = 0x2, PairTkip = 0x4, PairCcmp = 0x8,
           GroupWep40 = 0x10, GroupWep104 = 0x20, GroupTkip = 0x40, GroupCcmp = 0x80,
           KeyMgmtPsk = 0x100, KeyMgmt8021x = 0x200 };
    Q_DECLARE_FLAGS(Capabilities, Capability)
    Q_DECLARE_FLAGS(WpaFlags, WpaFlag)
    explicit AccessPoint( const QString & path, QObject * parent = 0 );
    virtual ~AccessPoint();

    QString uni() const;
    Capabilities capabilities() const;
    AccessPoint::WpaFlags wpaFlags() const;
    AccessPoint::WpaFlags rsnFlags() const;
    QString ssid() const;
    QByteArray rawSsid() const;
    uint frequency() const;
    QString hardwareAddress() const;
    uint maxBitRate() const;
    WirelessDevice::OperationMode mode() const;
    int signalStrength() const;
protected Q_SLOTS:
    void propertiesChanged(const QVariantMap &properties);
    Q_SIGNALS:
        /**
         * This signal is emitted when the signal strength of this network has changed.
         *
         * @param strength the new signal strength value for this network
         */
        void signalStrengthChanged(int strength);

        /**
         * This signal is emitted when the bitrate of this network has changed.
         *
         * @param bitrate the new bitrate value for this network
         */
        void bitRateChanged(int bitrate);

        /**
         * This signal is emitted when the capabilities of this network have changed.
         *
         * @param caps the new capabilities
         */
        void capabilitiesChanged(AccessPoint::Capabilities);

        /**
         * This signal is emitted when the WPA flags in use by this access point change
         *
         * @param flags the new flags
         */
        void wpaFlagsChanged(AccessPoint::WpaFlags flags);

        /**
         * This signal is emitted when the RSN(WPA2) flags in use by this access point change
         *
         * @param flags the new flags
         */
        void rsnFlagsChanged(AccessPoint::WpaFlags flags);
        /**
         * This signal is emitted when the ssid of this Access Point changes
         *
         * @param ssid the new SSID
         */
        void ssidChanged(const QString &ssid);

        /**
         * This signal is emitted when the frequency used by this Access Point changes
         *
         * @param frequency the new frequency
         */
        void frequencyChanged(uint frequency);
private:
    static AccessPoint::Capabilities convertCapabilities(int);
    static AccessPoint::WpaFlags convertWpaFlags(uint);
private:
    AccessPointPrivate * d_ptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(AccessPoint::WpaFlags)

}
#endif


