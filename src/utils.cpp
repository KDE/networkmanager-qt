/*
    Copyright 2009 Will Stephenson <wstephenson@kde.org>
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

#include "utils.h"

QHostAddress NetworkManager::ipv6AddressAsHostAddress(const QByteArray &address)
{
//     Q_ASSERT(address.size() == 16);
    Q_IPV6ADDR tmp;
    for (int i = 0; i < 16; ++i) {
        tmp[i] = address[i];
    }
    QHostAddress hostaddress(tmp);
    Q_ASSERT(hostaddress.protocol() == QAbstractSocket::IPv6Protocol);

    return hostaddress;
}

QByteArray NetworkManager::ipv6AddressFromHostAddress(const QHostAddress &address)
{
//     Q_ASSERT(address.protocol() == QAbstractSocket::IPv6Protocol);
    Q_IPV6ADDR tmp = address.toIPv6Address();
    QByteArray assembledAddress;
    for (int i = 0; i < 16; ++i) {
        assembledAddress[i] = tmp[i];
    }

    return assembledAddress;
}

QString NetworkManager::macAddressAsString(const QByteArray &ba)
{
    QStringList mac;

    for (int i = 0; i < ba.size(); ++i) {
        mac << QString("%1").arg((quint8)ba[i], 2, 16, QLatin1Char('0')).toUpper();
    }

    return mac.join(":");
}

QByteArray NetworkManager::macAddressFromString(const QString &s)
{
    QStringList macStringList = s.split(':');
//     Q_ASSERT(macStringList.size() == 6);
    QByteArray ba;
    if (!s.isEmpty()) {
        ba.resize(6);
        int i = 0;

        Q_FOREACH (const QString & macPart, macStringList) {
            ba[i++] = macPart.toUInt(0, 16);
        }
    }
    return ba;
}

bool NetworkManager::macAddressIsValid(const QString &macAddress)
{
    QRegExp macAddressCheck = QRegExp("([a-fA-F0-9][a-fA-F0-9]:){5}[0-9a-fA-F][0-9a-fA-F]");

    return macAddress.contains(macAddressCheck);
}

bool NetworkManager::macAddressIsValid(const QByteArray &macAddress)
{
    return macAddressIsValid(macAddressAsString(macAddress));
}

int NetworkManager::findChannel(int freq)
{
    int channel;
    if (freq < 2500) {
        channel = 0;
        int i = 0;
        QList<QPair<int, int> > bFreqs = getBFreqs();
        while (i < bFreqs.size()) {
            if (bFreqs.at(i).second <= freq) {
                channel = bFreqs.at(i).first;
            } else {
                break;
            }
            i++;
        }
        return channel;
    }
    channel = 0;
    int i = 0;
    QList<QPair<int, int> > aFreqs = getAFreqs();
    while (i < aFreqs.size()) {
        if (aFreqs.at(i).second <= freq) {
            channel = aFreqs.at(i).first;
        } else {
            break;
        }
        i++;
    }

    return channel;
}

NetworkManager::WirelessSetting::FrequencyBand NetworkManager::findFrequencyBand(int freq)
{
    if (freq < 2500) {
        return WirelessSetting::Bg;
    }

    return WirelessSetting::A;
}

bool NetworkManager::deviceSupportsApCiphers(NetworkManager::WirelessDevice::Capabilities interfaceCaps, NetworkManager::AccessPoint::WpaFlags apCiphers, WirelessSecurityType type)
{
    bool havePair = false;
    bool haveGroup = true;

    if (type == NetworkManager::StaticWep) {
        havePair = true;
    } else {
        if (interfaceCaps.testFlag(NetworkManager::WirelessDevice::Wep40) && apCiphers.testFlag(NetworkManager::AccessPoint::PairWep40)) {
            havePair = true;
        }
        if (interfaceCaps.testFlag(NetworkManager::WirelessDevice::Wep104) && apCiphers.testFlag(NetworkManager::AccessPoint::PairWep104)) {
            havePair = true;
        }
        if (interfaceCaps.testFlag(NetworkManager::WirelessDevice::Tkip) && apCiphers.testFlag(NetworkManager::AccessPoint::PairTkip)) {
            havePair = true;
        }
        if (interfaceCaps.testFlag(NetworkManager::WirelessDevice::Ccmp) && apCiphers.testFlag(NetworkManager::AccessPoint::PairCcmp)) {
            havePair = true;
        }
    }

    if (interfaceCaps.testFlag(NetworkManager::WirelessDevice::Wep40) && apCiphers.testFlag(NetworkManager::AccessPoint::GroupWep40)) {
        haveGroup = true;
    }
    if (interfaceCaps.testFlag(NetworkManager::WirelessDevice::Wep104) && apCiphers.testFlag(NetworkManager::AccessPoint::GroupWep104)) {
        haveGroup = true;
    }
    if (type != StaticWep) {
        if (interfaceCaps.testFlag(NetworkManager::WirelessDevice::Tkip) && apCiphers.testFlag(NetworkManager::AccessPoint::GroupTkip)) {
            haveGroup = true;
        }
        if (interfaceCaps.testFlag(NetworkManager::WirelessDevice::Ccmp) && apCiphers.testFlag(NetworkManager::AccessPoint::GroupCcmp)) {
            haveGroup = true;
        }
    }

    return (havePair && haveGroup);
}

// Keep this in sync with NetworkManager/libnm-util/nm-utils.c:nm_utils_security_valid()
bool NetworkManager::securityIsValid(WirelessSecurityType type, NetworkManager::WirelessDevice::Capabilities interfaceCaps, bool haveAp, bool adhoc, NetworkManager::AccessPoint::Capabilities apCaps, NetworkManager::AccessPoint::WpaFlags apWpa, NetworkManager::AccessPoint::WpaFlags apRsn)
{
    bool good = true;

    //kDebug() << "type(" << type << ") interfaceCaps(" << interfaceCaps << ") haveAp(" << haveAp << ") adhoc(" << adhoc << ") apCaps(" << apCaps << ") apWpa(" << apWpa << " apRsn(" << apRsn << ")";

    if (!haveAp) {
        if (type == NoneSecurity) {
            return true;
        }
        if ((type == StaticWep)
                || ((type == DynamicWep) && !adhoc)
                || ((type == Leap) && !adhoc)) {
            if (interfaceCaps.testFlag(NetworkManager::WirelessDevice::Wep40) ||
                    interfaceCaps.testFlag(NetworkManager::WirelessDevice::Wep104)) {
                return true;
            }
        }

        // apCaps.testFlag(Privacy) == true for StaticWep, Leap and DynamicWep
        // see libs/internals/wirelessinterfaceconnectionhelpers.cpp

        // TODO: this is not in nm-utils.c
//         if (type == Knm::WirelessSecurity::WpaPsk
//                 || ((type == Knm::WirelessSecurity::WpaEap) && !adhoc)) {
//             if (interfaceCaps.testFlag(NetworkManager::WirelessDevice::Wpa) &&
//                 !apCaps.testFlag(NetworkManager::AccessPoint::Privacy)) {
//                 return true;
//             }
//         }
//         if (type == Knm::WirelessSecurity::Wpa2Psk
//                 || ((type == Knm::WirelessSecurity::Wpa2Eap) && !adhoc)) {
//             if (interfaceCaps.testFlag(NetworkManager::WirelessDevice::Rsn) &&
//                 !apCaps.testFlag(NetworkManager::AccessPoint::Privacy)) {
//                 return true;
//             }
//         }
        return false;
    }

    switch (type) {
    case NoneSecurity:
        Q_ASSERT(haveAp);
        if (apCaps.testFlag(NetworkManager::AccessPoint::Privacy)) {
            return false;
        }
        if (apWpa || apRsn) {
            return false;
        }
        break;
    case Leap: /* require PRIVACY bit for LEAP? */
        if (adhoc) {
            return false;
        }
    /* Fall through */
    case StaticWep:
        Q_ASSERT(haveAp);
        if (!apCaps.testFlag(NetworkManager::AccessPoint::Privacy)) {
            return false;
        }
        if (apWpa || apRsn) {
            if (!deviceSupportsApCiphers(interfaceCaps, apWpa, StaticWep))
                if (!deviceSupportsApCiphers(interfaceCaps, apRsn, StaticWep)) {
                    return false;
                }
        }
        break;
    case DynamicWep:
        if (adhoc) {
            return false;
        }
        Q_ASSERT(haveAp);
        if (apRsn || !(apCaps.testFlag(NetworkManager::AccessPoint::Privacy))) {
            return false;
        }
        /* Some APs broadcast minimal WPA-enabled beacons that must be handled */
        if (apWpa) {
            if (!apWpa.testFlag(NetworkManager::AccessPoint::KeyMgmt8021x)) {
                return false;
            }
            if (!deviceSupportsApCiphers(interfaceCaps, apWpa, DynamicWep)) {
                return false;
            }
        }
        break;
    case WpaPsk:
        if (!interfaceCaps.testFlag(NetworkManager::WirelessDevice::Wpa)) {
            return false;
        }
        if (haveAp) {
            /* Ad-Hoc WPA APs won't necessarily have the PSK flag set */
            if (adhoc) {
                if (apWpa.testFlag(NetworkManager::AccessPoint::GroupTkip) &&
                        interfaceCaps.testFlag(NetworkManager::WirelessDevice::Tkip)) {
                    return true;
                }
                if (apWpa.testFlag(NetworkManager::AccessPoint::GroupCcmp) &&
                        interfaceCaps.testFlag(NetworkManager::WirelessDevice::Ccmp)) {
                    return true;
                }
            } else if (apWpa.testFlag(NetworkManager::AccessPoint::KeyMgmtPsk)) {
                if (apWpa.testFlag(NetworkManager::AccessPoint::PairTkip) &&
                        interfaceCaps.testFlag(NetworkManager::WirelessDevice::Tkip)) {
                    return true;
                }
                if (apWpa.testFlag(NetworkManager::AccessPoint::PairCcmp) &&
                        interfaceCaps.testFlag(NetworkManager::WirelessDevice::Ccmp)) {
                    return true;
                }
            }
            return false;
        }
        break;
    case Wpa2Psk:
        if (!interfaceCaps.testFlag(NetworkManager::WirelessDevice::Rsn)) {
            return false;
        }
        if (haveAp) {
            /* Ad-Hoc WPA APs won't necessarily have the PSK flag set */
            if (apRsn.testFlag(NetworkManager::AccessPoint::KeyMgmtPsk) || adhoc) {
                if (apRsn.testFlag(NetworkManager::AccessPoint::PairTkip) &&
                        interfaceCaps.testFlag(NetworkManager::WirelessDevice::Tkip)) {
                    return true;
                }
                if (apRsn.testFlag(NetworkManager::AccessPoint::PairCcmp) &&
                        interfaceCaps.testFlag(NetworkManager::WirelessDevice::Ccmp)) {
                    return true;
                }
            }
            return false;
        }
        break;
    case WpaEap:
        if (adhoc) {
            return false;
        }
        if (!interfaceCaps.testFlag(NetworkManager::WirelessDevice::Wpa)) {
            return false;
        }
        if (haveAp) {
            if (!apWpa.testFlag(NetworkManager::AccessPoint::KeyMgmt8021x)) {
                return false;
            }
            /* Ensure at least one WPA cipher is supported */
            if (!deviceSupportsApCiphers(interfaceCaps, apWpa, WpaEap)) {
                return false;
            }
        }
        break;
    case Wpa2Eap:
        if (adhoc) {
            return false;
        }
        if (!interfaceCaps.testFlag(NetworkManager::WirelessDevice::Rsn)) {
            return false;
        }
        if (haveAp) {
            if (!apRsn.testFlag(NetworkManager::AccessPoint::KeyMgmt8021x)) {
                return false;
            }
            /* Ensure at least one WPA cipher is supported */
            if (!deviceSupportsApCiphers(interfaceCaps, apRsn, Wpa2Eap)) {
                return false;
            }
        }
        break;
    default:
        good = false;
        break;
    }

    return good;
}

