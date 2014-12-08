/*
    Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
    Copyright 2011-2013 Lamarque Souza <lamarque@kde.org>

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

#ifndef NETWORKMANAGERQT_SECRETAGENT_H
#define NETWORKMANAGERQT_SECRETAGENT_H

#include <QObject>
#include <QDBusContext>
#include <QDBusObjectPath>
#include <QDBusMessage>

#include <networkmanagerqt_export.h>
#include "generictypes.h"

namespace NetworkManager
{
class SecretAgentPrivate;

/**
 * Implementation of a private D-Bus interface used by secret agents that store and provide secrets to NetworkManager.
 * If an agent provides secrets to NetworkManager as part of connection creation, and the some of those secrets are "agent owned"
 * the agent should store those secrets itself and should not expect its SaveSecrets() method to be called.
 * SaveSecrets() will be called eg if some program other than the agent itself (like a connection editor) changes the secrets out of band.
 */
class NETWORKMANAGERQT_EXPORT SecretAgent : public QObject, protected QDBusContext
{
    Q_OBJECT
public:
    enum Error {
        NotAuthorized,
        InvalidConnection,
        UserCanceled,
        AgentCanceled,
        InternalError,
        NoSecrets
    };

    /**
     * Flags modifying the behavior of GetSecrets request.
     */
    enum GetSecretsFlag {
        None = 0, /**<  No special behavior; by default no user interaction is allowed and requests for secrets are fulfilled from persistent storage, or if no secrets are available an error is returned. */
        AllowInteraction = 0x01, /**< Allows the request to interact with the user, possibly prompting via UI for secrets if any are required, or if none are found in persistent storage. */
        RequestNew = 0x02, /**< Explicitly prompt for new secrets from the user. This flag signals that NetworkManager thinks any existing secrets are invalid or wrong. This flag implies that interaction is allowed. */
        UserRequested = 0x04 /**< Set if the request was initiated by user-requested action via the D-Bus interface, as opposed to automatically initiated by NetworkManager in response to (for example) scan results or carrier changes. */
    };
    Q_DECLARE_FLAGS(GetSecretsFlags, GetSecretsFlag)

    /**
     * Registers a SecretAgent with the \p id on NetworkManager
     */
    explicit SecretAgent(const QString &id, QObject *parent = 0);
    virtual ~SecretAgent();

    /**
     * Send to NetworkManager the \p error the subclass has
     * found, the \p explanation is useful for debugging purposes,
     * and the \p callMessage is ONLY needed if \ref setDelayedReply()
     * was set to @p true when the method was called.
     */
    void sendError(Error error, const QString &explanation, const QDBusMessage &callMessage = QDBusMessage()) const;

public Q_SLOTS:
    /**
     * Called when the subclass should retrieve and return secrets.
     * If the request is canceled, called function should call
     * \ref sendError(), in this case the return value is ignored.
     *
     * @param connection Nested settings maps containing the connection for which secrets are being requested.
     *        This may contain system-owned secrets if the agent has successfully authenticated to modify system network settings
     *        and the GetSecrets request flags allow user interaction.
     * @param connection_path Object path of the connection for which secrets are being requested.
     * @param setting_name Setting name for which secrets are being requested.
     * @param hints Array of strings of key names in the requested setting for which NetworkManager thinks a secrets may be required,
     *        and/or well-known identifiers and data that may be useful to the client in processing the secrets request. Note that it's not
     *        always possible to determine which secret is required, so in some cases no hints may be given. The Agent should return any
     *        secrets it has, or that it thinks are required, regardless of what hints NetworkManager sends in this request.
     * @param flags Flags which modify the behavior of the secrets request (see @ref GetSecretsFlag)
     */
    virtual NMVariantMapMap GetSecrets(const NMVariantMapMap &connection, const QDBusObjectPath &connection_path, const QString &setting_name, const QStringList &hints, uint flags) = 0;

    /**
     * Called when the subclass should cancel an outstanding request to
     * get secrets for a given connection.
     * Cancelling the request MUST \ref sendError() with the original
     * DBus message using \ref AgentCanceled param as the error type.
     *
     * @param connection_path Object path of the connection for which, if secrets for the given 'setting_name' are being requested, the request should be canceled.
     * @param setting_name Setting name for which secrets for this connection were originally being requested.
     */
    virtual void CancelGetSecrets(const QDBusObjectPath &connection_path, const QString &setting_name) = 0;

    /**
     * Called when the subclass should save the secrets contained in the
     * connection to backing storage.
     *
     * @param connection Nested settings maps containing the connection for which secrets are being saved.
     *        This may contain system-owned secrets if the agent has successfully authenticated to modify system network settings
     *        and the GetSecrets request flags allow user interaction.
     * @param connection_path Object path of the connection for which the agent should save secrets to backing storage.
     */
    virtual void SaveSecrets(const NMVariantMapMap &connection, const QDBusObjectPath &connection_path) = 0;

    /**
     * Called when the subclass should delete the secrets contained in the
     * connection from backing storage.
     *
     * @param connection Nested settings maps containing the connection properties (sans secrets),
     *        for which the agent should delete the secrets from backing storage.
     * @param connection_path Object path of the connection for which the agent should delete secrets from backing storage.
     */
    virtual void DeleteSecrets(const NMVariantMapMap &connection, const QDBusObjectPath &connection_path) = 0;

private:
    Q_DECLARE_PRIVATE(SecretAgent)
    Q_PRIVATE_SLOT(d_func(), void registerAgent())

    SecretAgentPrivate *const d_ptr;
};
}
Q_DECLARE_OPERATORS_FOR_FLAGS(NetworkManager::SecretAgent::GetSecretsFlags)

#endif // NETWORKMANAGERQT_SECRETAGENT_H
