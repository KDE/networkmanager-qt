/*
Copyright 2008,2009 Will Stephenson <wstephenson@kde.org>
Copyright 2011-2013 Lamarque V. Souza <lamarque@kde.org>

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

#ifndef NMQT_SETTINGS_CONNECTION_H
#define NMQT_SETTINGS_CONNECTION_H

#include "NetworkManagerQt-export.h"
#include "generic-types.h"
#include "settings/connectionsettings.h"

#include <QObject>
#include <QSharedPointer>

class QDBusPendingCallWatcher;

namespace NetworkManager
{

class ConnectionPrivate;

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
     * Returns the settings of this connection
     */
    ConnectionSettings::Ptr settings();

    /**
     * Retrieves this connections's secrets (passwords and / or encryption keys).
     * This is an asynchronous call, connect to the gotSecrets signal to
     * read the retrieved secrets.
     *
     * @param setting the setting identifier.
     */
    void secrets(const QString &setting);

    void update(const NMVariantMapMap &);

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
     * @param id connections's path.
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