NetworkManager::WirelessSecurityType NetworkManager::findBestWirelessSecurity(NetworkManager::WirelessDevice::Capabilities interfaceCaps, bool haveAp, bool adHoc, NetworkManager::AccessPoint::Capabilities apCaps, NetworkManager::AccessPoint::WpaFlags apWpa, NetworkManager::AccessPoint::WpaFlags apRsn)
{
    QList<NetworkManager::WirelessSecurityType> types;

    // The ordering of this list is a pragmatic combination of security level and popularity.
    // Therefore static WEP is before LEAP and Dynamic WEP because there is no way to detect
    // if an AP is capable of Dynamic WEP and showing Dynamic WEP first would confuse
    // Static WEP users.
    types << NetworkManager::Wpa2Eap << NetworkManager::Wpa2Psk << NetworkManager::WpaEap << NetworkManager::WpaPsk << NetworkManager::StaticWep << NetworkManager::DynamicWep << NetworkManager::Leap << NetworkManager::NoneSecurity;

    Q_FOREACH (NetworkManager::WirelessSecurityType type, types) {
        if (NetworkManager::securityIsValid(type, interfaceCaps, haveAp, adHoc, apCaps, apWpa, apRsn)) {
            return type;
        }
    }
    return NetworkManager::UnknownSecurity;
}

