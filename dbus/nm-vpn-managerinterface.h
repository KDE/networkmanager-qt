/*
 * This file was generated by dbusxml2cpp version 0.6
 * Command line was: dbusxml2cpp -N -m -p nm-vpn-managerinterface /space/kde/sources/trunk/KDE/kdebase/workspace/solid/networkmanager-0.7/dbus/introspection/nm-vpn-manager.xml
 *
 * dbusxml2cpp is Copyright (C) 2006 Trolltech ASA. All rights reserved.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef NM_VPN_MANAGERINTERFACE_H
#define NM_VPN_MANAGERINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.freedesktop.NetworkManager.VPN.Manager
 */
class OrgFreedesktopNetworkManagerVPNManagerInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.freedesktop.NetworkManager.VPN.Manager"; }

public:
    OrgFreedesktopNetworkManagerVPNManagerInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OrgFreedesktopNetworkManagerVPNManagerInterface();

public Q_SLOTS: // METHODS
    inline QDBusReply<QDBusObjectPath> Connect(const QString &connection_type, const QDBusObjectPath &connection, const QDBusObjectPath &device)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(connection_type) << qVariantFromValue(connection) << qVariantFromValue(device);
        return callWithArgumentList(QDBus::Block, QLatin1String("Connect"), argumentList);
    }

    inline QDBusReply<QList<QDBusObjectPath> > ListConnections()
    {
        QList<QVariant> argumentList;
        return callWithArgumentList(QDBus::Block, QLatin1String("ListConnections"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

#endif
