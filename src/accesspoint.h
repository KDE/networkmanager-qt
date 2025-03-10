/*
    SPDX-FileCopyrightText: 2008 Will Stephenson <wstephenson@kde.org>
    SPDX-FileCopyrightText: 2011-2013 Lamarque V. Souza <lamarque@kde.org>
    SPDX-FileCopyrightText: 2013 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_ACCESSPOINT_H
#define NETWORKMANAGERQT_ACCESSPOINT_H

#include <networkmanagerqt/networkmanagerqt_export.h>

#include <nm-version.h>

#include <QObject>
#include <QSharedPointer>
#include <QVariantMap>

namespace NetworkManager
{
class AccessPointPrivate;

/*!
 * \class NetworkManager::AccessPoint
 * \inheaderfile NetworkManagerQt/AccessPoint
 * \inmodule NetworkManagerQt
 *
 * \brief Represents an access point.
 */
class NETWORKMANAGERQT_EXPORT AccessPoint : public QObject
{
    Q_OBJECT
public:
    /*!
     * \typedef NetworkManager::AccessPoint::Ptr
     */
    typedef QSharedPointer<AccessPoint> Ptr;
    /*!
     * \typedef NetworkManager::AccessPoint::List
     */
    typedef QList<Ptr> List;
    /*!
     *
     * The access point's current operating mode
     *
     * \value Unknown
     *        Not associated with a network
     * \value Adhoc
     *        A station in an infrastructure wireless network
     * \value ApMode
     *        Access point in an infrastructure network
     */
    enum OperationMode {
        Unknown = 0,
        Adhoc,
        Infra,
        ApMode,
    };
    /*!
     *
     * General capabilities of an access point
     *
     * \value None
     *        Null capability - says nothing about the access point
     * \value Privacy
     *        Access point supports privacy measures
     */
    enum Capability {
        None = 0x0,
        Privacy = 0x1,
    };
    /*!
     *
     * Flags describing the access point's capabilities according to WPA (Wifi Protected Access)
     *
     * \value PairWep40
     * \value PairWep104
     * \value PairTkip
     * \value PairCcmp
     * \value GroupWep40
     * \value GroupWep104
     * \value GroupTkip
     * \value GroupCcmp
     * \value KeyMgmtPsk
     * \value KeyMgmt8021x
     * \value KeyMgmtSAE
     * \value KeyMgmtOWE
     * \value KeyMgmtOWETM
     * \value KeyMgmtEapSuiteB192
     */
    enum WpaFlag {
        PairWep40 = 0x1,
        PairWep104 = 0x2,
        PairTkip = 0x4,
        PairCcmp = 0x8,
        GroupWep40 = 0x10,
        GroupWep104 = 0x20,
        GroupTkip = 0x40,
        GroupCcmp = 0x80,
        KeyMgmtPsk = 0x100,
        KeyMgmt8021x = 0x200,
        KeyMgmtSAE = 0x400,
        KeyMgmtOWE = 0x800,
        KeyMgmtOWETM = 0x1000,
        KeyMgmtEapSuiteB192 = 0x2000,
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)
    Q_FLAG(Capabilities)
    Q_DECLARE_FLAGS(WpaFlags, WpaFlag)
    Q_FLAG(WpaFlags)
    /*!
     */
    explicit AccessPoint(const QString &path, QObject *parent = nullptr);
    ~AccessPoint() override;

    /*!
     * \brief Returns path of the access point.
     */
    QString uni() const;
    /*!
     * \brief Returns capabilities of an access point.
     */
    Capabilities capabilities() const;
    /*!
     * \brief Returns flags describing the access point's capabilities according to WPA (Wifi Protected Access).
     * \sa WpaFlag
     */
    AccessPoint::WpaFlags wpaFlags() const;
    /*!
     * \brief Returns Flags describing the access point's capabilities according to the RSN (Robust Secure Network) protocol.
     * \sa WpaFlag
     */
    AccessPoint::WpaFlags rsnFlags() const;
    /*!
     * \brief Returns The Service Set Identifier identifying the access point.
     */
    QString ssid() const;
    /*!
     * \brief Returns raw SSID, encoded as a byte array.
     */
    QByteArray rawSsid() const;
    /*!
     * \brief Returns The radio channel frequency in use by the access point, in MHz.
     */
    uint frequency() const;
    /*!
     * \brief Returns The hardware address (BSSID) of the access point.
     */
    QString hardwareAddress() const;
    /*!
     * \brief Returns The maximum bitrate this access point is capable of, in kilobits/second (Kb/s).
     */
    uint maxBitRate() const;
    /*!
     * \brief Returns Describes the operating mode of the access point.
     */
    OperationMode mode() const;
    /*!
     * \brief Returns The current signal quality of the access point, in percent.
     */
    int signalStrength() const;
    /*!
     * \brief Returns The timestamp (in CLOCK_BOOTTIME seconds) for the last time the access point
     * was found in scan results. A value of -1 means the access point has never been found in scan results.
     * \since 5.14.0
     */
    int lastSeen() const;
    /*!
     * The bandwidth announced by the access point in MHz.
     * \since 6.12.0.
     */
    uint bandwidth() const;
    /*!
     * \brief Helper method to convert wire representation of operation \a mode to enum.
     */
    static OperationMode convertOperationMode(uint mode);

Q_SIGNALS:
    /*!
     * \brief This signal is emitted when the signal strength of this network has changed.
     *
     * \a strength the new signal strength value for this network
     */
    void signalStrengthChanged(int strength);

    /*!
     * \brief This signal is emitted when the bitrate of this network has changed.
     *
     * \a bitrate the new bitrate value for this network
     */
    void bitRateChanged(int bitrate);

    /*!
     * \brief This signal is emitted when the capabilities of this network have changed.
     *
     * \a caps the new capabilities
     */
    void capabilitiesChanged(AccessPoint::Capabilities caps);

    /*!
     * \brief This signal is emitted when the WPA flags in use by this access point change.
     *
     * \a flags the new flags
     */
    void wpaFlagsChanged(AccessPoint::WpaFlags flags);

    /*!
     * \brief This signal is emitted when the RSN(WPA2) flags in use by this access point change.
     *
     * \a flags the new flags
     */
    void rsnFlagsChanged(AccessPoint::WpaFlags flags);
    /*!
     * \brief This signal is emitted when the ssid of this Access Point changes.
     *
     * \a ssid the new SSID
     */
    void ssidChanged(const QString &ssid);

    /*!
     * \brief This signal is emitted when the frequency used by this Access Point changes.
     *
     * \a frequency the new frequency
     */
    void frequencyChanged(uint frequency);

    /*!
     * \brief This signal is emitted when the timestamp for the last time the access point was found
     * in scan results changes.
     *
     * \a lastSeen the timestamp for the last time the access point was found in scan results.
     * \since 5.14.0
     */
    void lastSeenChanged(int lastSeen);

    /*!
     * This signal is emitted when bandwidth announced by the access point changes.
     *
     * \a lastSeen the bandwidth announced by the access point in MHz.
     * \since 6.12.0
     */
    void bandwidthChanged(uint bandwidth);

private:
    Q_DECLARE_PRIVATE(AccessPoint)

    AccessPointPrivate *const d_ptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(AccessPoint::WpaFlags)

}
#endif