bool NetworkManager::wepKeyIsValid(const QString &key, NetworkManager::WirelessSecuritySetting::WepKeyType type)
{
    if (key.isEmpty()) {
        return false;
    }

    const int keylen = key.length();

    if (type != WirelessSecuritySetting::NotSpecified) {
        if (type == WirelessSecuritySetting::Hex) {
            if (keylen == 10 || keylen == 26) {
                /* Hex key */
                for (int i = 0; i < keylen; ++i) {
                    if (!(key.at(i).isDigit() || (key.at(i) >= 'A' && key.at(i) <= 'F') || (key.at(i) >= 'a' && key.at(i) <= 'f'))) {
                        return false;
                    }
                }
                return true;
            } else if (keylen == 5 || keylen == 13) {
                /* ASCII KEY */
                for (int i = 0; i < keylen; ++i) {
                    if (!key.at(i).isPrint()) {
                        return false;
                    }
                }
                return true;
            }

            return false;
        } else if (type == WirelessSecuritySetting::Passphrase) {
            if (!keylen || keylen > 64) {
                return false;
            }

            return true;
        }
    }

    return false;
}

bool NetworkManager::wpaPskIsValid(const QString &psk)
{
    if (psk.isEmpty()) {
        return false;
    }

    const int psklen = psk.length();

    if (psklen < 8 || psklen > 64) {
        return false;
    }

    if (psklen == 64) {
        /* Hex PSK */
        for (int i = 0; i < psklen; ++i) {
            if (!psk.at(i).isLetterOrNumber()) {
                return false;
            }
        }
    }

    return true;
}

