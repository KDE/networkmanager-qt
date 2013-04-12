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

#include "secretagent.h"
#include "secretagent_p.h"
#include "manager_p.h"

#include <NetworkManager.h>

#include <QHash>
#include <QUuid>

#include <QDBusArgument>
#include <QDBusConnection>

#include "nm-secret-agentadaptor.h"
#include "nm-agent-managerinterface.h"

NetworkManager::SecretAgentPrivate::SecretAgentPrivate(const QString &id, NetworkManager::SecretAgent *parent)
: q_ptr(parent), agent(parent), agentManager(NetworkManagerPrivate::DBUS_SERVICE, QLatin1String(NM_DBUS_PATH_AGENT_MANAGER), QDBusConnection::systemBus(), parent),
watcher(NetworkManagerPrivate::DBUS_SERVICE, QDBusConnection::systemBus(), QDBusServiceWatcher::WatchForRegistration, parent), agentId(id)
{
    Q_Q(SecretAgent);
    qRegisterMetaType<QVariantMapMap>("QVariantMapMap");
    qDBusRegisterMetaType<QVariantMapMap>();

    watcher.connect(&watcher, SIGNAL(serviceRegistered(QString)), q, SLOT(registerAgent()));
    agentManager.connection().registerObject(QLatin1String(NM_DBUS_PATH_SECRET_AGENT), &agent, QDBusConnection::ExportAllSlots);

    registerAgent();
}

NetworkManager::SecretAgentPrivate::~SecretAgentPrivate()
{
    agentManager.Unregister();
}

void NetworkManager::SecretAgentPrivate::registerAgent()
{
    agentManager.Register(agentId);
}

NetworkManager::SecretAgent::SecretAgent(const QString &id, QObject * parent)
: QObject(parent), QDBusContext(), d_ptr(new NetworkManager::SecretAgentPrivate(id, this))
{
}

NetworkManager::SecretAgent::~SecretAgent()
{
    Q_D(SecretAgent);
    delete d;
}

#include "secretagent.moc"
