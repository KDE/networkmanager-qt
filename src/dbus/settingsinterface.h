/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was:
 *
 * qdbusxml2cpp is Copyright (C) 2016 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef SETTINGSINTERFACE_H
#define SETTINGSINTERFACE_H

#include "generictypes.h"

#include <QDBusAbstractInterface>
#include <QDBusObjectPath>
#include <QDBusPendingReply>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>

/*
 * Proxy class for interface org.freedesktop.NetworkManager.Settings
 */
class OrgFreedesktopNetworkManagerSettingsInterface : public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
#ifdef NMQT_STATIC
    {
        return "org.kde.fakenetwork.Settings";
    }
#else
    {
        return "org.freedesktop.NetworkManager.Settings";
    }
#endif

public:
    OrgFreedesktopNetworkManagerSettingsInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~OrgFreedesktopNetworkManagerSettingsInterface() override;

    Q_PROPERTY(bool CanModify READ canModify)
    inline bool canModify() const
    {
        return qvariant_cast<bool>(property("CanModify"));
    }

    Q_PROPERTY(QList<QDBusObjectPath> Connections READ connections)
    inline QList<QDBusObjectPath> connections() const
    {
        return qvariant_cast<QList<QDBusObjectPath>>(property("Connections"));
    }

    Q_PROPERTY(QString Hostname READ hostname)
    inline QString hostname() const
    {
        return qvariant_cast<QString>(property("Hostname"));
    }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QDBusObjectPath> AddConnection(NMVariantMapMap connection)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(connection);
        return asyncCallWithArgumentList(QStringLiteral("AddConnection"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> AddConnectionUnsaved(NMVariantMapMap connection)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(connection);
        return asyncCallWithArgumentList(QStringLiteral("AddConnectionUnsaved"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> GetConnectionByUuid(const QString &uuid)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(uuid);
        return asyncCallWithArgumentList(QStringLiteral("GetConnectionByUuid"), argumentList);
    }

    inline QDBusPendingReply<QList<QDBusObjectPath>> ListConnections()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ListConnections"), argumentList);
    }

    inline QDBusPendingReply<bool, QStringList> LoadConnections(const QStringList &filenames)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(filenames);
        return asyncCallWithArgumentList(QStringLiteral("LoadConnections"), argumentList);
    }

    inline QDBusPendingReply<bool> ReloadConnections()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ReloadConnections"), argumentList);
    }

    inline QDBusPendingReply<> SaveHostname(const QString &hostname)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(hostname);
        return asyncCallWithArgumentList(QStringLiteral("SaveHostname"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void ConnectionRemoved(const QDBusObjectPath &connection);
    void NewConnection(const QDBusObjectPath &connection);
    void PropertiesChanged(const QVariantMap &properties);
};

#endif
