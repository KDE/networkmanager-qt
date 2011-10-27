/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NM_SETTINGS_H
#define NM_SETTINGS_H

#include "nmqt-export.h"

#include <QObject>
#include "generic-types.h"
#include "manager.h"

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
        void connectionAdded(const QString &);
        void connectionRemoved(const QString &);
        void connectionAddComplete(const QString &, bool, const QString &);
    };
    NMQT_EXPORT QList<NetworkManager::Settings::Connection*> listConnections();
    NMQT_EXPORT NetworkManager::Settings::Connection* findConnection(const QString &);
    NMQT_EXPORT QString addConnection(const QVariantMapMap &);
    NMQT_EXPORT void saveHostname(const QString &);
    NMQT_EXPORT bool canModify();
    NMQT_EXPORT QString hostname();
    NMQT_EXPORT Notifier* notifier();
}
}


#endif