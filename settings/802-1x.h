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

#ifndef NMQT_SECURITY8021XSETTING_H
#define NMQT_SECURITY8021XSETTING_H

#include "../QtNetworkManager-export.h"
#include "setting.h"

namespace NetworkManager
{
namespace Settings
{
    class Security8021xSettingPrivate;

    class NMQT_EXPORT Security8021xSetting : public Setting
    {
    Q_DECLARE_PRIVATE(Security8021xSetting)
    public:
        enum EapMethod {EapMethodUnknown = 0, EapMethodLeap, EapMethodMd5, EapMethodTls, EapMethodPeap, EapMethodTtls, EapMethodSim, EapMethodFast};
        enum PeapVersion {PeapVersionUnknown = -1, PeapVersionZero, PeapVersionOne};
        enum PeapLabel {PeapLabelUnknown = 0, PeapLabelForce};
        enum FastProvisioning {FastProvisioningUnknown = -1, FastProvisioningDisabled, FastProvisioningAllowUnauthenticated, FastProvisioningAllowAuthenticated, FastProvisioningAllowBoth};
        enum AuthMethod {AuthMethodUnknown = 0, AuthMethodPap, AuthMethodChap, AuthMethodMschap, AuthMethodMschapv2,
                         AuthMethodGtc, AuthMethodOtp, AuthMethodMd5, AuthMethodTls};
        enum AuthEapMethod {AuthEapMethodUnknown = 0, AuthEapMethodMd5, AuthEapMethodMschapv2,
                            AuthEapMethodOtp, AuthEapMethodGtc, AuthEapMethodTls};

        Security8021xSetting();
        Security8021xSetting(Security8021xSetting* setting);
        ~Security8021xSetting();

        QString name() const;

        void setEapMethods(const QList<EapMethod> & methods);
        QList<EapMethod> eapMethods() const;

        void setIdentity(const QString & identity);
        QString identity() const;

        void setAnonymousIdentity(const QString & identity);
        QString anonymousIdentity() const;

        void setPacFile(const QString & file);
        QString pacFile() const;

        void setCaCertificate(const QByteArray & certificate);
        QByteArray caCertificate() const;

        void setCaPath(const QString & path);
        QString caPath() const;

        void setSubjectMatch(const QString & substring);
        QString subjectMatch() const;

        void setAltSubjectMatches(const QStringList & strings);
        QStringList altSubjectMatches() const;

        void setClientCertificate(const QByteArray & certificate);
        QByteArray clientCertificate() const;

        void setPhase1PeapVersion(PeapVersion version);
        PeapVersion phase1PeapVersion() const;

        void setPhase1PeapLabel(PeapLabel label);
        PeapLabel phase1PeapLabel() const;

        void setPhase1FastProvisioning(FastProvisioning provisioning);
        FastProvisioning phase1FastProvisioning() const;

        void setPhase2AuthMethod(AuthMethod method);
        AuthMethod phase2AuthMethod() const;

        void setPhase2AuthEapMethod(AuthEapMethod method);
        AuthEapMethod phase2AuthEapMethod() const;

        void setPhase2CaCertificate(const QByteArray & certificate);
        QByteArray phase2CaCertificate() const;

        void setPhase2CaPath(const QString & path);
        QString phase2CaPath() const;

        void setPhase2SubjectMatch(const QString & substring);
        QString phase2SubjectMatch() const;

        void setPhase2AltSubjectMatches(const QStringList & strings);
        QStringList phase2AltSubjectMatches() const;

        void setPhase2ClientCertificate(const QByteArray & certificate);
        QByteArray phase2ClientCertificate() const;

        void setPassword(const QString & password);
        QString password() const;

        void setPasswordFlags(Setting::SecretFlagType flags);
        Setting::SecretFlagType passwordFlags() const;

        void setPasswordRaw(const QByteArray & password);
        QByteArray passwordRaw() const;

        void setPasswordRawFlags(Setting::SecretFlagType flags);
        Setting::SecretFlagType passwordRawFlags() const;

        void setPrivateKey(const QByteArray & key);
        QByteArray privateKey() const;

        void setPrivateKeyPassword(const QString & password);
        QString privateKeyPassword() const;

        void setPrivateKeyPasswordFlags(Setting::SecretFlagType flags);
        Setting::SecretFlagType privateKeyPasswordFlags() const;

        void setPhase2PrivateKey(const QByteArray & key);
        QByteArray phase2PrivateKey() const;

        void setPhase2PrivateKeyPassword(const QString & password);
        QString phase2PrivateKeyPassword() const;

        void setPhase2PrivateKeyPasswordFlags(Setting::SecretFlagType flags);
        Setting::SecretFlagType phase2PrivateKeyPasswordFlags() const;

        void setPin(const QString & pin);
        QString pin() const;

        void setPinFlags(Setting::SecretFlagType flags);
        Setting::SecretFlagType pinFlags() const;

        void setSystemCaCertificates(bool use);
        bool systemCaCertificates() const;

        QStringList needSecrets() const;

        void secretsFromMap(const QVariantMap & secrets);

        QVariantMap secretsToMap() const;

        void fromMap(const QVariantMap & setting);

        QVariantMap toMap() const;

        //FOR DEBUG
        void printSetting();
    protected:
        Security8021xSettingPrivate *d_ptr;
    };
}
}

#endif // NMQT_SECURITY8021XSETTING_H
