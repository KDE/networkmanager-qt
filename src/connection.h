/*
    Copyright 2008,2009 Will Stephenson <wstephenson@kde.org>
    Copyright 2011-2013 Lamarque V. Souza <lamarque@kde.org>
    Copyright 2013 Jan Grulich <jgrulich@redhat.com>

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

#ifndef NETWORKMANAGERQT_SETTINGS_CONNECTION_H
#define NETWORKMANAGERQT_SETTINGS_CONNECTION_H

#include "NetworkManagerQt-export.h"
#include "generictypes.h"
#include "settings/connectionsettings.h"

#include <QObject>
#include <QSharedPointer>

class QDBusPendingCallWatcher;

namespace NetworkManager
{

class ConnectionPrivate;

/**
 * This class represents a single network connection configuration.
 */
class NETWORKMANAGERQT_EXPORT Connection : public QObject
{
    Q_OBJECT
public:
    typedef QSharedPointer<Connection> Ptr;
    typedef QList<Ptr> List;

    /**
     * Constructs a connection object for the given path
     */
    explicit Connection(const QString &path, QObject *parent = 0);
    ~Connection();

    /**
     * Returns if this connection is valid
     */
    bool isValid() const;

    /**
     * Returns the unique identifier of this connection
     */
    QString uuid() const;

    /**
     * Returns the path (DBus) of this connection
     */
    QString path() const;

    /**
     * Returns the name of this connection
     */
    QString name() const;

    /**
     * If set, indicates that the in-memory state of the
     * connection does not match the on-disk state. This flag
     * will be set when updateUnsaved() is called or when any
     * connection details change, and cleared when the connection
     * is saved to disk via save() or from internal operations.
     *
     * @since 0.9.9.0
     */
    bool isUnsaved() const;

    /**
     * Returns the settings of this connection
     */
    ConnectionSettings::Ptr settings();

    /**
     * Retrieves this connections's secrets (passwords and / or encryption keys).
     * This is an asynchronous call, connect to the gotSecrets() signal to
     * read the retrieved secrets.
     *
     * @param setting the setting identifier.
     */
    void secrets(const QString &setting);

    /**
     * Update the connection with new @p settings and properties, replacing all previous settings and properties.
     * Secrets may be part of the update request, and will be either stored in persistent storage or given to a Secret Agent for storage,
     * depending on the request.
     */
    void update(const NMVariantMapMap & settings);

    /**
     * Update the connection with new @p settings and properties (replacing
     * all previous settings and properties) but do not immediately save
     * the connection to disk. Secrets may be part of the update request
     * and may sent to a Secret Agent for storage, depending on the the
     * flags associated with each secret.
     *
     * Use the save() method to save these changes to disk. Note
     * that unsaved changes will be lost if the connection is
     * reloaded from disk (either automatically on file change or
     * due to an explicit reloadConnections() call).
     *
     * @since 0.9.9.0
     */
    void updateUnsaved(const NMVariantMapMap & settings);

    /**
     * Saves a "dirty" connection (that had previously been
     * updated with updateUnsaved()) to persistent storage.
     *
     * @since 0.9.9.0
     */
    void save();

    /**
     * Removes the connection from NetworkManager database,
     * this operation does not ask for confirmation but
     * a policykit rule might prevent it from being removed
     * without the proper password.
     */
    void remove();

Q_SIGNALS:
    /**
     * Reports the secrets retrieved.
     *
     * @param id connections's uuid.
     * @param success true if secrets were correctly retrieved or false for error.
     * @param set secrets retrieved.
     * @param message error message if any, empty string if success is true.
     */
    void gotSecrets(const QString &id, bool success, const NMVariantMapMap &set, const QString &message);

    /**
     * Emitted when the connection settings changes
     */
    void updated();

    /**
     * Emitted when the connection was removed
     * @param path connections's path.
     */
    void removed(const QString &path);

private Q_SLOTS:
    void onSecretsArrived(QDBusPendingCallWatcher *);
    void onConnectionUpdated();
    void onConnectionRemoved();

private:
    Q_DECLARE_PRIVATE(Connection)

    ConnectionPrivate *const d_ptr;
};

}
#endif // CONNECTION_H
