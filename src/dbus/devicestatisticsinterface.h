/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was:
 *
 * qdbusxml2cpp is Copyright (C) 2016 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef DEVICESTATISTICSINTERFACE_H
#define DEVICESTATISTICSINTERFACE_H

#include "generictypes.h"

#include <QDBusAbstractInterface>
#include <QObject>
#include <QString>
#include <QVariant>

/*
 * Proxy class for interface org.freedesktop.NetworkManager.Device.Statistics
 */
class OrgFreedesktopNetworkManagerDeviceStatisticsInterface : public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
#ifdef NMQT_STATIC
    {
        return "org.kde.fakenetwork.Device.Statistics";
    }
#else
    {
        return "org.freedesktop.NetworkManager.Device.Statistics";
    }
#endif

public:
    OrgFreedesktopNetworkManagerDeviceStatisticsInterface(const QString &service,
                                                          const QString &path,
                                                          const QDBusConnection &connection,
                                                          QObject *parent = nullptr);

    ~OrgFreedesktopNetworkManagerDeviceStatisticsInterface() override;

    Q_PROPERTY(uint RefreshRateMs READ refreshRateMs WRITE setRefreshRateMs)
    inline uint refreshRateMs() const
    {
        return qvariant_cast<uint>(property("RefreshRateMs"));
    }
    inline void setRefreshRateMs(uint value)
    {
        setProperty("RefreshRateMs", QVariant::fromValue(value));
    }

    Q_PROPERTY(qulonglong RxBytes READ rxBytes)
    inline qulonglong rxBytes() const
    {
        return qvariant_cast<qulonglong>(property("RxBytes"));
    }

    Q_PROPERTY(qulonglong TxBytes READ txBytes)
    inline qulonglong txBytes() const
    {
        return qvariant_cast<qulonglong>(property("TxBytes"));
    }

public Q_SLOTS: // METHODS
Q_SIGNALS: // SIGNALS
    void PropertiesChanged(const QVariantMap &properties);
};

#endif
