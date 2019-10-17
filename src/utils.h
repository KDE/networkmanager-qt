/*
    Copyright 2012-2013 Jan Grulich <jgrulich@redhat.com>

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

#ifndef NETWORKMANAGERQT_UTILS_H
#define NETWORKMANAGERQT_UTILS_H

#include <QPair>
#include <QHostAddress>

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "wirelessdevice.h"
#include "wirelesssecuritysetting.h"
#include "wirelesssetting.h"

namespace NetworkManager
{
    enum WirelessSecurityType { UnknownSecurity = -1, NoneSecurity, StaticWep, DynamicWep, Leap, WpaPsk, WpaEap, Wpa2Psk, Wpa2Eap, SAE };

    /**
     * @return QHostAddress representation of an ipv6 address
     * @param address byte array containing the binary representation of the address
     */
    NETWORKMANAGERQT_EXPORT QHostAddress ipv6AddressAsHostAddress(const QByteArray &address);

    /**
     * @return binary representation of an ipv6 address
     * @param address qhostaddress containing the address
     */
    NETWORKMANAGERQT_EXPORT QByteArray ipv6AddressFromHostAddress(const QHostAddress &address);

    /**
     * @return String representation of a mac address.
     * @param ba byte array containing the binary repesentation of the address
     */
    NETWORKMANAGERQT_EXPORT QString macAddressAsString(const QByteArray &ba);

    /**
     * @return binary repesentation of a mac address.
     * @param s string representation of the address
     */
    NETWORKMANAGERQT_EXPORT QByteArray macAddressFromString(const QString &s);

    NETWORKMANAGERQT_EXPORT bool macAddressIsValid(const QString &macAddress);
    NETWORKMANAGERQT_EXPORT bool macAddressIsValid(const QByteArray &macAddress);

    /**
     * @param freq frequency of a wireless network
     * @return The frequency channel.
     */
    NETWORKMANAGERQT_EXPORT int findChannel(int freq);

    NETWORKMANAGERQT_EXPORT NetworkManager::WirelessSetting::FrequencyBand findFrequencyBand(int freq);

    NETWORKMANAGERQT_EXPORT bool deviceSupportsApCiphers(NetworkManager::WirelessDevice::Capabilities, NetworkManager::AccessPoint::WpaFlags ciphers, WirelessSecurityType type);

    NETWORKMANAGERQT_EXPORT bool securityIsValid(WirelessSecurityType type, NetworkManager::WirelessDevice::Capabilities interfaceCaps, bool haveAp, bool adHoc, NetworkManager::AccessPoint::Capabilities apCaps, NetworkManager::AccessPoint::WpaFlags apWpa, NetworkManager::AccessPoint::WpaFlags apRsn);

    NETWORKMANAGERQT_EXPORT WirelessSecurityType findBestWirelessSecurity(NetworkManager::WirelessDevice::Capabilities, bool haveAp, bool adHoc, NetworkManager::AccessPoint::Capabilities apCaps, NetworkManager::AccessPoint::WpaFlags apWpa, NetworkManager::AccessPoint::WpaFlags apRsn);

    NETWORKMANAGERQT_EXPORT bool wepKeyIsValid(const QString &key, NetworkManager::WirelessSecuritySetting::WepKeyType type);

    NETWORKMANAGERQT_EXPORT bool wpaPskIsValid(const QString &psk);

    NETWORKMANAGERQT_EXPORT WirelessSecurityType securityTypeFromConnectionSetting(const NetworkManager::ConnectionSettings::Ptr &settings);

    NETWORKMANAGERQT_EXPORT QList<QPair<int, int> > getBFreqs();
    NETWORKMANAGERQT_EXPORT QList<QPair<int, int> > getAFreqs();

    NETWORKMANAGERQT_EXPORT QDateTime clockBootTimeToDateTime(qlonglong clockBootime);
}

#endif // NETWORKMANAGERQT_UTILS_H
