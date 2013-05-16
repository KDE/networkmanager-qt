/*
    Copyright 2012-2013  Jan Grulich <jgrulich@redhat.com>
    Copyright 2013  Will Stephenson <wstephenson@suse.de>

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

NetworkManager::Settings::CertificateWrapper::CertificateWrapper() :
    scheme(Security8021xSetting::CertKeySchemeNone)
{

}

NetworkManager::Settings::CertificateWrapper::CertificateWrapper(const NetworkManager::Settings::CertificateWrapper &other) :
    scheme(other.scheme), cert(other.cert), fileName(other.fileName)
{
}

NetworkManager::Settings::CertificateWrapper::~CertificateWrapper()
{
}

void NetworkManager::Settings::CertificateWrapper::loadCert(const QString & path, NetworkManager::Settings::Security8021xSetting::CertKeyScheme theScheme)
{
    // http://projects.gnome.org/NetworkManager/developers/api/09/ref-settings.html#idp8706528
    // says this can be a PEM or DER encoded file, but QCA only has a method for PEM from a file,
    // and i don't feel like doing file access in here myself today
    QCA::ConvertResult result;
    cert = QCA::Certificate::fromPEMFile(path, &result);
    if (result == QCA::ConvertGood) {
        scheme = theScheme;
        if (theScheme == NetworkManager::Settings::Security8021xSetting::CertKeySchemePath) {
            fileName = path;
        } else {
            fileName = QString();
        }
    } else {
        scheme = NetworkManager::Settings::Security8021xSetting::CertKeySchemeNone;
        cert = QCA::Certificate();
    }
}

void NetworkManager::Settings::CertificateWrapper::setCertBlob(const QByteArray& certBytes)
{
    cert = QCA::Certificate::fromDER(certBytes);
    scheme = NetworkManager::Settings::Security8021xSetting::CertKeySchemeBlob;
    fileName = QString();
    // WILL also clear hash
}

void NetworkManager::Settings::CertificateWrapper::setCertPath(const QString& path)
{
    cert = QCA::Certificate();
    scheme = NetworkManager::Settings::Security8021xSetting::CertKeySchemePath;
    fileName = path;
    // WILL also clear hash
}

QByteArray NetworkManager::Settings::CertificateWrapper::blob() const
{
    if (scheme == NetworkManager::Settings::Security8021xSetting::CertKeySchemeBlob) {
        // DER encoded data according to
        // http://projects.gnome.org/NetworkManager/developers/api/09/ref-settings.html#idp8706528
        return cert.toDER();
    }
    return QByteArray();
}

QString NetworkManager::Settings::CertificateWrapper::path() const
{
    if (scheme == NetworkManager::Settings::Security8021xSetting::CertKeySchemePath) {
        return fileName;
    }
    return QString();
}

NetworkManager::Settings::KeyWrapper::KeyWrapper() :
    scheme(Security8021xSetting::CertKeySchemeNone)
{

}

NetworkManager::Settings::KeyWrapper::KeyWrapper(const NetworkManager::Settings::KeyWrapper &other) :
    scheme(other.scheme), key(other.key), fileName(other.fileName)
{
}

NetworkManager::Settings::KeyWrapper::~KeyWrapper()
{
}

// WILL use SecureArray here?
void NetworkManager::Settings::KeyWrapper::loadKey(const QString & path, const QCA::SecureArray& password, NetworkManager::Settings::Security8021xSetting::CertKeyScheme theScheme)
{
    // http://projects.gnome.org/NetworkManager/developers/api/09/ref-settings.html#idp8706528
    // says this can be a PEM or DER encoded file, but QCA only has a method for PEM from a file,
    // and i don't feel like doing file access in here myself today
    QCA::ConvertResult result;
    key = QCA::PrivateKey::fromPEMFile(path, password, &result);
    if (result == QCA::ConvertGood) {
        scheme = theScheme;
        if (theScheme == NetworkManager::Settings::Security8021xSetting::CertKeySchemePath) {
            fileName = path;
        } else {
            fileName = QString();
        }
    } else {
        scheme = NetworkManager::Settings::Security8021xSetting::CertKeySchemeNone;
        key = QCA::PrivateKey();
    }
}

void NetworkManager::Settings::KeyWrapper::setBlob(const QByteArray& certBytes)
{
    key = QCA::PrivateKey::fromDER(certBytes);
    scheme = NetworkManager::Settings::Security8021xSetting::CertKeySchemeBlob;
    fileName = QString();
}

void NetworkManager::Settings::KeyWrapper::setPath(const QString& path)
{
    key = QCA::PrivateKey();
    scheme = NetworkManager::Settings::Security8021xSetting::CertKeySchemePath;
    fileName = path;
}

QByteArray NetworkManager::Settings::KeyWrapper::blob() const
{
    if (scheme == NetworkManager::Settings::Security8021xSetting::CertKeySchemeBlob) {
        // DER encoded data according to
        // http://projects.gnome.org/NetworkManager/developers/api/09/ref-settings.html#idp8706528
        return key.toDER().toByteArray();
    }
    return QByteArray();
}

QString NetworkManager::Settings::KeyWrapper::path() const
{
    if (scheme == NetworkManager::Settings::Security8021xSetting::CertKeySchemePath) {
        return fileName;
    }
    return QString();
}

NetworkManager::Settings::Security8021xSettingPrivate::Security8021xSettingPrivate():
    name(NM_SETTING_802_1X_SETTING_NAME),
    phase1PeapVer(Security8021xSetting::PeapVersionUnknown),
    phase1PeapLabel(Security8021xSetting::PeapLabelUnknown),
    phase1FastProvisioning(Security8021xSetting::FastProvisioningUnknown),
    phase2AuthMethod(Security8021xSetting::AuthMethodUnknown),
    phase2AuthEapMethod(Security8021xSetting::AuthEapMethodUnknown),
    passwordFlags(NetworkManager::Settings::Setting::None),
    passwordRawFlags(NetworkManager::Settings::Setting::None),
    privateKeyPasswordFlags(NetworkManager::Settings::Setting::None),
    phase2PrivateKeyPasswordFlags(NetworkManager::Settings::Setting::None),
    pinFlags(NetworkManager::Settings::Setting::None),
    systemCaCerts(false)
{ }

NetworkManager::Settings::Security8021xSetting::Security8021xSetting():
    Setting(Setting::Security8021x),
    d_ptr(new Security8021xSettingPrivate())
{ }

NetworkManager::Settings::Security8021xSetting::Security8021xSetting(const Ptr &other):
    Setting(other),
    d_ptr(new Security8021xSettingPrivate())
{
    Q_D(Security8021xSetting);
    setEapMethods(other->eapMethods());
    setIdentity(other->identity());
    setAnonymousIdentity(other->anonymousIdentity());
    setPacFile(other->pacFile());
    // WILL is this a hack?
    d->caCert = other.data()->d_ptr->caCert;
    setSubjectMatch(other->subjectMatch());
    setAltSubjectMatches(other->altSubjectMatches());
    // WILL is this a hack?
    d->clientCert = other.data()->d_ptr->clientCert;
    setPhase1PeapVersion(other->phase1PeapVersion());
    setPhase1PeapLabel(other->phase1PeapLabel());
    setPhase1FastProvisioning(other->phase1FastProvisioning());
    setPhase2AuthMethod(other->phase2AuthMethod());
    setPhase2AuthEapMethod(other->phase2AuthEapMethod());
    d->phase2CaCert = other.data()->d_ptr->phase2CaCert;
    d->phase2ClientCert = other.data()->d_ptr->phase2ClientCert;
    setPhase2SubjectMatch(other->phase2SubjectMatch());
    setPhase2AltSubjectMatches(other->phase2AltSubjectMatches());
    setPassword(other->password());
    setPasswordFlags(other->passwordFlags());
    setPasswordRaw(other->passwordRaw());
    setPasswordRawFlags(other->passwordRawFlags());
    setPrivateKey(other->privateKey());
    setPrivateKeyPassword(other->privateKeyPassword());
    setPrivateKeyPasswordFlags(other->privateKeyPasswordFlags());
    setPhase2PrivateKey(other->phase2PrivateKey());
    setPhase2PrivateKeyPassword(other->phase2PrivateKeyPassword());
    setPhase2PrivateKeyPasswordFlags(other->phase2PrivateKeyPasswordFlags());
    setSystemCaCertificates(other->systemCaCertificates());
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

NetworkManager::Settings::Security8021xSetting::CertKeyScheme NetworkManager::Settings::Security8021xSetting::caCertificateScheme() const
{
    Q_D(const Security8021xSetting);
    return d->caCert.scheme;
}

void NetworkManager::Settings::Security8021xSetting::setCaCertificate(const QString &caCertPath, CertKeyScheme scheme)
{
    Q_D(Security8021xSetting);

    d->caCert.loadCert(caCertPath, scheme);
}

QByteArray NetworkManager::Settings::Security8021xSetting::caCertificateBlob() const
{
    Q_D(const Security8021xSetting);
    return d->caCert.blob();
}

QString NetworkManager::Settings::Security8021xSetting::caCertificatePath() const
{
    Q_D(const Security8021xSetting);
    return d->caCert.path();
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

NetworkManager::Settings::Security8021xSetting::CertKeyScheme NetworkManager::Settings::Security8021xSetting::clientCertificateScheme() const
{
    Q_D(const Security8021xSetting);
    return d->clientCert.scheme;
}

void NetworkManager::Settings::Security8021xSetting::setClientCertificate(const QString &clientCertPath, CertKeyScheme scheme)
{
    Q_D(Security8021xSetting);

    d->clientCert.loadCert(clientCertPath, scheme);
}

QByteArray NetworkManager::Settings::Security8021xSetting::clientCertificateBlob() const
{
    Q_D(const Security8021xSetting);
    return d->clientCert.blob();
}

QString NetworkManager::Settings::Security8021xSetting::clientCertificatePath() const
{
    Q_D(const Security8021xSetting);
    return d->clientCert.path();
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

NetworkManager::Settings::Security8021xSetting::CertKeyScheme NetworkManager::Settings::Security8021xSetting::phase2CaCertificateScheme() const
{
    Q_D(const Security8021xSetting);
    return d->phase2CaCert.scheme;
}

void NetworkManager::Settings::Security8021xSetting::setPhase2CaCertificate(const QString &phase2CaCertPath, CertKeyScheme scheme)
{
    Q_D(Security8021xSetting);

    d->phase2CaCert.loadCert(phase2CaCertPath, scheme);
}

QByteArray NetworkManager::Settings::Security8021xSetting::phase2CaCertificateBlob() const
{
    Q_D(const Security8021xSetting);
    return d->phase2CaCert.blob();
}

QString NetworkManager::Settings::Security8021xSetting::phase2CaCertificatePath() const
{
    Q_D(const Security8021xSetting);
    return d->phase2CaCert.path();
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

NetworkManager::Settings::Security8021xSetting::CertKeyScheme NetworkManager::Settings::Security8021xSetting::phase2ClientCertificateScheme() const
{
    Q_D(const Security8021xSetting);
    return d->phase2ClientCert.scheme;
}

void NetworkManager::Settings::Security8021xSetting::setPhase2ClientCertificate(const QString &phase2ClientCertPath, CertKeyScheme scheme)
{
    Q_D(Security8021xSetting);

    d->phase2ClientCert.loadCert(phase2ClientCertPath, scheme);
}

QByteArray NetworkManager::Settings::Security8021xSetting::phase2ClientCertificateBlob() const
{
    Q_D(const Security8021xSetting);
    return d->phase2ClientCert.blob();
}

QString NetworkManager::Settings::Security8021xSetting::phase2ClientCertificatePath() const
{
    Q_D(const Security8021xSetting);
    return d->phase2ClientCert.path();
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

void NetworkManager::Settings::Security8021xSetting::setPasswordFlags(NetworkManager::Settings::Setting::SecretFlags flags)
{
    Q_D(Security8021xSetting);

    d->passwordFlags = flags;
}

NetworkManager::Settings::Setting::SecretFlags NetworkManager::Settings::Security8021xSetting::passwordFlags() const
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

void NetworkManager::Settings::Security8021xSetting::setPasswordRawFlags(NetworkManager::Settings::Setting::SecretFlags flags)
{
    Q_D(Security8021xSetting);

    d->passwordRawFlags = flags;
}

NetworkManager::Settings::Setting::SecretFlags NetworkManager::Settings::Security8021xSetting::passwordRawFlags() const
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

void NetworkManager::Settings::Security8021xSetting::setPrivateKeyPasswordFlags(NetworkManager::Settings::Setting::SecretFlags flags)
{
    Q_D(Security8021xSetting);

    d->privateKeyPasswordFlags = flags;
}

NetworkManager::Settings::Setting::SecretFlags NetworkManager::Settings::Security8021xSetting::privateKeyPasswordFlags() const
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

void NetworkManager::Settings::Security8021xSetting::setPhase2PrivateKeyPasswordFlags(NetworkManager::Settings::Setting::SecretFlags flags)
{
    Q_D(Security8021xSetting);

    d->phase2PrivateKeyPasswordFlags = flags;
}

NetworkManager::Settings::Setting::SecretFlags NetworkManager::Settings::Security8021xSetting::phase2PrivateKeyPasswordFlags() const
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

void NetworkManager::Settings::Security8021xSetting::setPinFlags(NetworkManager::Settings::Setting::SecretFlags flags)
{
    Q_D(Security8021xSetting);

    d->pinFlags = flags;
}

NetworkManager::Settings::Setting::SecretFlags NetworkManager::Settings::Security8021xSetting::pinFlags() const
{
    Q_D(const Security8021xSetting);

    return d->pinFlags;
}

bool NetworkManager::Settings::Security8021xSetting::systemCaCertificates() const
{
    Q_D(const Security8021xSetting);

    return d->systemCaCerts;
}

QStringList NetworkManager::Settings::Security8021xSetting::needSecrets(bool requestNew) const
{
    QStringList secrets;

    if (eapMethods().contains(EapMethodTls) && (privateKeyPassword().isEmpty() || requestNew) &&
            !privateKeyPasswordFlags().testFlag(NotRequired)) {
        secrets << QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD);
    } else if ((eapMethods().contains(EapMethodTtls) || eapMethods().contains(EapMethodPeap) ||
                eapMethods().contains(EapMethodLeap) || eapMethods().contains(EapMethodFast)) &&
               (password().isEmpty() || requestNew) && !passwordFlags().testFlag(NotRequired)) {
        secrets << QLatin1String(NM_SETTING_802_1X_PASSWORD);
        secrets << QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW);
    } else if (eapMethods().contains(EapMethodSim) && (pin().isEmpty() || requestNew) && !pinFlags().testFlag(NotRequired)) {
        secrets << QLatin1String(NM_SETTING_802_1X_PIN);
    }

    if ((phase2AuthMethod() == AuthMethodTls || phase2AuthEapMethod() == AuthEapMethodTls) &&
            (phase2PrivateKeyPassword().isEmpty() || requestNew) && !phase2PrivateKeyPasswordFlags().testFlag(NotRequired)) {
        secrets << QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD);
    }

    return secrets;
}

void NetworkManager::Settings::Security8021xSetting::secretsFromMap(const QVariantMap& secrets)
{
    if (secrets.contains(QLatin1String(NM_SETTING_802_1X_PASSWORD))) {
        setPassword(secrets.value(QLatin1String(NM_SETTING_802_1X_PASSWORD)).toString());
    }

    if (secrets.contains(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW))) {
        setPasswordRaw(secrets.value(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW)).toByteArray());
    }

    if (secrets.contains(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD))) {
        setPrivateKeyPassword(secrets.value(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD)).toString());
    }

    if (secrets.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD))) {
        setPhase2PrivateKeyPassword(secrets.value(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD)).toString());
    }

    if (secrets.contains(QLatin1String(NM_SETTING_802_1X_PIN))) {
        setPin(secrets.value(QLatin1String(NM_SETTING_802_1X_PIN)).toString());
    }
}

QVariantMap NetworkManager::Settings::Security8021xSetting::secretsToMap() const
{
    QVariantMap secrets;

    if (!password().isEmpty()) {
        secrets.insert(QLatin1String(NM_SETTING_802_1X_PASSWORD), password());
    }

    if (!passwordRaw().isEmpty()) {
        secrets.insert(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW), passwordRaw());
    }

    if (!privateKeyPassword().isEmpty()) {
        secrets.insert(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD), privateKeyPassword());
    }

    if (!phase2PrivateKeyPassword().isEmpty()) {
        secrets.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD), phase2PrivateKeyPassword());
    }

    if (!pin().isEmpty()) {
        secrets.insert(QLatin1String(NM_SETTING_802_1X_PIN), pin());
    }

    return secrets;
}

void NetworkManager::Settings::Security8021xSetting::fromMap(const QVariantMap& setting)
{
    Q_D(Security8021xSetting);

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_EAP))) {
        QStringList methods = setting.value(QLatin1String(NM_SETTING_802_1X_EAP)).toStringList();
        QList<EapMethod> eapMethods;
        foreach(const QString & method, methods) {
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
        d->caCert.setCertBlob(setting.value(QLatin1String(NM_SETTING_802_1X_CA_CERT)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_CA_PATH))) {
    }
        d->caCert.setCertPath(setting.value(QLatin1String(NM_SETTING_802_1X_CA_PATH)).toString());

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_SUBJECT_MATCH))) {
        setSubjectMatch(setting.value(QLatin1String(NM_SETTING_802_1X_SUBJECT_MATCH)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_ALTSUBJECT_MATCHES))) {
        setAltSubjectMatches(setting.value(QLatin1String(NM_SETTING_802_1X_ALTSUBJECT_MATCHES)).toStringList());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_CLIENT_CERT))) {
        d->clientCert.setCertBlob(setting.value(QLatin1String(NM_SETTING_802_1X_CLIENT_CERT)).toByteArray());
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
        d->phase2CaCert.setCertBlob(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_CA_CERT)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_CA_PATH))) {
        d->phase2CaCert.setCertPath(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_CA_PATH)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_SUBJECT_MATCH))) {
        setPhase2SubjectMatch(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_SUBJECT_MATCH)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_ALTSUBJECT_MATCHES))) {
        setPhase2AltSubjectMatches(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_ALTSUBJECT_MATCHES)).toStringList());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_CLIENT_CERT))) {
        d->phase2ClientCert.setCertBlob(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_CLIENT_CERT)).toByteArray());
    }

    // WILL PHASE2_CLIENT_CERT_PATH needed?

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PASSWORD))) {
        setPassword(setting.value(QLatin1String(NM_SETTING_802_1X_PASSWORD)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PASSWORD_FLAGS))) {
        setPasswordFlags((Setting::SecretFlags)setting.value(QLatin1String(NM_SETTING_802_1X_PASSWORD_FLAGS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW))) {
        setPasswordRaw(setting.value(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW_FLAGS))) {
        setPasswordRawFlags((Setting::SecretFlags)setting.value(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW_FLAGS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY))) {
        setPrivateKey(setting.value(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD))) {
        setPrivateKeyPassword(setting.value(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD_FLAGS))) {
        setPrivateKeyPasswordFlags((Setting::SecretFlags)setting.value(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD_FLAGS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY))) {
        setPhase2PrivateKey(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD))) {
        setPhase2PrivateKeyPassword(setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD_FLAGS))) {
        setPhase2PrivateKeyPasswordFlags((Setting::SecretFlags)setting.value(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD_FLAGS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PIN))) {
        setPin(setting.value(QLatin1String(NM_SETTING_802_1X_PIN)).toString());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_PIN_FLAGS))) {
        setPinFlags((Setting::SecretFlags)setting.value(QLatin1String(NM_SETTING_802_1X_PIN_FLAGS)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_802_1X_SYSTEM_CA_CERTS))) {
        setSystemCaCertificates(setting.value(QLatin1String(NM_SETTING_802_1X_SYSTEM_CA_CERTS)).toBool());
    }
}

QVariantMap NetworkManager::Settings::Security8021xSetting::toMap() const
{
    Q_D(const Security8021xSetting);
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

    switch (d->caCert.scheme) {
        case CertKeySchemeNone:
            break;
        case CertKeySchemeBlob:
            setting.insert(QLatin1String(NM_SETTING_802_1X_CA_CERT), d->caCert.cert.toDER());
            break;
        case CertKeySchemePath:
            setting.insert(QLatin1String(NM_SETTING_802_1X_CA_PATH), d->caCert.fileName);
            break;
    };

    if (!subjectMatch().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_SUBJECT_MATCH), subjectMatch());
    }

    if (!altSubjectMatches().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_ALTSUBJECT_MATCHES), altSubjectMatches());
    }

    if (d->clientCert.scheme == CertKeySchemeBlob) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_CLIENT_CERT), d->clientCert.blob());
    }

    if (phase1PeapVersion() != PeapVersionUnknown) {
        QString version;

        switch (phase1PeapVersion()) {
        case PeapVersionZero:
            version = '0';
            break;
        case PeapVersionOne:
            version = '1';
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
            provisioning = '0';
            break;
        case FastProvisioningAllowUnauthenticated:
            provisioning = '1';
            break;
        case FastProvisioningAllowAuthenticated:
            provisioning = '2';
            break;
        case FastProvisioningAllowBoth:
            provisioning = '3';
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

    if (phase2CaCertificateScheme() == CertKeySchemeBlob) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_CA_CERT), phase2CaCertificateBlob());
    }

    if (phase2CaCertificateScheme() == CertKeySchemePath) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_CA_PATH), phase2CaCertificatePath());
    }

    if (!phase2SubjectMatch().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_SUBJECT_MATCH), phase2SubjectMatch());
    }

    if (!phase2AltSubjectMatches().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_ALTSUBJECT_MATCHES), phase2AltSubjectMatches());
    }

    if (d->phase2ClientCert.scheme == CertKeySchemeBlob) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_CLIENT_CERT), d->phase2ClientCert.blob());
    }

    if (!password().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PASSWORD), password());
    }

    if (passwordFlags() != None) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PASSWORD_FLAGS), (int)passwordFlags());
    }

    if (!passwordRaw().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW), passwordRaw());
    }

    if (passwordRawFlags() != None) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PASSWORD_RAW_FLAGS), (int)passwordRawFlags());
    }

    if (!privateKey().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY), privateKey());
    }

    if (!privateKeyPassword().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD), privateKeyPassword());
    }

    if (privateKeyPasswordFlags() != None) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD_FLAGS), (int)privateKeyPasswordFlags());
    }

    if (!phase2PrivateKey().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY), phase2PrivateKey());
    }

    if (!phase2PrivateKeyPassword().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD), phase2PrivateKeyPassword());
    }

    if (phase2PrivateKeyPasswordFlags() != None) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD_FLAGS), (int)phase2PrivateKeyPasswordFlags());
    }

    if (!pin().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PIN), pin());
    }

    if (pinFlags() != None) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_PIN_FLAGS), (int)pinFlags());
    }

    if (systemCaCertificates()) {
        setting.insert(QLatin1String(NM_SETTING_802_1X_SYSTEM_CA_CERTS), systemCaCertificates());
    }

    return setting;
}

QDebug NetworkManager::Settings::operator <<(QDebug dbg, const NetworkManager::Settings::Security8021xSetting &setting)
{
    dbg.nospace() << static_cast<NetworkManager::Settings::Setting>(setting);

    dbg.nospace() << NM_SETTING_802_1X_EAP << ": " << setting.eapMethods() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_IDENTITY << ": " << setting.identity() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_ANONYMOUS_IDENTITY << ": " << setting.anonymousIdentity() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PAC_FILE << ": " << setting.pacFile() << '\n';

    switch (setting.caCertificateScheme()) {
        case NetworkManager::Settings::Security8021xSetting::CertKeySchemeNone:
            dbg.nospace() << NM_SETTING_802_1X_CA_CERT << ": " << "NONE";
            break;
        case NetworkManager::Settings::Security8021xSetting::CertKeySchemeBlob:
            dbg.nospace() << NM_SETTING_802_1X_CA_CERT << ": " << setting.caCertificateBlob();
            break;
        case NetworkManager::Settings::Security8021xSetting::CertKeySchemePath:
            dbg.nospace() << NM_SETTING_802_1X_CA_PATH << ": " << setting.caCertificatePath();
            break;
    };

    dbg.nospace() << NM_SETTING_802_1X_SUBJECT_MATCH << ": " << setting.subjectMatch() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_ALTSUBJECT_MATCHES << ": " << setting.altSubjectMatches() << '\n';

    switch (setting.clientCertificateScheme()) {
        case NetworkManager::Settings::Security8021xSetting::CertKeySchemeNone:
            dbg.nospace() << NM_SETTING_802_1X_CLIENT_CERT << ": " << "NONE";
            break;
        case NetworkManager::Settings::Security8021xSetting::CertKeySchemeBlob:
            dbg.nospace() << NM_SETTING_802_1X_CLIENT_CERT << ": " << setting.clientCertificateBlob();
            break;
    };

    dbg.nospace() << NM_SETTING_802_1X_PHASE1_PEAPVER << ": " << setting.phase1PeapVersion() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PHASE1_PEAPLABEL << ": " << setting.phase1PeapLabel() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PHASE1_FAST_PROVISIONING << ": " << setting.phase1FastProvisioning() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PHASE2_AUTH << ": " << setting.phase2AuthMethod() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PHASE2_AUTHEAP << ": " << setting.phase2AuthEapMethod() << '\n';

    switch (setting.phase2CaCertificateScheme()) {
        case NetworkManager::Settings::Security8021xSetting::CertKeySchemeNone:
            dbg.nospace() << NM_SETTING_802_1X_PHASE2_CA_CERT << ": " << "NONE";
            break;
        case NetworkManager::Settings::Security8021xSetting::CertKeySchemeBlob:
            dbg.nospace() << NM_SETTING_802_1X_PHASE2_CA_CERT << ": " << setting.phase2CaCertificateBlob();
            break;
        case NetworkManager::Settings::Security8021xSetting::CertKeySchemePath:
            dbg.nospace() << NM_SETTING_802_1X_PHASE2_CA_PATH << ": " << setting.phase2CaCertificatePath();
            break;
    };

    dbg.nospace() << NM_SETTING_802_1X_PHASE2_SUBJECT_MATCH << ": " << setting.phase2SubjectMatch() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PHASE2_ALTSUBJECT_MATCHES << ": " << setting.phase2AltSubjectMatches() << '\n';

    switch (setting.phase2ClientCertificateScheme()) {
        case NetworkManager::Settings::Security8021xSetting::CertKeySchemeNone:
            dbg.nospace() << NM_SETTING_802_1X_PHASE2_CLIENT_CERT << ": " << "NONE";
            break;
        case NetworkManager::Settings::Security8021xSetting::CertKeySchemeBlob:
            dbg.nospace() << NM_SETTING_802_1X_PHASE2_CLIENT_CERT << ": " << setting.phase2ClientCertificateBlob();
            break;
    };

    dbg.nospace() << NM_SETTING_802_1X_PASSWORD << ": " << setting.password() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PASSWORD_FLAGS << ": " << setting.passwordFlags() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PASSWORD_RAW << ": " << setting.passwordRaw() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PASSWORD_RAW_FLAGS << ": " << setting.passwordRawFlags() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PRIVATE_KEY << ": " << setting.privateKey() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD << ": " << setting.privateKeyPassword() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PRIVATE_KEY_PASSWORD_FLAGS << ": " << setting.privateKeyPasswordFlags() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PHASE2_PRIVATE_KEY << ": " << setting.phase2PrivateKey() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD << ": " << setting.phase2PrivateKeyPassword() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PHASE2_PRIVATE_KEY_PASSWORD_FLAGS << ": " << setting.phase2PrivateKeyPasswordFlags() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PIN << ": " << setting.pin() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_PIN_FLAGS << ": " << setting.pinFlags() << '\n';
    dbg.nospace() << NM_SETTING_802_1X_SYSTEM_CA_CERTS << ": " << setting.systemCaCertificates() << '\n';

    return dbg.maybeSpace();
}
