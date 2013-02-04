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

#include "802-1x.h"
#include "802-1x_p.h"

#include <nm-setting-8021x.h>

#include <QtCore/QDebug>

NetworkManager::Settings::Security8021xSettingPrivate::Security8021xSettingPrivate():
    name(QString("802-1x")),
    eap(QList<Security8021xSetting::EapMethod>()),
    identity(QString()),
    anonymousIdentity(QString()),
    pacFile(QString()),
    caCert(QByteArray()),
    caPath(QString()),
    subjectMatch(QString()),
    altSubjectMatches(QStringList()),
    clientCert(QByteArray()),
    phase1PeapVer(Security8021xSetting::PeapVersionUnknown),
    phase1PeapLabel(Security8021xSetting::PeapLabelUnknown),
    phase1FastProvisioning(Security8021xSetting::FastProvisioningUnknown),
    phase2AuthMethod(Security8021xSetting::AuthMethodUnknown),
    phase2AuthEapMethod(Security8021xSetting::AuthEapMethodUnknown),
    phase2CaCert(QByteArray()),
    phase2CaPath(QString()),
    phase2SubjectMatch(QString()),
    phase2AltSubjectMatches(QStringList()),
    phase2ClientCert(QByteArray()),
    password(QString()),
    passwordFlags(NetworkManager::Settings::Setting::None),
    passwordRaw(QByteArray()),
    passwordRawFlags(NetworkManager::Settings::Setting::None),
    privateKey(QByteArray()),
    privateKeyPassword(QByteArray()),
    privateKeyPasswordFlags(NetworkManager::Settings::Setting::None),
    phase2PrivateKey(QByteArray()),
    phase2PrivateKeyPassword(QString()),
    phase2PrivateKeyPasswordFlags(NetworkManager::Settings::Setting::None),
    pin(QString()),
    pinFlags(NetworkManager::Settings::Setting::None),
    systemCaCerts(false)
{ }

NetworkManager::Settings::Security8021xSetting::Security8021xSetting():
    Setting(Setting::Security8021x),
    d_ptr(new Security8021xSettingPrivate())
{ }

NetworkManager::Settings::Security8021xSetting::Security8021xSetting(Security8021xSetting * setting):
    Setting(setting),
    d_ptr(new Security8021xSettingPrivate())
{
    setEapMethods(setting->eapMethods());
    setIdentity(setting->identity());
    setAnonymousIdentity(setting->anonymousIdentity());
    setPacFile(setting->pacFile());
    setCaCertificate(setting->caCertificate());
    setCaPath(setting->caPath());
    setSubjectMatch(setting->subjectMatch());
    setAltSubjectMatches(setting->altSubjectMatches());
    setClientCertificate(setting->clientCertificate());
    setPhase1PeapVersion(setting->phase1PeapVersion());
    setPhase1PeapLabel(setting->phase1PeapLabel());
    setPhase1FastProvisioning(setting->phase1FastProvisioning());
    setPhase2AuthMethod(setting->phase2AuthMethod());
    setPhase2AuthEapMethod(setting->phase2AuthEapMethod());
    setPhase2CaCertificate(setting->phase2CaCertificate());
    setPhase2CaPath(setting->phase2CaPath());
    setPhase2SubjectMatch(setting->phase2SubjectMatch());
    setPhase2AltSubjectMatches(setting->phase2AltSubjectMatches());
    setPassword(setting->password());
    setPasswordFlags(setting->passwordFlags());
    setPasswordRaw(setting->passwordRaw());
    setPasswordRawFlags(setting->passwordRawFlags());
    setPrivateKey(setting->privateKey());
    setPrivateKeyPassword(setting->privateKeyPassword());
    setPrivateKeyPasswordFlags(setting->privateKeyPasswordFlags());
    setPhase2PrivateKey(setting->phase2PrivateKey());
    setPhase2PrivateKeyPassword(setting->phase2PrivateKeyPassword());
    setPhase2PrivateKeyPasswordFlags(setting->phase2PrivateKeyPasswordFlags());
    setSystemCaCertificates(setting->systemCaCertificates());
}

