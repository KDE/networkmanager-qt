/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -N -m -i generic-types.h -p nm-managerinterface /home/grulja/projects/libnm-qt/dbus/introspection/nm-manager.xml
 *
 * qdbusxml2cpp is Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "nm-managerinterface.h"

/*
 * Implementation of interface class OrgFreedesktopNetworkManagerInterface
 */

OrgFreedesktopNetworkManagerInterface::OrgFreedesktopNetworkManagerInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

OrgFreedesktopNetworkManagerInterface::~OrgFreedesktopNetworkManagerInterface()
{
}


#include "nm-managerinterface.moc"
