/*
    Copyright 2012-2013  Jan Grulich <jgrulich@redhat.com>

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

#ifndef NMQT_UTILS_H
#define NMQT_UTILS_H

#include <QPair>
#include <QStringList>
#include <QHostAddress>

#include "NetworkManagerQt-export.h"
#include "wirelessdevice.h"
#include "settings/wirelesssecuritysetting.h"
#include "settings/wirelesssetting.h"

namespace NetworkManager
{

class NETWORKMANAGERQT_EXPORT Utils
{
public:
    enum WirelessSecurityType { Unknown = -1, None, StaticWep, DynamicWep, Leap, WpaPsk, WpaEap, Wpa2Psk, Wpa2Eap };

    /**
     * @return QHostAddress representation of an ipv6 address
     * @param address byte array containing the binary representation of the address
     */
    static QHostAddress ipv6AddressAsHostAddress(const QByteArray &address);

    /**
     * @return binary representation of an ipv6 address
     * @param address qhostaddress containing the address
     */
    static QByteArray ipv6AddressFromHostAddress(const QHostAddress &address);

    /**
     * @return String representation of a mac address.
     * @param ba byte array containing the binary repesentation of the address
     */
    static QString macAddressAsString(const QByteArray &ba);

    /**
     * @return binary repesentation of a mac address.
     * @param s string representation of the address
     */
    static QByteArray macAddressFromString(const QString &s);

    static bool macAddressIsValid(const QString &macAddress);
    static bool macAddressIsValid(const QByteArray &macAddress);

    /**
     * @param freq frequency of a wireless network
     * @return The frequency channel.
     */
    static int findChannel(int freq);

    static NetworkManager::WirelessSetting::FrequencyBand findFrequencyBand(int freq);

    static bool deviceSupportsApCiphers(NetworkManager::WirelessDevice::Capabilities, NetworkManager::AccessPoint::WpaFlags ciphers, Utils::WirelessSecurityType type);

    static bool securityIsValid(Utils::WirelessSecurityType type, NetworkManager::WirelessDevice::Capabilities interfaceCaps, bool haveAp, bool adHoc, NetworkManager::AccessPoint::Capabilities apCaps, NetworkManager::AccessPoint::WpaFlags apWpa, NetworkManager::AccessPoint::WpaFlags apRsn);

    static Utils::WirelessSecurityType findBestWirelessSecurity(NetworkManager::WirelessDevice::Capabilities, bool haveAp, bool adHoc, NetworkManager::AccessPoint::Capabilities apCaps, NetworkManager::AccessPoint::WpaFlags apWpa, NetworkManager::AccessPoint::WpaFlags apRsn);

    static bool wepKeyIsValid(const QString &key, NetworkManager::WirelessSecuritySetting::WepKeyType type);

    static bool wpaPskIsValid(const QString &psk);

    static QList<QPair<int, int> > getBFreqs();
    static QList<QPair<int, int> > getAFreqs();
};

}

#endif // NMQT_UTILS_H