NetworkManager::Settings::Security8021xSetting::~Security8021xSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::Security8021xSetting::name() const
{
    Q_D(const Security8021xSetting);

    return d->name;
}

void NetworkManager::Settings::Security8021xSetting::setEapMethods(const QList< NetworkManager::Settings::Security8021xSetting::EapMethod >& methods)
{
    Q_D(Security8021xSetting);

    d->eap = methods;
}

QList< NetworkManager::Settings::Security8021xSetting::EapMethod > NetworkManager::Settings::Security8021xSetting::eapMethods() const
{
    Q_D(const Security8021xSetting);

    return d->eap;
}

void NetworkManager::Settings::Security8021xSetting::setIdentity(const QString& identity)
{
    Q_D(Security8021xSetting);

    d->identity = identity;
}

QString NetworkManager::Settings::Security8021xSetting::identity() const
{
    Q_D(const Security8021xSetting);

    return d->identity;
}

void NetworkManager::Settings::Security8021xSetting::setAnonymousIdentity(const QString& identity)
{
    Q_D(Security8021xSetting);

    d->anonymousIdentity = identity;
}

QString NetworkManager::Settings::Security8021xSetting::anonymousIdentity() const
{
    Q_D(const Security8021xSetting);

    return d->anonymousIdentity;
}

void NetworkManager::Settings::Security8021xSetting::setPacFile(const QString& file)
{
    Q_D(Security8021xSetting);

    d->pacFile = file;
}

QString NetworkManager::Settings::Security8021xSetting::pacFile() const
{
    Q_D(const Security8021xSetting);

    return d->pacFile;
}

void NetworkManager::Settings::Security8021xSetting::setCaCertificate(const QByteArray& certificate)
{
    Q_D(Security8021xSetting);

    d->caCert = certificate;
}

QByteArray NetworkManager::Settings::Security8021xSetting::caCertificate() const
{
    Q_D(const Security8021xSetting);

    return d->caCert;
}

void NetworkManager::Settings::Security8021xSetting::setCaPath(const QString& path)
{
    Q_D(Security8021xSetting);

    d->caPath = path;
}

QString NetworkManager::Settings::Security8021xSetting::caPath() const
{
    Q_D(const Security8021xSetting);

    return d->caPath;
}

void NetworkManager::Settings::Security8021xSetting::setSubjectMatch(const QString& substring)
{
    Q_D(Security8021xSetting);

    d->subjectMatch = substring;
}

QString NetworkManager::Settings::Security8021xSetting::subjectMatch() const
{
    Q_D(const Security8021xSetting);

    return d->subjectMatch;
}

void NetworkManager::Settings::Security8021xSetting::setAltSubjectMatches(const QStringList& strings)
{
    Q_D(Security8021xSetting);

    d->altSubjectMatches = strings;
}

QStringList NetworkManager::Settings::Security8021xSetting::altSubjectMatches() const
{
    Q_D(const Security8021xSetting);

    return d->altSubjectMatches;
}

void NetworkManager::Settings::Security8021xSetting::setClientCertificate(const QByteArray& certificate)
{
    Q_D(Security8021xSetting);

    d->clientCert = certificate;
}

QByteArray NetworkManager::Settings::Security8021xSetting::clientCertificate() const
{
    Q_D(const Security8021xSetting);

    return d->clientCert;
}

void NetworkManager::Settings::Security8021xSetting::setPhase1PeapVersion(NetworkManager::Settings::Security8021xSetting::PeapVersion version)
{
    Q_D(Security8021xSetting);

    d->phase1PeapVer = version;
}

NetworkManager::Settings::Security8021xSetting::PeapVersion NetworkManager::Settings::Security8021xSetting::phase1PeapVersion() const
{
    Q_D(const Security8021xSetting);

    return d->phase1PeapVer;
}

void NetworkManager::Settings::Security8021xSetting::setPhase1PeapLabel(NetworkManager::Settings::Security8021xSetting::PeapLabel label)
{
    Q_D(Security8021xSetting);

    d->phase1PeapLabel = label;
}

