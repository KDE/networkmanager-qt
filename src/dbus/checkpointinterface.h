/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was:
 *
 * qdbusxml2cpp is Copyright (C) 2016 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef CHECKPOINTINTERFACE_H
#define CHECKPOINTINTERFACE_H

#include "generictypes.h"

#include <QDBusAbstractInterface>
#include <QDBusObjectPath>
#include <QList>
#include <QObject>
#include <QString>
#include <QVariant>

/*
 * Proxy class for interface org.freedesktop.NetworkManager.Checkpoint
 */
class OrgFreedesktopNetworkManagerCheckpointInterface : public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
#ifdef NMQT_STATIC
    {
        return "org.kde.fakenetwork.Checkpoint";
    }
#else
    {
        return "org.freedesktop.NetworkManager.Checkpoint";
    }
#endif

public:
    OrgFreedesktopNetworkManagerCheckpointInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~OrgFreedesktopNetworkManagerCheckpointInterface() override;

    Q_PROPERTY(qlonglong Created READ created)
    inline qlonglong created() const
    {
        return qvariant_cast<qlonglong>(property("Created"));
    }

    Q_PROPERTY(QList<QDBusObjectPath> Devices READ devices)
    inline QList<QDBusObjectPath> devices() const
    {
        return qvariant_cast<QList<QDBusObjectPath>>(property("Devices"));
    }

    Q_PROPERTY(uint RollbackTimeout READ rollbackTimeout)
    inline uint rollbackTimeout() const
    {
        return qvariant_cast<uint>(property("RollbackTimeout"));
    }

public Q_SLOTS: // METHODS
Q_SIGNALS: // SIGNALS
    void PropertiesChanged(const QVariantMap &properties);
};

#endif