NetworkManager::WirelessSecurityType NetworkManager::securityTypeFromConnectionSetting(const NetworkManager::ConnectionSettings::Ptr &settings)
{
    NetworkManager::WirelessSetting::Ptr wifiSetting = settings->setting(Setting::Wireless).dynamicCast<WirelessSetting>();
    if (!wifiSetting->security().isEmpty()) {
        NetworkManager::WirelessSecuritySetting::Ptr wifiSecuritySetting = settings->setting(Setting::WirelessSecurity).dynamicCast<WirelessSecuritySetting>();
        if (wifiSecuritySetting->keyMgmt() == WirelessSecuritySetting::Wep) {
            return StaticWep;
        } else if (wifiSecuritySetting->keyMgmt() == WirelessSecuritySetting::Ieee8021x) {
            if (wifiSecuritySetting->authAlg() == WirelessSecuritySetting::Leap) {
                return Leap;
            } else {
                return DynamicWep;
            }
        } else if (wifiSecuritySetting->keyMgmt() == WirelessSecuritySetting::WpaPsk) {
            if (wifiSecuritySetting->proto().contains(WirelessSecuritySetting::Rsn) && !wifiSecuritySetting->proto().contains(WirelessSecuritySetting::Wpa)) {
                return Wpa2Psk;
            }
            return WpaPsk;
        } else if (wifiSecuritySetting->keyMgmt() == WirelessSecuritySetting::WpaEap) {
            if (wifiSecuritySetting->proto().contains(WirelessSecuritySetting::Rsn) && !wifiSecuritySetting->proto().contains(WirelessSecuritySetting::Wpa)) {
                return Wpa2Eap;
            }
            return WpaEap;
        }
    }
    return NoneSecurity;
}

