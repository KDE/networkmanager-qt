/*
    Copyright 2014 Jan Grulich <jgrulich@redhat.com>

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

#ifndef NETWORKMANAGERQT_FAKE_NETWORK_SETTINGS_H
#define NETWORKMANAGERQT_FAKE_NETWORK_SETTINGS_H

#include <QObject>

#include <QDBusObjectPath>
#include <QDBusPendingReply>

#include "../generictypes.h"

#include "connection.h"

class Settings : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.fakenetwork.Settings")
public:
    explicit Settings(QObject *parent = nullptr);
    virtual ~Settings();

    Q_PROPERTY(bool CanModify READ canModify)
    Q_PROPERTY(QList<QDBusObjectPath> Connections READ connections)
    Q_PROPERTY(QString Hostname READ hostname)

    bool canModify() const;
    QList<QDBusObjectPath> connections() const;
    QString hostname() const;

private Q_SLOTS:
    void onConnectionRemoved(const QDBusObjectPath &connectionPath);

public Q_SLOTS: // METHODS
    Q_SCRIPTABLE QDBusObjectPath AddConnection(const NMVariantMapMap &connection);
    Q_SCRIPTABLE QDBusObjectPath AddConnectionUnsaved(const NMVariantMapMap &connection);
    Q_SCRIPTABLE QDBusObjectPath GetConnectionByUuid(const QString &uuid);
    Q_SCRIPTABLE QList<QDBusObjectPath> ListConnections();
    Q_SCRIPTABLE void SaveHostname(const QString &hostname);

Q_SIGNALS:
    void connectionAdded(const QDBusObjectPath &connection);
    void connectionRemoved(const QDBusObjectPath &connection);
    Q_SCRIPTABLE void ConnectionRemoved(const QDBusObjectPath &connection);
    Q_SCRIPTABLE void NewConnection(const QDBusObjectPath &connection);
    Q_SCRIPTABLE void PropertiesChanged(const QVariantMap &properties);
private:
    bool m_canModify;
    QMap<QDBusObjectPath, Connection *> m_connections;
    QString m_hostname;

    /* Not part of DBus interface */
    int m_connectionCounter;
};

#endif
