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

#ifndef NMQT_SECRETAGENT_H
#define NMQT_SECRETAGENT_H

#include <QObject>
#include <QDBusContext>
#include <QDBusObjectPath>
#include <QDBusMessage>

#include "NetworkManagerQt-export.h"
#include "generic-types.h"

namespace NetworkManager
{
class SecretAgentPrivate;

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

    enum GetSecretsFlag {
        None = 0,
        AllowInteraction = 0x01,
        RequestNew = 0x02,
        UserRequested = 0x04
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
     * and the \p callMessage is ONLY needed if \sa setDelayedReply()
     * was set to true when the method was called.
     */
    void sendError(Error error, const QString &explanation, const QDBusMessage &callMessage = QDBusMessage()) const;

public Q_SLOTS:
    /**
     * Called when the subclass should retrieve and return secrets.
     * If the request is canceled, called function should call
     * \sa sendError(), in this case the return value is ignored.
     */
    virtual NMVariantMapMap GetSecrets(const NMVariantMapMap &connection, const QDBusObjectPath &connection_path, const QString &setting_name, const QStringList &hints, uint flags) = 0;

    /**
     * Called when the subclass should cancel an outstanding request to
     * get secrets for a given connection.
     * Cancelling the request MUST \sa sendError() with the original
     * DBus message using \sa AgentCanceled param as the error type.
     */
    virtual void CancelGetSecrets(const QDBusObjectPath &connection_path, const QString &setting_name) = 0;

    /**
     * Called when the subclass should save the secrets contained in the
     * connection to backing storage.
     */
    virtual void SaveSecrets(const NMVariantMapMap &connection, const QDBusObjectPath &connection_path) = 0;

    /**
     * Called when the subclass should delete the secrets contained in the
     * connection from backing storage.
     */
    virtual void DeleteSecrets(const NMVariantMapMap &connection, const QDBusObjectPath &connection_path) = 0;

private:
    Q_DECLARE_PRIVATE(SecretAgent)
    Q_PRIVATE_SLOT(d_ptr, void registerAgent())

    SecretAgentPrivate *const d_ptr;
};
}
Q_DECLARE_OPERATORS_FOR_FLAGS(NetworkManager::SecretAgent::GetSecretsFlags)


#endif // NMQT_SECRETAGENT_H
