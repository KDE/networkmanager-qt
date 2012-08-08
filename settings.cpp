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

#include "settings.h"
#include "settings_p.h"
#include "macros.h"
#include "connection.h"
#include "manager_p.h"

#include <QDBusObjectPath>

#include <nm-setting-connection.h>

NM_GLOBAL_STATIC(NetworkManager::Settings::SettingsPrivate, globalSettings)

NetworkManager::Settings::SettingsPrivate::SettingsPrivate()
    : iface(NetworkManagerPrivate::DBUS_SERVICE, NetworkManagerPrivate::DBUS_SETTINGS_PATH, QDBusConnection::systemBus())
{
    connect(NetworkManager::notifier(), SIGNAL(serviceDisappeared()), this, SLOT(daemonUnregistered()));
    connect(NetworkManager::notifier(), SIGNAL(serviceAppeared()), this, SLOT(init()));

    if (NetworkManager::status() != NetworkManager::Unknown) {
        init();
    }
}

void NetworkManager::Settings::SettingsPrivate::init()
{
    QDBusPendingReply<QList<QDBusObjectPath> > reply = iface.ListConnections();
    reply.waitForFinished();
    if (!reply.isError()) {
        foreach (const QDBusObjectPath &path, reply.value()) {
            QString id = path.path();
            connections.insert(id, 0);
        }
    }
    m_canModify = iface.canModify();
    m_hostname = iface.hostname();

    connect(&iface, SIGNAL(PropertiesChanged(QVariantMap)), this, SLOT(propertiesChanged(QVariantMap)));
    connect(&iface, SIGNAL(NewConnection(QDBusObjectPath)), this, SLOT(slotConnectionAdded(QDBusObjectPath)));
}

QList<NetworkManager::Settings::Connection*> NetworkManager::Settings::SettingsPrivate::listConnections()
{
    QList<NetworkManager::Settings::Connection*> list;
    QMap<QString, Connection*>::const_iterator i;
    for (i = connections.constBegin(); i != connections.constEnd(); ++i) {
        list.append(findRegisteredConnection(i.key()));
    }
    return list;
}

QString NetworkManager::Settings::SettingsPrivate::hostname() const
{
    return m_hostname;
}

bool NetworkManager::Settings::SettingsPrivate::canModify() const
{
    return m_canModify;
}

QString NetworkManager::Settings::SettingsPrivate::addConnection(const QVariantMapMap &connection)
{
    QDBusPendingReply<QDBusObjectPath> reply = iface.AddConnection(connection);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, 0);
    QVariantMap connectionSettings = connection.value(QLatin1String(NM_SETTING_CONNECTION_SETTING_NAME));    
    QString id = connectionSettings.value(QLatin1String(NM_SETTING_CONNECTION_UUID)).toString();
    watcher->setProperty("libQtNetworkManager_id", id);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(onConnectionAddArrived(QDBusPendingCallWatcher*)));
    return id;
}

void NetworkManager::Settings::SettingsPrivate::onConnectionAddArrived(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<QDBusObjectPath> reply = *watcher;
    QString id = watcher->property("libQtNetworkManager_id").value<QString>();
    QString message;
    bool success = true;
    if (!reply.isValid()) {
        message = reply.error().message();
        success = false;
    } else {
        message = reply.argumentAt<0>().path();
    }
    emit connectionAddComplete(id, success, reply.error().message());
    watcher->deleteLater();
}

void NetworkManager::Settings::SettingsPrivate::saveHostname(const QString &hostname)
{
    iface.SaveHostname(hostname);
}

void NetworkManager::Settings::SettingsPrivate::propertiesChanged(const QVariantMap &changedProperties)
{
    QStringList propKeys = changedProperties.keys();
    QLatin1String canModifyKey("CanModify"),
                  hostnameKey("Hostname");
    QVariantMap::const_iterator it = changedProperties.find(canModifyKey);
    if (it != changedProperties.end()) {
        m_canModify = it->toBool();
        emit canModifyChanged(m_canModify);
        propKeys.removeOne(canModifyKey);
    }
    it = changedProperties.find(hostnameKey);
    if (it != changedProperties.end()) {
        m_hostname = it->toString();
        emit hostnameChanged(m_hostname);
        propKeys.removeOne(hostnameKey);
    }
    //if (propKeys.count()) {
    //    nmDebug() << "Unhandled properties: " << propKeys;
    //}
}

void NetworkManager::Settings::SettingsPrivate::slotConnectionAdded(const QDBusObjectPath &path)
{
    QString id = path.path();
    if (connections.contains(id))
        return;
    connections.insert(id, 0);
    emit connectionAdded(id);
}

NetworkManager::Settings::Connection* NetworkManager::Settings::SettingsPrivate::findRegisteredConnection(const QString &path)
{
    NetworkManager::Settings::Connection *rc = 0;
    if (connections.contains(path) && connections.value(path) != 0) {
        rc = connections.value(path);
    } else {
        rc = new NetworkManager::Settings::Connection(path, this);
        connections.insert(path, rc);
        connect(rc, SIGNAL(removed(QString)), this, SLOT(onConnectionRemoved(QString)));
    }
    return rc;
}

void NetworkManager::Settings::SettingsPrivate::onConnectionRemoved(const QString &path)
{
    NetworkManager::Settings::Connection *rc = connections.take(path);
    emit connectionRemoved(path);
    rc->deleteLater();
}

void NetworkManager::Settings::SettingsPrivate::daemonUnregistered()
{
    QMap<QString, Connection*>::const_iterator i;
    for (i = connections.constBegin(); i != connections.constEnd(); ++i) {
        onConnectionRemoved(i.key());
    }
    connections.clear();
}

QList<NetworkManager::Settings::Connection*> NetworkManager::Settings::listConnections()
{
    return globalSettings->listConnections();
}

NetworkManager::Settings::Connection* NetworkManager::Settings::findConnection(const QString &path)
{
    return globalSettings->findRegisteredConnection(path);
}

QString NetworkManager::Settings::addConnection(const QVariantMapMap &connection)
{
    return globalSettings->addConnection(connection);
}

void NetworkManager::Settings::saveHostname(const QString &hostname)
{
    globalSettings->saveHostname(hostname);
}

bool NetworkManager::Settings::canModify()
{
    return globalSettings->canModify();
}

QString NetworkManager::Settings::hostname()
{
    return globalSettings->hostname();
}

NetworkManager::Settings::Notifier* NetworkManager::Settings::notifier()
{
    return globalSettings;
}
