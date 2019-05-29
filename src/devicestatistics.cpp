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

#include "devicestatistics_p.h"
#include "manager_p.h"
#include "nmdebug.h"

NetworkManager::DeviceStatisticsPrivate::DeviceStatisticsPrivate(const QString &path, DeviceStatistics *q)
#ifdef NMQT_STATIC
    : iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    : iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , refreshRateMs(0)
    , rxBytes(0)
    , txBytes(0)
    , q_ptr(q)
{
    uni = path;
}

NetworkManager::DeviceStatistics::DeviceStatistics(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new DeviceStatisticsPrivate(path, this))
{
    Q_D(DeviceStatistics);

    // Refresh rate by default is 0,
    // as soon as the refresh rate is changed, we'll get the rest of properties initialised

    // Get all DeviceStatistics's properties at once
    QDBusConnection::systemBus().connect(NetworkManagerPrivate::DBUS_SERVICE, d->uni, NetworkManagerPrivate::FDO_DBUS_PROPERTIES,
                                         QLatin1String("PropertiesChanged"), d, SLOT(dbusPropertiesChanged(QString,QVariantMap,QStringList)));
}

NetworkManager::DeviceStatistics::~DeviceStatistics()
{
    Q_D(DeviceStatistics);
    delete d;
}

uint NetworkManager::DeviceStatistics::refreshRateMs() const
{
    Q_D(const DeviceStatistics);
    return d->refreshRateMs;
}

void NetworkManager::DeviceStatistics::setRefreshRateMs(uint refreshRate)
{
    Q_D(DeviceStatistics);
    d->iface.setRefreshRateMs(refreshRate);
}

qulonglong NetworkManager::DeviceStatistics::rxBytes() const
{
    Q_D(const DeviceStatistics);
    return d->rxBytes;
}

qulonglong NetworkManager::DeviceStatistics::txBytes() const
{
    Q_D(const DeviceStatistics);
    return d->txBytes;
}

void NetworkManager::DeviceStatisticsPrivate::dbusPropertiesChanged(const QString &interfaceName, const QVariantMap &properties, const QStringList &invalidatedProperties)
{
    Q_UNUSED(invalidatedProperties);
    if (interfaceName == QLatin1String("org.freedesktop.NetworkManager.Device.Statistics")) {
        propertiesChanged(properties);
    }
}

void NetworkManager::DeviceStatisticsPrivate::propertiesChanged(const QVariantMap &properties)
{
    Q_Q(DeviceStatistics);

    // qCDebug(NMQT) << Q_FUNC_INFO << properties;

    QVariantMap::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        const QString property = it.key();
        if (property == QLatin1String("RefreshRateMs")) {
            refreshRateMs = it->toUInt();
            Q_EMIT q->refreshRateMsChanged(refreshRateMs);
        } else if (property == QLatin1String("RxBytes")) {
            rxBytes = it->toULongLong();
            Q_EMIT q->rxBytesChanged(rxBytes);
        } else if (property == QLatin1String("TxBytes")) {
            txBytes = it->toULongLong();
            Q_EMIT q->txBytesChanged(txBytes);
        } else {
            qCWarning(NMQT) << Q_FUNC_INFO << "Unhandled property" << property;
        }
        ++it;
    }
}

