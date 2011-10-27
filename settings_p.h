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

#ifndef NM_SETTINGS_P_H
#define NM_SETTINGS_P_H

#include <QObject>
#include <QDBusObjectPath>

#include "dbus/nm-settingsinterface.h"
#include "settings.h"

class QDBusPendingCallWatcher;

namespace NetworkManager
{
namespace Settings
{
    class Connection;
    class SettingsPrivate : public NetworkManager::Settings::Notifier
    {
    Q_OBJECT
    public:
        SettingsPrivate();
        QList<NetworkManager::Settings::Connection*> listConnections();
        QString hostname();
        bool canModify();
        QString addConnection(const QVariantMapMap &);
        void saveHostname(const QString &);
        Connection *findRegisteredConnection(const QString &);

        QMap<QString, Connection*> connections;
        bool m_canModify;
        QString m_hostname;
        OrgFreedesktopNetworkManagerSettingsInterface iface;
    protected Q_SLOTS:
        void slotConnectionAdded(const QDBusObjectPath &);
        void connectionRemoved(const QString &);
        void propertiesChanged(const QVariantMap &);
        void onConnectionAddArrived(QDBusPendingCallWatcher *);
        void daemonUnregistered();
        void init();
    };
}
}


#endif