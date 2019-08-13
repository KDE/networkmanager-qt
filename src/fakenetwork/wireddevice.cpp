/*
    Copyright 2014 Jan Grulich <jgrulich@redhat.com>

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

#include "wireddevice.h"


WiredDevice::WiredDevice(QObject *parent)
    : Device(parent)
{
}

WiredDevice::~WiredDevice()
{
    QVariantMap map;
    map.insert(QLatin1Literal("AvailableConnections"), QVariant::fromValue<QList<QDBusObjectPath> >(QList<QDBusObjectPath>()));
    Q_EMIT PropertiesChanged(map);
}

bool WiredDevice::carrier() const
{
    return m_carrier;
}

QString WiredDevice::hwAddress() const
{
    return m_hwAddress;
}

QString WiredDevice::permHwAddress() const
{
    return m_permHwAddress;
}

uint WiredDevice::speed() const
{
    return m_speed;
}

void WiredDevice::setCarrier(bool carrier)
{
    m_carrier = carrier;
}

void WiredDevice::setHwAddress(const QString &hwAddress)
{
    m_hwAddress = hwAddress;
}

void WiredDevice::setPermanentHwAddress(const QString &permanentHwAddress)
{
    m_permHwAddress = permanentHwAddress;
}

void WiredDevice::setSpeed(uint speed)
{
    m_speed = speed;
}

void WiredDevice::setState(uint state)
{
    Device::setState(state);
    // set speed
}