NetworkManager::Settings::Security8021xSetting::PeapLabel NetworkManager::Settings::Security8021xSetting::phase1PeapLabel() const
{
    Q_D(const Security8021xSetting);

    return d->phase1PeapLabel;
}

void NetworkManager::Settings::Security8021xSetting::setPhase1FastProvisioning(NetworkManager::Settings::Security8021xSetting::FastProvisioning provisioning)
{
    Q_D(Security8021xSetting);

    d->phase1FastProvisioning = provisioning;
}

NetworkManager::Settings::Security8021xSetting::FastProvisioning NetworkManager::Settings::Security8021xSetting::phase1FastProvisioning() const
{
    Q_D(const Security8021xSetting);

    return d->phase1FastProvisioning;
}

void NetworkManager::Settings::Security8021xSetting::setPhase2AuthMethod(NetworkManager::Settings::Security8021xSetting::AuthMethod method)
{
    Q_D(Security8021xSetting);

    d->phase2AuthMethod = method;
}

NetworkManager::Settings::Security8021xSetting::AuthMethod NetworkManager::Settings::Security8021xSetting::phase2AuthMethod() const
{
    Q_D(const Security8021xSetting);

    return d->phase2AuthMethod;
}

void NetworkManager::Settings::Security8021xSetting::setPhase2AuthEapMethod(NetworkManager::Settings::Security8021xSetting::AuthEapMethod method)
{
    Q_D(Security8021xSetting);

    d->phase2AuthEapMethod = method;
}

NetworkManager::Settings::Security8021xSetting::AuthEapMethod NetworkManager::Settings::Security8021xSetting::phase2AuthEapMethod() const
{
    Q_D(const Security8021xSetting);

    return d->phase2AuthEapMethod;
}

void NetworkManager::Settings::Security8021xSetting::setPhase2CaCertificate(const QByteArray& certificate)
{
    Q_D(Security8021xSetting);

    d->phase2CaCert = certificate;
}

QByteArray NetworkManager::Settings::Security8021xSetting::phase2CaCertificate() const
{
    Q_D(const Security8021xSetting);

    return d->phase2CaCert;
}

void NetworkManager::Settings::Security8021xSetting::setPhase2CaPath(const QString& path)
{
    Q_D(Security8021xSetting);

    d->phase2CaPath = path;
}

QString NetworkManager::Settings::Security8021xSetting::phase2CaPath() const
{
    Q_D(const Security8021xSetting);

    return d->phase2CaPath;
}

void NetworkManager::Settings::Security8021xSetting::setPhase2SubjectMatch(const QString& substring)
{
    Q_D(Security8021xSetting);

    d->phase2SubjectMatch = substring;
}

QString NetworkManager::Settings::Security8021xSetting::phase2SubjectMatch() const
{
    Q_D(const Security8021xSetting);

    return d->phase2SubjectMatch;
}

void NetworkManager::Settings::Security8021xSetting::setPhase2AltSubjectMatches(const QStringList& strings)
{
    Q_D(Security8021xSetting);

    d->phase2AltSubjectMatches = strings;
}

QStringList NetworkManager::Settings::Security8021xSetting::phase2AltSubjectMatches() const
{
    Q_D(const Security8021xSetting);

    return d->phase2AltSubjectMatches;
}

void NetworkManager::Settings::Security8021xSetting::setPhase2ClientCertificate(const QByteArray& certificate)
{
    Q_D(Security8021xSetting);

    d->phase2ClientCert = certificate;
}

QByteArray NetworkManager::Settings::Security8021xSetting::phase2ClientCertificate() const
{
    Q_D(const Security8021xSetting);

    return d->phase2ClientCert;
}

void NetworkManager::Settings::Security8021xSetting::setPassword(const QString& password)
{
    Q_D(Security8021xSetting);

    d->password = password;
}

QString NetworkManager::Settings::Security8021xSetting::password() const
{
    Q_D(const Security8021xSetting);

    return d->password;
}

