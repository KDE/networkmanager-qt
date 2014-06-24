/*
    Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
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

#ifndef NETWORKMANAGERQT_SETTINGS_P_H
#define NETWORKMANAGERQT_SETTINGS_P_H

#include <QObject>
#include <QDBusObjectPath>

#include "dbus/nm-settingsinterface.h"
#include "settings.h"

class QDBusPendingCallWatcher;

namespace NetworkManager
{

class SettingsPrivate : public NetworkManager::SettingsNotifier
{
    Q_OBJECT
    friend class NetworkManagerPrivate;
public:
    SettingsPrivate();
    Connection::List listConnections();
    NetworkManager::Connection::Ptr findConnectionByUuid(const QString &uuid);
    QString hostname() const;
    bool canModify() const;
    QDBusPendingReply<QDBusObjectPath> addConnection(const NMVariantMapMap &);
#if NM_CHECK_VERSION(0, 9, 10)
    QDBusPendingReply<QDBusObjectPath> addConnectionUnsaved(const NMVariantMapMap &);
    QDBusPendingReply<bool, QStringList> loadConnections(const QStringList& filenames);
#endif
    void saveHostname(const QString &);
#if NM_CHECK_VERSION(0, 9, 10)
    QDBusPendingReply<bool> reloadConnections();
#endif
    Connection::Ptr findRegisteredConnection(const QString &);

    OrgFreedesktopNetworkManagerSettingsInterface iface;
    QMap<QString, Connection::Ptr> connections;
    bool m_canModify;
    QString m_hostname;
protected Q_SLOTS:
    void onConnectionAdded(const QDBusObjectPath &);
#if NM_CHECK_VERSION(0, 9, 10)
    void onConnectionRemoved(const QDBusObjectPath &);
#else
    void onConnectionRemoved(const QString &);
#endif
    void propertiesChanged(const QVariantMap &properties);
    void initNotifier();
protected:
    void daemonUnregistered();
    void init();
};

}


#endif
