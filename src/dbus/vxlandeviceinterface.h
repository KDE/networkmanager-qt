/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was:
 *
 * qdbusxml2cpp is Copyright (C) 2016 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef VXLANDEVICEINTERFACE_H
#define VXLANDEVICEINTERFACE_H

#include "generictypes.h"

#include <QDBusAbstractInterface>
#include <QDBusObjectPath>
#include <QObject>
#include <QString>
#include <QVariant>

/*
 * Proxy class for interface org.freedesktop.NetworkManager.Device.Vxlan
 */
class OrgFreedesktopNetworkManagerDeviceVxlanInterface : public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
#ifdef NMQT_STATIC
    {
        return "org.kde.fakenetwork.Device.Vxlan";
    }
#else
    {
        return "org.freedesktop.NetworkManager.Device.Vxlan";
    }
#endif

public:
    OrgFreedesktopNetworkManagerDeviceVxlanInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~OrgFreedesktopNetworkManagerDeviceVxlanInterface() override;

    Q_PROPERTY(uint Ageing READ ageing)
    inline uint ageing() const
    {
        return qvariant_cast<uint>(property("Ageing"));
    }

    Q_PROPERTY(ushort DstPort READ dstPort)
    inline ushort dstPort() const
    {
        return qvariant_cast<ushort>(property("DstPort"));
    }

    Q_PROPERTY(QString Group READ group)
    inline QString group() const
    {
        return qvariant_cast<QString>(property("Group"));
    }

    Q_PROPERTY(QString HwAddress READ hwAddress)
    inline QString hwAddress() const
    {
        return qvariant_cast<QString>(property("HwAddress"));
    }

    Q_PROPERTY(uint Id READ id)
    inline uint id() const
    {
        return qvariant_cast<uint>(property("Id"));
    }

    Q_PROPERTY(bool L2miss READ l2miss)
    inline bool l2miss() const
    {
        return qvariant_cast<bool>(property("L2miss"));
    }

    Q_PROPERTY(bool L3miss READ l3miss)
    inline bool l3miss() const
    {
        return qvariant_cast<bool>(property("L3miss"));
    }

    Q_PROPERTY(bool Learning READ learning)
    inline bool learning() const
    {
        return qvariant_cast<bool>(property("Learning"));
    }

    Q_PROPERTY(uint Limit READ limit)
    inline uint limit() const
    {
        return qvariant_cast<uint>(property("Limit"));
    }

    Q_PROPERTY(QString Local READ local)
    inline QString local() const
    {
        return qvariant_cast<QString>(property("Local"));
    }

    Q_PROPERTY(QDBusObjectPath Parent READ parent)
    inline QDBusObjectPath parent() const
    {
        return qvariant_cast<QDBusObjectPath>(property("Parent"));
    }

    Q_PROPERTY(bool Proxy READ proxy)
    inline bool proxy() const
    {
        return qvariant_cast<bool>(property("Proxy"));
    }

    Q_PROPERTY(bool Rsc READ rsc)
    inline bool rsc() const
    {
        return qvariant_cast<bool>(property("Rsc"));
    }

    Q_PROPERTY(ushort SrcPortMax READ srcPortMax)
    inline ushort srcPortMax() const
    {
        return qvariant_cast<ushort>(property("SrcPortMax"));
    }

    Q_PROPERTY(ushort SrcPortMin READ srcPortMin)
    inline ushort srcPortMin() const
    {
        return qvariant_cast<ushort>(property("SrcPortMin"));
    }

    Q_PROPERTY(uchar Tos READ tos)
    inline uchar tos() const
    {
        return qvariant_cast<uchar>(property("Tos"));
    }

    Q_PROPERTY(uchar Ttl READ ttl)
    inline uchar ttl() const
    {
        return qvariant_cast<uchar>(property("Ttl"));
    }

public Q_SLOTS: // METHODS
Q_SIGNALS: // SIGNALS
    void PropertiesChanged(const QVariantMap &properties);
};

#endif
