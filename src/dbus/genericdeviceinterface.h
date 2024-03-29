/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was:
 *
 * qdbusxml2cpp is Copyright (C) 2016 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef GENERICDEVICEINTERFACE_H
#define GENERICDEVICEINTERFACE_H

#include "generictypes.h"

#include <QDBusAbstractInterface>
#include <QObject>
#include <QString>
#include <QVariant>

/*
 * Proxy class for interface org.freedesktop.NetworkManager.Device.Generic
 */
class OrgFreedesktopNetworkManagerDeviceGenericInterface : public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
#ifdef NMQT_STATIC
    {
        return "org.kde.fakenetwork.Device.Generic";
    }
#else
    {
        return "org.freedesktop.NetworkManager.Device.Generic";
    }
#endif

public:
    OrgFreedesktopNetworkManagerDeviceGenericInterface(const QString &service,
                                                       const QString &path,
                                                       const QDBusConnection &connection,
                                                       QObject *parent = nullptr);

    ~OrgFreedesktopNetworkManagerDeviceGenericInterface() override;

    Q_PROPERTY(QString HwAddress READ hwAddress)
    inline QString hwAddress() const
    {
        return qvariant_cast<QString>(property("HwAddress"));
    }

    Q_PROPERTY(QString TypeDescription READ typeDescription)
    inline QString typeDescription() const
    {
        return qvariant_cast<QString>(property("TypeDescription"));
    }

public Q_SLOTS: // METHODS
Q_SIGNALS: // SIGNALS
    void PropertiesChanged(const QVariantMap &properties);
};

#endif