void NetworkManager::Settings::Security8021xSetting::setPasswordFlags(NetworkManager::Settings::Setting::SecretFlagType flags)
{
    Q_D(Security8021xSetting);

    d->passwordFlags = flags;
}

NetworkManager::Settings::Setting::SecretFlagType NetworkManager::Settings::Security8021xSetting::passwordFlags() const
{
    Q_D(const Security8021xSetting);

    return d->passwordFlags;
}

void NetworkManager::Settings::Security8021xSetting::setPasswordRaw(const QByteArray& password)
{
    Q_D(Security8021xSetting);

    d->passwordRaw = password;
}

QByteArray NetworkManager::Settings::Security8021xSetting::passwordRaw() const
{
    Q_D(const Security8021xSetting);

    return d->passwordRaw;
}

void NetworkManager::Settings::Security8021xSetting::setPasswordRawFlags(NetworkManager::Settings::Setting::SecretFlagType flags)
{
    Q_D(Security8021xSetting);

    d->passwordRawFlags = flags;
}

NetworkManager::Settings::Setting::SecretFlagType NetworkManager::Settings::Security8021xSetting::passwordRawFlags() const
{
    Q_D(const Security8021xSetting);

    return d->passwordRawFlags;
}

void NetworkManager::Settings::Security8021xSetting::setPrivateKey(const QByteArray& key)
{
    Q_D(Security8021xSetting);

    d->privateKey = key;
}

QByteArray NetworkManager::Settings::Security8021xSetting::privateKey() const
{
    Q_D(const Security8021xSetting);

    return d->privateKey;
}

void NetworkManager::Settings::Security8021xSetting::setPrivateKeyPassword(const QString& password)
{
    Q_D(Security8021xSetting);

    d->privateKeyPassword = password;
}

QString NetworkManager::Settings::Security8021xSetting::privateKeyPassword() const
{
    Q_D(const Security8021xSetting);

    return d->privateKeyPassword;
}

void NetworkManager::Settings::Security8021xSetting::setPrivateKeyPasswordFlags(NetworkManager::Settings::Setting::SecretFlagType flags)
{
    Q_D(Security8021xSetting);

    d->privateKeyPasswordFlags = flags;
}

NetworkManager::Settings::Setting::SecretFlagType NetworkManager::Settings::Security8021xSetting::privateKeyPasswordFlags() const
{
    Q_D(const Security8021xSetting);

    return d->privateKeyPasswordFlags;
}

void NetworkManager::Settings::Security8021xSetting::setPhase2PrivateKey(const QByteArray& key)
{
    Q_D(Security8021xSetting);

    d->phase2PrivateKey = key;
}

QByteArray NetworkManager::Settings::Security8021xSetting::phase2PrivateKey() const
{
    Q_D(const Security8021xSetting);

    return d->phase2PrivateKey;
}

void NetworkManager::Settings::Security8021xSetting::setPhase2PrivateKeyPassword(const QString& password)
{
    Q_D(Security8021xSetting);

    d->phase2PrivateKeyPassword = password;
}

QString NetworkManager::Settings::Security8021xSetting::phase2PrivateKeyPassword() const
{
    Q_D(const Security8021xSetting);

    return d->phase2PrivateKeyPassword;
}

void NetworkManager::Settings::Security8021xSetting::setPhase2PrivateKeyPasswordFlags(NetworkManager::Settings::Setting::SecretFlagType flags)
{
    Q_D(Security8021xSetting);

    d->phase2PrivateKeyPasswordFlags = flags;
}

NetworkManager::Settings::Setting::SecretFlagType NetworkManager::Settings::Security8021xSetting::phase2PrivateKeyPasswordFlags() const
{
    Q_D(const Security8021xSetting);

    return d->phase2PrivateKeyPasswordFlags;
}

void NetworkManager::Settings::Security8021xSetting::setSystemCaCertificates(bool use)
{
    Q_D(Security8021xSetting);

    d->systemCaCerts = use;
}

void NetworkManager::Settings::Security8021xSetting::setPin(const QString& pin)
{
    Q_D(Security8021xSetting);

    d->pin = pin;
}

