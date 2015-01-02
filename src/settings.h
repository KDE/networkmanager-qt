/*
    Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
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

#ifndef NETWORKMANAGERQT_SETTINGS_H
#define NETWORKMANAGERQT_SETTINGS_H

#include <networkmanagerqt_export.h>

#include <QObject>
#include "generictypes.h"
#include "manager.h"
#include "connection.h"

#include <QString>
#include <QUuid>

namespace NetworkManager
{

/**
 * This class manages provides access to connections and notify about new ones
 */
class NETWORKMANAGERQT_EXPORT SettingsNotifier : public QObject
{
    Q_OBJECT
Q_SIGNALS:
    /**
     * Emitted when the settings are modifiable by user
     * @param canModify @p true if the user can modify the settings
     */
    void canModifyChanged(bool canModify);
    /**
     * Emitted when the hostname has changed
     * @param hostname new hostname
     */
    void hostnameChanged(const QString &hostname);
    /**
     * Emitted when a new connection is added
     *
     * \note This signal is not emitted when the Network Manager
     * daemon starts, if you are interested in keeping an
     * updated listing of connections you must also watch for
     * NetworkManager::Notifier::serviceAppeared() and
     * NetworkManager::Notifier::serviceDisappeared() signals
     */
    void connectionAdded(const QString &path);
    /**
     * Emitted when a new connection is removed
     *
     * \note This signal is not emitted when the Network Manager
     * daemon starts, if you are interested in keeping an
     * updated listing of connections you must also watch for
     * NetworkManager::Notifier::serviceAppeared() and
     * NetworkManager::Notifier::serviceDisappeared() signals
     */
    void connectionRemoved(const QString &path);
};
/**
 * Retrieves the list of connections.
 */
NETWORKMANAGERQT_EXPORT NetworkManager::Connection::List listConnections();

/**
 * Retrieves the connection for the given path, returns null if not found
 */
NETWORKMANAGERQT_EXPORT NetworkManager::Connection::Ptr findConnection(const QString &path);

/**
 * Add new connection and save it to disk. This operation does not start
 * the network connection unless (1) device is idle and able to connect to
 * the network described by the new connection, and (2) the connection
 * is allowed to be started automatically.
 * Once the connection has been added, you will get a notification through
 * SettingsNotifier::connectionAddComplete()
 *
 * @returns Uuid of the new connection that was just added.
 *
 * @since 0.9.9.0
 */
NETWORKMANAGERQT_EXPORT QDBusPendingReply<QDBusObjectPath> addConnection(const NMVariantMapMap &settings);

#if NM_CHECK_VERSION(0, 9, 10)
/**
 * Add new connection but do not save it to disk immediately.  This
 * operation does not start the network connection unless (1) device is
 * idle and able to connect to the network described by the new connection,
 * and (2) the connection is allowed to be started automatically.
 *
 * Use the 'Save' method on the connection to save these changes
 * to disk. Note that unsaved changes will be lost if the
 * connection is reloaded from disk (either automatically on file
 * change or due to an explicit ReloadConnections call).
 *
 * Once the connection has been added, you will get a notification through
 * SettingsNotifier::connectionAddComplete()
 *
 * @returns Uuid of the new connection that was just added.
 *
 * @since 0.9.9.0
 */
NETWORKMANAGERQT_EXPORT QDBusPendingReply<QDBusObjectPath> addConnectionUnsaved(const NMVariantMapMap &settings);
#endif

/**
 * Retrieves the connection for the given @p uuid, returns null if not found
 */
NETWORKMANAGERQT_EXPORT NetworkManager::Connection::Ptr findConnectionByUuid(const QString &uuid);

#if NM_CHECK_VERSION(0, 9, 10)
/**
 * Loads or reloads the indicated connections from disk. You
 * should call this after making changes directly to an on-disk
 * connection file to make sure that NetworkManager sees the
 * changes. (If "monitor-connection-files" in NetworkManager.conf
 * is "true", then this will have no real effect, but is
 * harmless.) As with AddConnection(), this operation does not
 * necessarily start the network connection.
 *
 * @returns  Success or failure of the operation as a whole. True if
 *           NetworkManager at least tried to load the indicated
 *           connections, even if it did not succeed. False if an error
 *           occurred before trying to load the connections (eg,
 *           permission denied).
 *
 * @returns  Paths of connection files that could not be loaded.
 *
 * @since 0.9.9.0
 */
NETWORKMANAGERQT_EXPORT QDBusPendingReply<bool, QStringList> loadConnections(const QStringList &filenames);

/**
 * Tells NetworkManager to reload all connection files from disk,
 * including noticing any added or deleted connection files. By
 * default, connections are re-read automatically any time they
 * change, so you only need to use this command if you have set
 * "monitor-connection-files=false" in NetworkManager.conf.
 *
 * @returns Success or failure.
 *
 * @since 0.9.9.0
 */
NETWORKMANAGERQT_EXPORT QDBusPendingReply<bool> reloadConnections();
#endif

/**
 * Configure the following hostname
 */
NETWORKMANAGERQT_EXPORT void saveHostname(const QString &hostname);

/**
 * Returns @p true if the user can modify the settings
 */
NETWORKMANAGERQT_EXPORT bool canModify();

/**
 * Returns hostname of the machine
 */
NETWORKMANAGERQT_EXPORT QString hostname();

/**
 * Notifier object for connecting signals
 */
NETWORKMANAGERQT_EXPORT SettingsNotifier *settingsNotifier();
}

#endif
