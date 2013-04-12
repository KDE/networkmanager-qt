/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>

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

#ifndef NMQT_SETTINGS_H
#define NMQT_SETTINGS_H

#include "QtNetworkManager-export.h"

#include <QObject>
#include "generic-types.h"
#include "manager.h"
#include "connection.h"

#include <QString>
#include <QUuid>

namespace NetworkManager
{
namespace Settings
{
    class Connection;
    class NMQT_EXPORT Notifier : public QObject
    {
    Q_OBJECT
    Q_SIGNALS:
        void canModifyChanged(bool);
        void hostnameChanged(const QString &);
        /**
         * Emitted when a new connection is added
         *
         * \note This signal is not emitted when the Network Manager
         * daemon starts, if you are interested in keeping an
         * updated listing of connections you must also watch for
         * NetworkManager::serviceAppeared and
         * NetworkManager::serviceDisappeared signals
         */
        void connectionAdded(const QString &path);
        /**
         * Emitted when a new connection is removed
         *
         * \note This signal is not emitted when the Network Manager
         * daemon starts, if you are interested in keeping an
         * updated listing of connections you must also watch for
         * NetworkManager::serviceAppeared and
         * NetworkManager::serviceDisappeared signals
         */
        void connectionRemoved(const QString &path);
        void connectionAddComplete(const QString &, bool, const QString &);
    };
    /**
     * Retrieves the list of connections.
     */
    NMQT_EXPORT NetworkManager::Settings::Connection::List listConnections();

    /**
     * Retrieves the connection for the given path, returns null if not found
     */
    NMQT_EXPORT NetworkManager::Settings::Connection::Ptr findConnection(const QString &path);

    /**
     * Adds the connection with the given settings, returns the id for tracking
     */
    NMQT_EXPORT QString addConnection(const NMVariantMapMap &settings);

    /**
     * Retrieves the connection for the given uuid, returns null if not found
     */
    NMQT_EXPORT NetworkManager::Settings::Connection::Ptr findConnectionByUuid(const QString &uuid);

    /**
     * Configure the following hostname
     */
    NMQT_EXPORT void saveHostname(const QString &hostname);

    /**
     * Returns if the user can modify the settings
     */
    NMQT_EXPORT bool canModify();

    /**
     * Returns hostname of the machine
     */
    NMQT_EXPORT QString hostname();

    /**
     * Notifier object for connecting signals
     */
    NMQT_EXPORT Notifier* notifier();
}
}


#endif
