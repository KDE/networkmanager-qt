/*
Copyright 2008,2009 Will Stephenson <wstephenson@kde.org>

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

#ifndef NM_SETTINGS_CONNECTION_H
#define NM_SETTINGS__CONNECTION_H

#include "nmqt-export.h"

#include <QObject>
#include "device.h"
#include "manager.h"
#include "generic-types.h"

class QDBusPendingCallWatcher;

namespace NetworkManager
{
namespace Settings
{
    class ConnectionPrivate;
    class NMQT_EXPORT Connection : public QObject
    {
    Q_OBJECT
    Q_DECLARE_PRIVATE(Connection)
    public:
        explicit Connection(const QString &path, QObject * parent = 0);
        ~Connection();
        QString uuid() const;
        QString path() const;
        QString id() const;
        QVariantMapMap settings() const;

        /**
         * Retrieves this connections's secrets (passwords and / or encryption keys).
         * This is an asynchronous call, connect to the gotSecrets signal to
         * read the retrieved secrets.
         *
         * @param setting the setting identifier.
         */
        void secrets(const QString &setting);
        bool active() const;
        void update(const QVariantMapMap &);
        void remove();
    private:
        ConnectionPrivate *d_ptr;
    private Q_SLOTS:
        void secretsArrived(QDBusPendingCallWatcher *);
        void connectionUpdated();
        void connectionRemoved();
    Q_SIGNALS:
        /**
         * Reports the secrets retrieved.
         *
         * @param id connections's uuid.
         * @param success true if secrets were correctly retrieved or false for error.
         * @param set secrets retrieved.
         * @param message error message if any, empty string if success is true.
         */
        void gotSecrets(const QString &id, bool success, const QVariantMapMap &set, const QString &message);
        void updated(const QVariantMapMap &);
        void removed(const QString &);
    };
}
}
#endif // CONNECTION_H