QList<QPair<int, int> > NetworkManager::getBFreqs()
{
    QList<QPair<int, int> > freqs;

    freqs.append(QPair<int, int>(1, 2412));
    freqs.append(QPair<int, int>(2, 2417));
    freqs.append(QPair<int, int>(3, 2422));
    freqs.append(QPair<int, int>(4, 2427));
    freqs.append(QPair<int, int>(5, 2432));
    freqs.append(QPair<int, int>(6, 2437));
    freqs.append(QPair<int, int>(7, 2442));
    freqs.append(QPair<int, int>(8, 2447));
    freqs.append(QPair<int, int>(9, 2452));
    freqs.append(QPair<int, int>(10, 2457));
    freqs.append(QPair<int, int>(11, 2462));
    freqs.append(QPair<int, int>(12, 2467));
    freqs.append(QPair<int, int>(13, 2472));
    freqs.append(QPair<int, int>(14, 2484));

    return freqs;
}

QList<QPair<int, int> > NetworkManager::getAFreqs()
{
    QList<QPair<int, int> > freqs;

    freqs.append(QPair<int, int>(7, 5035));
    freqs.append(QPair<int, int>(8, 5040));
    freqs.append(QPair<int, int>(9, 5045));
    freqs.append(QPair<int, int>(11, 5055));
    freqs.append(QPair<int, int>(12, 5060));
    freqs.append(QPair<int, int>(16, 5080));
    freqs.append(QPair<int, int>(34, 5170));
    freqs.append(QPair<int, int>(36, 5180));
    freqs.append(QPair<int, int>(38, 5190));
    freqs.append(QPair<int, int>(40, 5200));
    freqs.append(QPair<int, int>(42, 5210));
    freqs.append(QPair<int, int>(44, 5220));
    freqs.append(QPair<int, int>(46, 5230));
    freqs.append(QPair<int, int>(48, 5240));
    freqs.append(QPair<int, int>(52, 5260));
    freqs.append(QPair<int, int>(56, 5280));
    freqs.append(QPair<int, int>(60, 5300));
    freqs.append(QPair<int, int>(64, 5320));
    freqs.append(QPair<int, int>(100, 5500));
    freqs.append(QPair<int, int>(104, 5520));
    freqs.append(QPair<int, int>(108, 5540));
    freqs.append(QPair<int, int>(112, 5560));
    freqs.append(QPair<int, int>(116, 5580));
    freqs.append(QPair<int, int>(120, 5600));
    freqs.append(QPair<int, int>(124, 5620));
    freqs.append(QPair<int, int>(128, 5640));
    freqs.append(QPair<int, int>(132, 5660));
    freqs.append(QPair<int, int>(136, 5680));
    freqs.append(QPair<int, int>(140, 5700));
    freqs.append(QPair<int, int>(149, 5745));
    freqs.append(QPair<int, int>(153, 5765));
    freqs.append(QPair<int, int>(157, 5785));
    freqs.append(QPair<int, int>(161, 5805));
    freqs.append(QPair<int, int>(165, 5825));
    freqs.append(QPair<int, int>(183, 4915));
    freqs.append(QPair<int, int>(184, 4920));
    freqs.append(QPair<int, int>(185, 4925));
    freqs.append(QPair<int, int>(187, 4935));
    freqs.append(QPair<int, int>(188, 4940));
    freqs.append(QPair<int, int>(189, 4945));
    freqs.append(QPair<int, int>(192, 4960));
    freqs.append(QPair<int, int>(196, 4980));

    return freqs;
}
