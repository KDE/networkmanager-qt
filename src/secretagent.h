/*
    SPDX-FileCopyrightText: 2011 Ilia Kats <ilia-kats@gmx.net>
    SPDX-FileCopyrightText: 2011-2013 Lamarque Souza <lamarque@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_SECRETAGENT_H
#define NETWORKMANAGERQT_SECRETAGENT_H

#include <QDBusContext>
#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QObject>

#include "generictypes.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

namespace NetworkManager
{
class SecretAgentPrivate;

/*!
 * \class NetworkManager::SecretAgent
 * \inheaderfile NetworkManagerQt/SecretAgent
 * \inmodule NetworkManagerQt
 *
 * \brief Implementation of a private D-Bus interface used by secret agents that store and provide secrets to NetworkManager.
 *
 * If an agent provides secrets to NetworkManager as part of connection creation, and the some of those secrets are "agent owned"
 * the agent should store those secrets itself and should not expect its SaveSecrets() method to be called.
 * SaveSecrets() will be called eg if some program other than the agent itself (like a connection editor) changes the secrets out of band.
 */
class NETWORKMANAGERQT_EXPORT SecretAgent : public QObject, protected QDBusContext
{
    Q_OBJECT
public:
    /*!
     *
     * \value NotAuthorized
     * \value InvalidConnection
     * \value UserCanceled
     * \value AgentCanceled
     * \value InternalError
     * \value NoSecrets
     */
    enum Error {
        NotAuthorized,
        InvalidConnection,
        UserCanceled,
        AgentCanceled,
        InternalError,
        NoSecrets,
    };

    /*!
     *
     * Flags modifying the behavior of GetSecrets request.
     *
     * \value None
     *        No special behavior; by default no user interaction is allowed and requests for secrets are fulfilled from persistent storage, or if no secrets
     * are available an error is returned.
     * \value AllowInteraction Allows the request to interact with the user, possibly prompting via UI for secrets if any
     * are required, or if none are found in persistent storage.
     * \value RequestNew Explicitly prompt for new secrets from the user. This flag signals that
     * NetworkManager thinks any existing secrets are invalid or wrong. This flag implies that interaction is allowed.
     * \value UserRequested Set if the request
     * was initiated by user-requested action via the D-Bus interface, as opposed to automatically initiated by NetworkManager in response to (for example) scan
     * results or carrier changes.
     */
    enum GetSecretsFlag {
        None = 0,
        AllowInteraction = 0x01,
        RequestNew = 0x02,
        UserRequested = 0x04,
    };
    Q_DECLARE_FLAGS(GetSecretsFlags, GetSecretsFlag)

    /*!
     *
     * Capabilities to pass to secret agents
     *
     * \value NoCapability
     *        No capability
     * \value VpnHints
     *        Pass hints to secret agent
     */
    enum Capability {
        NoCapability = 0,
        VpnHints = 0x01,
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)

    /*!
     * Registers a SecretAgent with the \a id on NetworkManager
     * Optionally add a capabilities argument
     */
    explicit SecretAgent(const QString &id, QObject *parent = nullptr);
    /*!
     */
    explicit SecretAgent(const QString &id, NetworkManager::SecretAgent::Capabilities capabilities, QObject *parent = nullptr);
    ~SecretAgent() override;

    /*!
     * Send to NetworkManager the \a error the subclass has
     * found, the \a explanation is useful for debugging purposes,
     * and the \a callMessage is ONLY needed if setDelayedReply()
     * was set to \a true when the method was called.
     */
    void sendError(Error error, const QString &explanation, const QDBusMessage &callMessage = QDBusMessage()) const;

public Q_SLOTS:
    /*!
     * Called when the subclass should retrieve and return secrets.
     * If the request is canceled, called function should call
     * sendError(), in this case the return value is ignored.
     *
     * \a connection Nested settings maps containing the connection for which secrets are being requested.
     *        This may contain system-owned secrets if the agent has successfully authenticated to modify system network settings
     *        and the GetSecrets request flags allow user interaction.
     *
     * \a connection_path Object path of the connection for which secrets are being requested.
     *
     * \a setting_name Setting name for which secrets are being requested.
     *
     * \a hints Array of strings of key names in the requested setting for which NetworkManager thinks a secrets may be required,
     *        and/or well-known identifiers and data that may be useful to the client in processing the secrets request. Note that it's not
     *        always possible to determine which secret is required, so in some cases no hints may be given. The Agent should return any
     *        secrets it has, or that it thinks are required, regardless of what hints NetworkManager sends in this request.
     *
     * \a flags Flags which modify the behavior of the secrets request (see \ GetSecretsFlag)
     */
    virtual NMVariantMapMap GetSecrets(const NMVariantMapMap &connection,
                                       const QDBusObjectPath &connection_path,
                                       const QString &setting_name,
                                       const QStringList &hints,
                                       uint flags) = 0;

    /*!
     * Called when the subclass should cancel an outstanding request to
     * get secrets for a given connection.
     * Cancelling the request MUST sendError() with the original
     * DBus message using AgentCanceled param as the error type.
     *
     * \a connection_path Object path of the connection for which, if secrets for the given 'setting_name' are being requested, the request should be
     * canceled.
     *
     * \a setting_name Setting name for which secrets for this connection were originally being requested.
     */
    virtual void CancelGetSecrets(const QDBusObjectPath &connection_path, const QString &setting_name) = 0;

    /*!
     * Called when the subclass should save the secrets contained in the
     * connection to backing storage.
     *
     * \a connection Nested settings maps containing the connection for which secrets are being saved.
     *        This may contain system-owned secrets if the agent has successfully authenticated to modify system network settings
     *        and the GetSecrets request flags allow user interaction.
     *
     * \a connection_path Object path of the connection for which the agent should save secrets to backing storage.
     */
    virtual void SaveSecrets(const NMVariantMapMap &connection, const QDBusObjectPath &connection_path) = 0;

    /*!
     * Called when the subclass should delete the secrets contained in the
     * connection from backing storage.
     *
     * \a connection Nested settings maps containing the connection properties (sans secrets),
     *        for which the agent should delete the secrets from backing storage.
     *
     * \a connection_path Object path of the connection for which the agent should delete secrets from backing storage.
     */
    virtual void DeleteSecrets(const NMVariantMapMap &connection, const QDBusObjectPath &connection_path) = 0;

private:
    Q_DECLARE_PRIVATE(SecretAgent)
    Q_PRIVATE_SLOT(d_func(), void registerAgent())
    Q_PRIVATE_SLOT(d_func(), void registerAgent(const NetworkManager::SecretAgent::Capabilities capabilities))
    Q_PRIVATE_SLOT(d_func(), void dbusInterfacesAdded(const QDBusObjectPath &path, const QVariantMap &interfaces))

    SecretAgentPrivate *const d_ptr;
};
}
Q_DECLARE_OPERATORS_FOR_FLAGS(NetworkManager::SecretAgent::GetSecretsFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(NetworkManager::SecretAgent::Capabilities)

#endif // NETWORKMANAGERQT_SECRETAGENT_H