QString NetworkManager::Settings::Security8021xSetting::pin() const
{
    Q_D(const Security8021xSetting);

    return d->pin;
}

void NetworkManager::Settings::Security8021xSetting::setPinFlags(NetworkManager::Settings::Setting::SecretFlagType flags)
{
    Q_D(Security8021xSetting);

    d->pinFlags = flags;
}

NetworkManager::Settings::Setting::SecretFlagType NetworkManager::Settings::Security8021xSetting::pinFlags() const
{
    Q_D(const Security8021xSetting);

    return d->pinFlags;
}

bool NetworkManager::Settings::Security8021xSetting::systemCaCertificates() const
{
    Q_D(const Security8021xSetting);

    return d->systemCaCerts;
}

QStringList NetworkManager::Settings::Security8021xSetting::needSecrets() const
{
    // TODO
}

void NetworkManager::Settings::Security8021xSetting::secretsFromMap(const QVariantMap& secrets)
{
    // TODO
}

QVariantMap NetworkManager::Settings::Security8021xSetting::secretsToMap() const
{
    // TODO
}

void NetworkManager::Settings::Security8021xSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_802_1X_EAP))) {
        QStringList methods = setting.value(QLatin1String(NM_SETTING_802_1X_EAP)).toStringList();
        QList<EapMethod> eapMethods;
        foreach (const QString & method, methods) {
            if (method == "leap") {
                eapMethods << EapMethodLeap;
            } else if (method == "md5") {
                eapMethods << EapMethodMd5;
            } else if (method == "tls") {
                eapMethods << EapMethodTls;
            } else if (method == "peap") {
                eapMethods << EapMethodPeap;
            } else if (method == "ttls") {
                eapMethods << EapMethodTtls;
            } else if (method == "sim") {
                eapMethods << EapMethodSim;
            } else if (method == "fast") {
                eapMethods << EapMethodFast;
            }
        }

        setEapMethods(eapMethods);
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_IDENTITY))) {
        setIdentity(setting.value(QLatin1String(NM_SETTING_802_1X_IDENTITY)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_ANONYMOUS_IDENTITY))) {
        setAnonymousIdentity(setting.value(QLatin1String(NM_SETTING_802_1X_ANONYMOUS_IDENTITY)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PAC_FILE))) {
        setPacFile(setting.value(QLatin1String(NM_SETTING_802_1X_PAC_FILE)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_CA_CERT))) {
        setCaCertificate(setting.value(QLatin1String(NM_SETTING_802_1X_CA_CERT)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_CA_PATH))) {
        setCaPath(setting.value(QLatin1String(NM_SETTING_802_1X_CA_PATH)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_SUBJECT_MATCH))) {
        setSubjectMatch(setting.value(QLatin1String(NM_SETTING_802_1X_SUBJECT_MATCH)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_ALTSUBJECT_MATCHES))) {
        setAltSubjectMatches(setting.value(QLatin1String(NM_SETTING_802_1X_ALTSUBJECT_MATCHES)).toStringList());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_CLIENT_CERT))) {
        setClientCertificate(setting.value(QLatin1String(NM_SETTING_802_1X_CLIENT_CERT)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE1_PEAPVER))) {
        QString version = setting.value(QLatin1String(NM_SETTING_802_1X_PHASE1_PEAPVER)).toString();

        if (version == "0") {
            setPhase1PeapVersion(PeapVersionZero);
        } else if (version == "1") {
            setPhase1PeapVersion(PeapVersionOne);
        }
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE1_PEAPLABEL))) {
        QString label = setting.value(QLatin1String(NM_SETTING_802_1X_PHASE1_PEAPLABEL)).toString();

        if (label == "1") {
            setPhase1PeapLabel(PeapLabelForce);
        }
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE1_FAST_PROVISIONING))) {
        QString provisioning = setting.value(QLatin1String(NM_SETTING_802_1X_PHASE1_FAST_PROVISIONING)).toString();

        if (provisioning == "0") {
            setPhase1FastProvisioning(FastProvisioningDisabled);
        } else if (provisioning == "1") {
            setPhase1FastProvisioning(FastProvisioningAllowUnauthenticated);
        } else if (provisioning == "2") {
            setPhase1FastProvisioning(FastProvisioningAllowAuthenticated);
        } else if (provisioning == "3") {
            setPhase1FastProvisioning(FastProvisioningAllowBoth);
        }
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_AUTH))) {
        QString authMethod = setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_AUTH)).toString();

        if (authMethod == "pap") {
            setPhase2AuthMethod(AuthMethodPap);
        } else if (authMethod == "chap") {
            setPhase2AuthMethod(AuthMethodChap);
        } else if (authMethod == "mschap") {
            setPhase2AuthMethod(AuthMethodMschap);
        } else if (authMethod == "mschapv2") {
            setPhase2AuthMethod(AuthMethodMschapv2);
        } else if (authMethod == "gtc") {
            setPhase2AuthMethod(AuthMethodGtc);
        } else if (authMethod == "otp") {
            setPhase2AuthMethod(AuthMethodOtp);
        } else if (authMethod == "md5") {
            setPhase2AuthMethod(AuthMethodMd5);
        } else if (authMethod == "tls") {
            setPhase2AuthMethod(AuthMethodTls);
        }
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_AUTHEAP))) {
        QString authEapMethod = setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_AUTHEAP)).toString();

        if (authEapMethod == "md5") {
            setPhase2AuthEapMethod(AuthEapMethodMd5);
        } else if (authEapMethod == "mschapv2") {
            setPhase2AuthEapMethod(AuthEapMethodMschapv2);
        } else if (authEapMethod == "otp") {
            setPhase2AuthEapMethod(AuthEapMethodOtp);
        } else if (authEapMethod == "gtc") {
            setPhase2AuthEapMethod(AuthEapMethodGtc);
        } else if (authEapMethod == "tls") {
            setPhase2AuthEapMethod(AuthEapMethodTls);
        }
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_CA_CERT))) {
        setPhase2CaCertificate(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_CA_CERT)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_CA_PATH))) {
        setPhase2CaPath(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_CA_PATH)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_SUBJECT_MATCH))) {
        setPhase2SubjectMatch(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_SUBJECT_MATCH)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_ALTSUBJECT_MATCHES))) {
        setPhase2AltSubjectMatches(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_ALTSUBJECT_MATCHES)).toStringList());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_CLIENT_CERT))) {
        setPhase2ClientCertificate(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_CLIENT_CERT)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PASSWORD))) {
        setPassword(setting.value(QLatin1String(NM_SETTING_802_1X_PASSWORD)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PASSWORD_FLAGS))) {
        setPasswordFlags((Setting::SecretFlagType)setting.value(QLatin1String(NM_SETTING_802_1X_PASSWORD_FLAGS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW))) {
        setPasswordRaw(setting.value(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW_FLAGS))) {
        setPasswordRawFlags((Setting::SecretFlagType)setting.value(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW_FLAGS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY))) {
        setPrivateKey(setting.value(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD))) {
        setPrivateKeyPassword(setting.value(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD_FLAGS))) {
        setPrivateKeyPasswordFlags((Setting::SecretFlagType)setting.value(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD_FLAGS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY))) {
        setPhase2PrivateKey(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD))) {
        setPhase2PrivateKeyPassword(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD_FLAGS))) {
        setPhase2PrivateKeyPasswordFlags((Setting::SecretFlagType)setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD_FLAGS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PIN))) {
        setPin(setting.value(QLatin1String(NM_SETTING_802_1X_PIN)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PIN_FLAGS))) {
        setPinFlags((Setting::SecretFlagType)setting.value(QLatin1String(NM_SETTING_802_1X_PIN_FLAGS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_SYSTEM_CA_CERTS))) {
        setSystemCaCertificates(setting.value(QLatin1String(NM_SETTING_802_1X_SYSTEM_CA_CERTS)).toBool());
    }
}

