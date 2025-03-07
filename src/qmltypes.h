/*
    SPDX-FileCopyrightText: 2023 Nicolas Fella <nicolas.fella@gmx.de>
    SPDX-FileCopyrightText: 2025 Carl Schwan <carl@carlschwan.eu>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include <manager.h>

#include <qqmlregistration.h>

namespace NMForeign
{
Q_NAMESPACE
QML_NAMED_ELEMENT(NetworkManager)
QML_FOREIGN_NAMESPACE(NetworkManager)
};

class ConnectionForeign : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Connection)
    QML_FOREIGN(NetworkManager::Connection)
    QML_UNCREATABLE("Get the connection from the SettingsManager singleton instead")
};

class ConnectionShared
{
    Q_GADGET
    QML_VALUE_TYPE(connectionPtr)
    Q_PROPERTY(NetworkManager::Connection *connection READ connection)

public:
    using List = QList<ConnectionShared>;
    explicit ConnectionShared();
    explicit ConnectionShared(const ConnectionShared &connection);
    explicit ConnectionShared(const NetworkManager::Connection::Ptr &ptr);

    [[nodiscard]] NetworkManager::Connection *connection() const;

private:
    NetworkManager::Connection::Ptr mPtr;
};

/*!
 Expose functions from settings.h to QML.
*/
class SettingsManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    /*!
        qmlproperty string SettingsManager::hostname
        This property holds the hostname of the machine.
    */
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)

    /*!
        qmlproperty bool SettingsManager::canModify
        This property holds whether the settings can be modified.
    */
    Q_PROPERTY(bool canModify READ canModify NOTIFY canModifyChanged)

public:
    explicit SettingsManager(QObject *parent = nullptr);

    /*! Retrieves the list of connections */
    Q_INVOKABLE ConnectionShared findConnection(const QString &path) const;
    /* Retrieves the connection for the given @p uuid, returns null if not found*/
    Q_INVOKABLE ConnectionShared findConnectionByUuid(const QString &uuid);
    /*! Retrieves the connection for the given path, returns null if not found */
    Q_INVOKABLE ConnectionShared::List listConnections() const;

    QString hostname() const;
    void setHostname(const QString &hostname);

    bool canModify() const;

Q_SIGNALS:
    /*! Emitted when the settings are modifiable by user */
    void canModifyChanged(bool canModify);
    /*! Emitted when the hostname has changed */
    void hostnameChanged(const QString &hostname);
};
