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

#ifndef NMQT_SETTINGS_802_11_WIRELESS_SECURITY_P_H
#define NMQT_SETTINGS_802_11_WIRELESS_SECURITY_P_H

#include <QtCore/QString>
#include <QtCore/QList>

namespace NetworkManager
{

class WirelessSecuritySettingPrivate
{
public:
    WirelessSecuritySettingPrivate();

    QString name;
    NetworkManager::WirelessSecuritySetting::KeyMgmt keyMgmt;
    quint32 wepTxKeyidx;
    NetworkManager::WirelessSecuritySetting::AuthAlg authAlg;
    QList<NetworkManager::WirelessSecuritySetting::WpaProtocolVersion> proto;
    QList<NetworkManager::WirelessSecuritySetting::WpaEncryptionCapabilities> pairwise;
    QList<NetworkManager::WirelessSecuritySetting::WpaEncryptionCapabilities> group;
    QString leapUsername;
    QString wepKey0;
    QString wepKey1;
    QString wepKey2;
    QString wepKey3;
    NetworkManager::Setting::SecretFlags wepKeyFlags;
    NetworkManager::WirelessSecuritySetting::WepKeyType wepKeyType;
    QString psk;
    NetworkManager::Setting::SecretFlags pskFlags;
    QString leapPassword;
    NetworkManager::Setting::SecretFlags leapPasswordFlags;
};

}

#endif // NMQT_SETTINGS_802_11_WIRELESS_SECURITY_P_H

