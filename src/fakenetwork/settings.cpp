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

#include "settings.h"

#include <QtDBus/QtDBus>

Settings::Settings(QObject *parent)
    : QObject(parent)
    , m_canModify(true)
    , m_hostname(QLatin1Literal("fake-hostname"))
    , m_connectionCounter(0)
{
    qDBusRegisterMetaType<NMVariantMapMap>();
}

Settings::~Settings()
{
    foreach (const QDBusObjectPath & connection, m_connections.keys()) {
        QDBusConnection::sessionBus().unregisterObject(connection.path());
        Q_EMIT ConnectionRemoved(connection);
    }

    qDeleteAll(m_connections);
}

bool Settings::canModify() const
{
    return m_canModify;
}

QList< QDBusObjectPath > Settings::connections() const
{
    return m_connections.keys();
}

QString Settings::hostname() const
{
    return m_hostname;
}

QDBusObjectPath Settings::AddConnection(const NMVariantMapMap &connection)
{
    Connection *newConnection = new Connection(this, connection);
    QString newConnectionPath = QString("/org/kde/fakenetwork/Settings/") + QString::number(m_connectionCounter++);
    newConnection->setConnectionPath(newConnectionPath);
    m_connections.insert(QDBusObjectPath(newConnectionPath), newConnection);
    QDBusConnection::sessionBus().registerObject(newConnectionPath, newConnection, QDBusConnection::ExportScriptableContents);

    connect(newConnection, &Connection::connectionRemoved, this, &Settings::onConnectionRemoved);

    Q_EMIT NewConnection(QDBusObjectPath(newConnectionPath));
    // Send it for FakeNetwork separately to get AvailableConnections signal after NewConnection
    Q_EMIT connectionAdded(QDBusObjectPath(newConnectionPath));

    return QDBusObjectPath(newConnectionPath);
}

QDBusObjectPath Settings::AddConnectionUnsaved(const NMVariantMapMap &connection)
{
    // TODO
    return QDBusObjectPath();
}

QDBusObjectPath Settings::GetConnectionByUuid(const QString &uuid)
{
    // TODO
    return QDBusObjectPath();
}

QList< QDBusObjectPath > Settings::ListConnections()
{
    return m_connections.keys();
}

void Settings::SaveHostname(const QString &hostname)
{
    m_hostname = hostname;
}

void Settings::onConnectionRemoved(const QDBusObjectPath &connectionPath)
{
    Connection *connection = m_connections.value(connectionPath);

    if (connection) {
        QDBusConnection::sessionBus().unregisterObject(connectionPath.path());
        Q_EMIT ConnectionRemoved(connectionPath);
        // Send it for FakeNetwork separately to get AvailableConnections signal after ConnectionRemoved
        Q_EMIT connectionRemoved(connectionPath);
        m_connections.remove(QDBusObjectPath(connectionPath));
        delete connection;
    }
}
