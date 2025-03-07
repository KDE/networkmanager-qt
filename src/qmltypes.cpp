// SPDX-FileCopyrightText: 2025 Carl Schwan <carl@carlschwan.eu>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "qmltypes.h"
#include "settings.h"
#include <QQmlEngine>

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)
{
    auto settingsNotifier = NetworkManager::settingsNotifier();
    connect(settingsNotifier, &NetworkManager::SettingsNotifier::hostnameChanged, this, &SettingsManager::hostnameChanged);
    connect(settingsNotifier, &NetworkManager::SettingsNotifier::canModifyChanged, this, &SettingsManager::canModifyChanged);
}

NetworkManager::Connection *SettingsManager::findConnection(const QString &path) const
{
    const auto value = NetworkManager::findConnection(path).get();
    if (value) {
        QQmlEngine::setObjectOwnership(value, QQmlEngine::CppOwnership);
    }
    return value;
}

NetworkManager::Connection *SettingsManager::findConnectionByUuid(const QString &uuid)
{
    const auto value = NetworkManager::findConnectionByUuid(uuid).get();
    if (value) {
        QQmlEngine::setObjectOwnership(value, QQmlEngine::CppOwnership);
    }
    return value;
}

NetworkManager::Connection::List SettingsManager::listConnections() const
{
    return NetworkManager::listConnections();
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