QVariantMap NetworkManager::Settings::Security8021xSetting::toMap() const
{
    QVariantMap setting;

    if (!eapMethods().isEmpty()) {
        QStringList methods;

        foreach(EapMethod method, eapMethods()) {
            if (method == EapMethodLeap) {
                methods << "leap";
            } else if (method == EapMethodMd5) {
                methods << "md5";
            } else if (method == EapMethodTls) {
                methods << "tls";
            } else if (method == EapMethodPeap) {
                methods << "peap";
            } else if (method == EapMethodTtls) {
                methods << "ttls";
            } else if (method == EapMethodSim) {
                methods << "sim";
            } else if (method == EapMethodFast) {
                methods << "fast";
            }
        }

        setting.insert(QLatin1String(NM_SETTING_802_1X_EAP), methods);
    }

    if (!identity().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_IDENTITY), identity());
    }

    if (!anonymousIdentity().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_ANONYMOUS_IDENTITY), anonymousIdentity());
    }

    if (!pacFile().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PAC_FILE), pacFile());
    }

    if (!caCertificate().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_CA_CERT), caCertificate());
    }

    if (!caPath().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_CA_PATH), caPath());
    }

    if (!subjectMatch().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_SUBJECT_MATCH), subjectMatch());
    }

    if (!altSubjectMatches().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_ALTSUBJECT_MATCHES), altSubjectMatches());
    }

    if (!clientCertificate().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_CLIENT_CERT), clientCertificate());
    }

    if (phase1PeapVersion() != PeapVersionUnknown) {
        QString version;

        switch (phase1PeapVersion()) {
            case PeapVersionZero:
                version = "0";
                break;
            case PeapVersionOne:
                version = "1";
                break;
        }

        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE1_PEAPVER), version);
    }

    if (phase1PeapLabel() != PeapLabelUnknown) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE1_PEAPLABEL), "1");
    }

    if (phase1FastProvisioning() != FastProvisioningUnknown) {
        QString provisioning;

        switch (phase1FastProvisioning()) {
            case FastProvisioningDisabled:
                provisioning = "0";
                break;
            case FastProvisioningAllowUnauthenticated:
                provisioning = "1";
                break;
            case FastProvisioningAllowAuthenticated:
                provisioning = "2";
                break;
            case FastProvisioningAllowBoth:
                provisioning = "3";
                break;
        }

        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE1_FAST_PROVISIONING), provisioning);
    }

    if (phase2AuthMethod() != AuthMethodUnknown) {
        QString authMethod;

        switch (phase2AuthMethod()) {
            case AuthMethodPap:
                authMethod = "pap";
                break;
            case AuthMethodChap:
                authMethod = "chap";
                break;
            case AuthMethodMschap:
                authMethod = "mschap";
                break;
            case AuthMethodMschapv2:
                authMethod = "mschapv2";
                break;
            case AuthMethodGtc:
                authMethod = "gtc";
                break;
            case AuthMethodOtp:
                authMethod = "otp";
                break;
            case AuthMethodMd5:
                authMethod = "md5";
                break;
            case AuthMethodTls:
                authMethod = "tls";
                break;
        }

        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_AUTH), authMethod);
    }

    if (phase2AuthEapMethod() != AuthEapMethodUnknown) {
        QString authEapMethod;

        switch (phase2AuthEapMethod()) {
            case AuthEapMethodMd5:
                authEapMethod = "md5";
                break;
            case AuthEapMethodMschapv2:
                authEapMethod = "mschapv2";
                break;
            case AuthEapMethodOtp:
                authEapMethod = "otp";
                break;
            case AuthEapMethodGtc:
                authEapMethod = "gtc";
                break;
            case AuthEapMethodTls:
                authEapMethod = "tls";
                break;
        }

        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_AUTHEAP), authEapMethod);
    }

    if (!phase2CaCertificate().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_CA_CERT), phase2CaCertificate());
    }

    if (!phase2CaPath().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_CA_PATH), phase2CaPath());
    }

    if (!phase2SubjectMatch().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_SUBJECT_MATCH), phase2SubjectMatch());
    }

    if (!phase2AltSubjectMatches().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_ALTSUBJECT_MATCHES), phase2AltSubjectMatches());
    }

    if (!phase2ClientCertificate().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_CLIENT_CERT), phase2ClientCertificate());
    }

    if (!password().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PASSWORD), password());
    }

    if (passwordFlags() != None) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PASSWORD_FLAGS), passwordFlags());
    }

    if (!passwordRaw().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW), passwordRaw());
    }

    if (passwordRawFlags() != None) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW_FLAGS), passwordRawFlags());
    }

    if (!privateKey().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY), privateKey());
    }

    if (!privateKeyPassword().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD), privateKeyPassword());
    }

    if (privateKeyPasswordFlags() != None) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD_FLAGS), privateKeyPasswordFlags());
    }

    if (!phase2PrivateKey().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY), phase2PrivateKey());
    }

    if (!phase2PrivateKeyPassword().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD), phase2PrivateKeyPassword());
    }

    if (phase2PrivateKeyPasswordFlags() != None) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD_FLAGS), phase2PrivateKeyPasswordFlags());
    }

    if (!pin().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PIN), pin());
    }

    if (pinFlags() != None) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PIN_FLAGS), pinFlags());
    }

    if (systemCaCertificates()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_SYSTEM_CA_CERTS), systemCaCertificates());
    }

    return setting;
}

