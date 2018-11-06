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

#ifndef NETWORKMANAGERQT_SECURITY8021X_SETTING_P_H
#define NETWORKMANAGERQT_SECURITY8021X_SETTING_P_H

#include <QStringList>

namespace NetworkManager
{

class Security8021xSettingPrivate
{
public:
    Security8021xSettingPrivate();

    QString name;
    QList<Security8021xSetting::EapMethod> eap;
    QString identity;
    QString anonymousIdentity;
    QString domainSuffixMatch;
    QString pacFile;
    QByteArray caCert;
    QString caPath;
    QString subjectMatch;
    QStringList altSubjectMatches;
    QByteArray clientCert;
    Security8021xSetting::PeapVersion phase1PeapVer;
    Security8021xSetting::PeapLabel phase1PeapLabel;
    Security8021xSetting::FastProvisioning phase1FastProvisioning;
    Security8021xSetting::AuthMethod phase2AuthMethod;
    Security8021xSetting::AuthEapMethod phase2AuthEapMethod;
    QByteArray phase2CaCert;
    QString phase2CaPath;
    QString phase2SubjectMatch;
    QStringList phase2AltSubjectMatches;
    QByteArray phase2ClientCert;
    QString password;
    Setting::SecretFlags passwordFlags;
    QByteArray passwordRaw;
    Setting::SecretFlags passwordRawFlags;
    QByteArray privateKey;
    QString privateKeyPassword;
    Setting::SecretFlags privateKeyPasswordFlags;
    QByteArray phase2PrivateKey;
    QString phase2PrivateKeyPassword;
    Setting::SecretFlags phase2PrivateKeyPasswordFlags;
    QString pin;
    Setting::SecretFlags pinFlags;
    bool systemCaCerts;
};

}

#endif // NETWORKMANAGERQT_SECURITY802X_SETTING_P_H
