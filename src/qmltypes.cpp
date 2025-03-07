// SPDX-FileCopyrightText: 2025 Carl Schwan <carl@carlschwan.eu>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "qmltypes.h"
#include "settings.h"
#include <QQmlEngine>

ConnectionShared::ConnectionShared()
{
}

ConnectionShared::ConnectionShared(const ConnectionShared &shared)
    : mPtr(shared.mPtr)
{
}

ConnectionShared::ConnectionShared(const NetworkManager::Connection::Ptr &ptr)
    : mPtr(ptr)
{
}

NetworkManager::Connection *ConnectionShared::connection() const
{
    const auto value = mPtr.get();
    if (value) {
        QQmlEngine::setObjectOwnership(value, QQmlEngine::CppOwnership);
    }
    return value;
}

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)
{
    auto settingsNotifier = NetworkManager::settingsNotifier();
    connect(settingsNotifier, &NetworkManager::SettingsNotifier::hostnameChanged, this, &SettingsManager::hostnameChanged);
    connect(settingsNotifier, &NetworkManager::SettingsNotifier::canModifyChanged, this, &SettingsManager::canModifyChanged);
}

ConnectionShared SettingsManager::findConnection(const QString &path) const
{
    return ConnectionShared{NetworkManager::findConnection(path)};
}

ConnectionShared SettingsManager::findConnectionByUuid(const QString &uuid)
{
    return ConnectionShared{NetworkManager::findConnectionByUuid(uuid)};
}

ConnectionShared::List SettingsManager::listConnections() const
{
    ConnectionShared::List result;
    const auto connections = NetworkManager::listConnections();
    for (const auto &connection : connections) {
        result << ConnectionShared{connection};
    }
    return result;
}

QString SettingsManager::hostname() const
{
    return NetworkManager::hostname();
}

void SettingsManager::setHostname(const QString &hostname)
{
    if (hostname == NetworkManager::hostname()) {
        return;
    }
    NetworkManager::saveHostname(hostname);
}

bool SettingsManager::canModify() const
{
    return NetworkManager::canModify();
}