void NetworkManager::Settings::Security8021xSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << "EAP METHODS - " << eapMethods();
    qDebug() << "IDENTITY - " << identity();
    qDebug() << "ANONYMOUS IDENTITY - " << anonymousIdentity();
    qDebug() << "PAC FILE - " << pacFile();
    qDebug() << "CA CERTIFICATE - " << caCertificate();
    qDebug() << "CA PATH - " << caPath();
    qDebug() << "SUBJECT MATCH - " << subjectMatch();
    qDebug() << "ALTSUBJECT MATCHES - " << altSubjectMatches();
    qDebug() << "CLIENT CERTIFICATE - " << clientCertificate();
    qDebug() << "PHASE1 PEAP VERSION - " << phase1PeapVersion();
    qDebug() << "PHASE1 PEAP LABEL - " << phase1PeapLabel();
    qDebug() << "PHASE1 FAST PROVISIONING - " << phase1FastProvisioning();
    qDebug() << "PHASE2 AUTH METHOD - " << phase2AuthMethod();
    qDebug() << "PHASE2 AUTH EAP METHOD - " << phase2AuthEapMethod();
    qDebug() << "PHASE2 CA CERTIFICATE - "<< phase2CaCertificate();
    qDebug() << "PHASE2 CA PATH - " << phase2CaPath();
    qDebug() << "PHASE2 SUBJECT MATCH - " << phase2SubjectMatch();
    qDebug() << "PHASE2 ALTSUBJECT MATCHES - " << phase2AltSubjectMatches();
    qDebug() << "PHASE2 CLIENT CERTIFICATE - " << phase2ClientCertificate();
    qDebug() << "PASSWORD - " << password();
    qDebug() << "PASSWORD FLAGS - " << passwordFlags();
    qDebug() << "PASSWORD RAW - " << passwordRaw();
    qDebug() << "PASSWORD RAW FLAGS - " << passwordRawFlags();
    qDebug() << "PRIVATE KEY - " << privateKey();
    qDebug() << "PRIVATE KEY PASSWORD - " << privateKeyPassword();
    qDebug() << "PRIVATE KEY PASSWORD FLAGS - " << privateKeyPasswordFlags();
    qDebug() << "PHASE2 PRIVATE KEY PASSWORD - " << phase2PrivateKey();
    qDebug() << "PHASE2 PRIVATE KEY PASSWORD FLAGS - " << phase2PrivateKeyPasswordFlags();
    qDebug() << "PIN - " << pin();
    qDebug() << "PIN FLAGS - " << pinFlags();
    qDebug() << "SYSTEM CA CERTIFICATES - " << systemCaCertificates();
}
