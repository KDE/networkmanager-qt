/*
Copyright 2008,2009 Will Stephenson <wstephenson@kde.org>
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

#include "connection.h"

#include <NetworkManager.h>
#include <nm-setting-cdma.h>
#include <nm-setting-connection.h>
#include <nm-setting-gsm.h>
#include <nm-setting-bluetooth.h>
#include <nm-setting-pppoe.h>
#include <nm-setting-vpn.h>
#include <nm-setting-wired.h>
#include <nm-setting-wireless.h>
#include <nm-setting-wimax.h>
#include <nm-setting-olpc-mesh.h>

#include <QDBusConnection>
#include <QDBusPendingReply>
#include <QDBusPendingCallWatcher>

#include "manager_p.h"
#include "device.h"
#include "activeconnection.h"
#include "nm-settings-connectioninterface.h"
#include "generic-types.h"

class NetworkManager::Settings::ConnectionPrivate
{
public:
    ConnectionPrivate(const QString &path)
    : iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
    {
    }
    void updateSettings(const NMVariantMapMap &newSettings = NMVariantMapMap());
    QString uuid;
    QString id;
    NMVariantMapMap settings;
    Settings::ConnectionSettings::Ptr connection;
    QString path;
    OrgFreedesktopNetworkManagerSettingsConnectionInterface iface;
};

NetworkManager::Settings::Connection::Connection(const QString & path, QObject * parent)
: QObject(parent), d_ptr(new ConnectionPrivate(path))
{
    Q_D(Connection);

    qDBusRegisterMetaType<NMVariantMapMap>();
    QDBusReply<NMVariantMapMap> reply = d->iface.GetSettings();
    if (reply.isValid()) {
        d->updateSettings(reply.value());
    } else {
        d->updateSettings();
    }
    d->path = path;

    //nmDebug() << m_connection;

    connect(&d->iface, SIGNAL(Updated()), this, SLOT(onConnectionUpdated()));
    connect(&d->iface, SIGNAL(Removed()), this, SLOT(onConnectionRemoved()));
}

NetworkManager::Settings::Connection::~Connection()
{
    delete d_ptr;
}

bool NetworkManager::Settings::Connection::isValid() const
{
    Q_D(const Connection);
    return d->iface.isValid();
}

QString NetworkManager::Settings::Connection::uuid() const
{
    Q_D(const Connection);
    return d->uuid;
}

QString NetworkManager::Settings::Connection::name() const
{
    Q_D(const Connection);
    return d->id;
}

NetworkManager::Settings::ConnectionSettings::Ptr NetworkManager::Settings::Connection::settings()
{
    Q_D(Connection);

    if (d->connection.isNull()) {
        d->connection = ConnectionSettings::Ptr(new ConnectionSettings(d->settings));
    }
    return d->connection;
}

void NetworkManager::Settings::Connection::secrets(const QString &setting)
{
    Q_D(Connection);
    QString id = uuid();
    QDBusPendingReply<NMVariantMapMap> reply = d->iface.GetSecrets(setting);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    watcher->setProperty("libQtNetworkManager_id", id);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), this,  SLOT(onSecretsArrived(QDBusPendingCallWatcher*)));
}

void NetworkManager::Settings::Connection::onSecretsArrived(QDBusPendingCallWatcher *watcher)
{
    Q_D(Connection);
    if (!watcher)
        return;
    QDBusPendingReply<NMVariantMapMap> reply = *watcher;
    QString id = watcher->property("libQtNetworkManager_id").value<QString>();
    bool success = true;
    NMVariantMapMap set;
    QString message;
    if (!reply.isValid()) {
        success = false;
        message = reply.error().message();
    } else {
        set = reply.argumentAt<0>();
    }
    emit gotSecrets(id, success, set, message);
    watcher->deleteLater();
}

bool NetworkManager::Settings::Connection::active() const
{
    Q_D(const Connection);
    ActiveConnection::List activeConnections = NetworkManager::activeConnections();
    foreach (const ActiveConnection::Ptr &activeConnection, activeConnections) {
        if (activeConnection->connection() == this) {
            return true;
        }
    }
    return false;
}

void NetworkManager::Settings::Connection::update(const NMVariantMapMap &settings)
{
    Q_D(Connection);
    d->iface.Update(settings);
}

void NetworkManager::Settings::Connection::remove()
{
    Q_D(Connection);
    d->iface.Delete();
}

QString NetworkManager::Settings::Connection::path() const
{
    Q_D(const Connection);
    return d->path;
}

void NetworkManager::Settings::Connection::onConnectionUpdated()
{
    Q_D(Connection);
    QDBusReply<NMVariantMapMap> reply = d->iface.GetSettings();
    if (reply.isValid()) {
        d->updateSettings(reply.value());
    } else {
        d->updateSettings();
    }
    emit updated();
}

void NetworkManager::Settings::Connection::onConnectionRemoved()
{
    Q_D(Connection);
    QString path = d->path;
    d->updateSettings();
    emit removed(path);
}

void NetworkManager::Settings::ConnectionPrivate::updateSettings(const NMVariantMapMap &newSettings)
{
    settings = newSettings;
    if (settings.contains(QLatin1String(NM_SETTING_CONNECTION_SETTING_NAME))) {
        QVariantMap connectionSetting = settings.value(QLatin1String(NM_SETTING_CONNECTION_SETTING_NAME));
        if (connectionSetting.contains(QLatin1String(NM_SETTING_CONNECTION_UUID))) {
            uuid = connectionSetting.value(QLatin1String(NM_SETTING_CONNECTION_UUID)).toString();
        }
        if (connectionSetting.contains(QLatin1String(NM_SETTING_CONNECTION_ID))) {
            id = connectionSetting.value(QLatin1String(NM_SETTING_CONNECTION_ID)).toString();
        }
    } else if (newSettings.isEmpty()) {
        uuid.clear();
        id.clear();
    }
    connection.clear();
}

// vim: sw=4 sts=4 et tw=100

#include "connection.moc"
