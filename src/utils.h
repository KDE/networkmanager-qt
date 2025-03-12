/*
    SPDX-FileCopyrightText: 2012-2013 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_UTILS_H
#define NETWORKMANAGERQT_UTILS_H

#include <QHostAddress>
#include <QPair>

#include "wirelessdevice.h"
#include "wirelesssecuritysetting.h"
#include "wirelesssetting.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

namespace NetworkManager
{
/*!
 *
 * Capabilities to pass to secret agents
 *
 * \value UnknownSecurity
 * \value NoneSecurity
 * \value StaticWep
 * \value DynamicWep
 * \value Leap
 * \value WpaPsk
 * \value WpaEap
 * \value Wpa2Psk
 * \value Wpa2Eap
 * \value SAE
 * \value Wpa3SuiteB192
 * \value OWE
 */
enum WirelessSecurityType {
    UnknownSecurity = -1,
    NoneSecurity,
    StaticWep,
    DynamicWep,
    Leap,
    WpaPsk,
    WpaEap,
    Wpa2Psk,
    Wpa2Eap,
    SAE,
    Wpa3SuiteB192,
    OWE,
};

/*!
 * Returns QHostAddress representation of an ipv6 address
 *
 * \a address byte array containing the binary representation of the address
 */
NETWORKMANAGERQT_EXPORT QHostAddress ipv6AddressAsHostAddress(const QByteArray &address);

/*!
 * Returns binary representation of an ipv6 address
 *
 * \a address qhostaddress containing the address
 */
NETWORKMANAGERQT_EXPORT QByteArray ipv6AddressFromHostAddress(const QHostAddress &address);

/*!
 * Returns String representation of a mac address.
 *
 * \a ba byte array containing the binary repesentation of the address
 */
NETWORKMANAGERQT_EXPORT QString macAddressAsString(const QByteArray &ba);

/*!
 * Returns binary repesentation of a mac address.
 *
 * \a s string representation of the address
 */
NETWORKMANAGERQT_EXPORT QByteArray macAddressFromString(const QString &s);

/*!
 */
NETWORKMANAGERQT_EXPORT bool macAddressIsValid(const QString &macAddress);
/*!
 */
NETWORKMANAGERQT_EXPORT bool macAddressIsValid(const QByteArray &macAddress);

/*!
 * \a freq frequency of a wireless network
 *
 * Returns The frequency channel.
 */
NETWORKMANAGERQT_EXPORT int findChannel(int freq);

/*!
 */
NETWORKMANAGERQT_EXPORT NetworkManager::WirelessSetting::FrequencyBand findFrequencyBand(int freq);

/*!
 */
NETWORKMANAGERQT_EXPORT bool
deviceSupportsApCiphers(NetworkManager::WirelessDevice::Capabilities, NetworkManager::AccessPoint::WpaFlags ciphers, WirelessSecurityType type);

/*!
 */
NETWORKMANAGERQT_EXPORT bool securityIsValid(WirelessSecurityType type,
                                             NetworkManager::WirelessDevice::Capabilities interfaceCaps,
                                             bool haveAp,
                                             bool adHoc,
                                             NetworkManager::AccessPoint::Capabilities apCaps,
                                             NetworkManager::AccessPoint::WpaFlags apWpa,
                                             NetworkManager::AccessPoint::WpaFlags apRsn);

/*!
 */
NETWORKMANAGERQT_EXPORT WirelessSecurityType findBestWirelessSecurity(NetworkManager::WirelessDevice::Capabilities,
                                                                      bool haveAp,
                                                                      bool adHoc,
                                                                      NetworkManager::AccessPoint::Capabilities apCaps,
                                                                      NetworkManager::AccessPoint::WpaFlags apWpa,
                                                                      NetworkManager::AccessPoint::WpaFlags apRsn);

/*!
 */
NETWORKMANAGERQT_EXPORT bool wepKeyIsValid(const QString &key, NetworkManager::WirelessSecuritySetting::WepKeyType type);

/*!
 */
NETWORKMANAGERQT_EXPORT bool wpaPskIsValid(const QString &psk);

/*!
 */
NETWORKMANAGERQT_EXPORT WirelessSecurityType securityTypeFromConnectionSetting(const NetworkManager::ConnectionSettings::Ptr &settings);

/*!
 */
NETWORKMANAGERQT_EXPORT QList<QPair<int, int>> getBFreqs();
/*!
 */
NETWORKMANAGERQT_EXPORT QList<QPair<int, int>> getAFreqs();

/*!
 */
NETWORKMANAGERQT_EXPORT QDateTime clockBootTimeToDateTime(qlonglong clockBootime);
}

#endif // NETWORKMANAGERQT_UTILS_H
