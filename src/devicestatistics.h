/*
    Copyright 2017 Jan Grulich <jgrulich@redhat.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NETWORKMANAGERQT_DEVICE_STATISTICS_H
#define NETWORKMANAGERQT_DEVICE_STATISTICS_H

#include <networkmanagerqt/networkmanagerqt_export.h>

#include <nm-version.h>

#include <QObject>
#include <QSharedPointer>

namespace NetworkManager
{

class DeviceStatisticsPrivate;

/**
 * Represents device statistics interface
 */
class NETWORKMANAGERQT_EXPORT DeviceStatistics : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint refreshRateMs READ refreshRateMs WRITE setRefreshRateMs NOTIFY refreshRateMsChanged)
    Q_PROPERTY(qulonglong txBytes READ txBytes NOTIFY txBytesChanged)
    Q_PROPERTY(qulonglong rxBytes READ rxBytes NOTIFY rxBytesChanged)

public:
    typedef QSharedPointer<DeviceStatistics> Ptr;
    typedef QList<Ptr> List;

    explicit DeviceStatistics(const QString &path, QObject *parent = nullptr);
    virtual ~DeviceStatistics();

    /**
     * Refresh rate of the rest of properties of this interface. The properties are guaranteed to be refreshed
     * each RefreshRateMs milliseconds in case the underlying counter has changed too. If zero, there is no guaranteed
     * refresh rate of the properties.
     */
    uint refreshRateMs() const;
    void setRefreshRateMs(uint refreshRate);
    /**
     * Number of received bytes
     */
    qulonglong rxBytes() const;
    /**
     * Number of transmitted bytes
     */
    qulonglong txBytes() const;

Q_SIGNALS:
    /**
     * Emitted when the refresh rate has changed
     */
    void refreshRateMsChanged(uint refreshRate);
    /**
     * Emitted when the received bytes has changed
     */
    void rxBytesChanged(qulonglong rxBytes);
    /**
     * Emitted when the transmitted bytes has changed
     */
    void txBytesChanged(qulonglong txBytes);

private:
    Q_DECLARE_PRIVATE(DeviceStatistics)
    DeviceStatisticsPrivate *const d_ptr;
};

}
#endif


