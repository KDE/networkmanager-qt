/*
    Copyright 2008 Will Stephenson <wstephenson@kde.org>
    Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>
    Copyright 2013 Jan Grulich <jgrulich@redhat.com>

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

#include "accesspoint.h"
#include "dbus/nm-access-pointinterface.h"
#include "manager_p.h"
#include "wirelessdevice.h"

#include "nmdebug.h"

namespace NetworkManager {

AccessPoint::Capabilities convertCapabilities(int caps)
{
    switch (caps) {
    case NM_802_11_AP_FLAGS_NONE: return AccessPoint::None;
    case NM_802_11_AP_FLAGS_PRIVACY: return AccessPoint::Privacy;
    }
    return AccessPoint::None;
}

AccessPoint::WpaFlags convertWpaFlags(uint theirFlags)
{
    return (AccessPoint::WpaFlags)theirFlags;
}

class AccessPointPrivate
{
public:
    AccessPointPrivate(const QString &path)
        : iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
        , capabilities(0)
        , wpaFlags(0)
        , rsnFlags(0)
        , frequency(0)
        , maxBitRate(0)
        , mode(AccessPoint::Unknown)
        , signalStrength(0) {
    }
    OrgFreedesktopNetworkManagerAccessPointInterface iface;
    QString uni;
    AccessPoint::Capabilities capabilities;
    AccessPoint::WpaFlags wpaFlags;
    AccessPoint::WpaFlags rsnFlags;
    QString ssid;
    QByteArray rawSsid;
    uint frequency;
    QString hardwareAddress;
    uint maxBitRate;
    AccessPoint::OperationMode mode;
    int signalStrength;
};
}

NetworkManager::AccessPoint::AccessPoint(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new AccessPointPrivate(path))
{
    Q_D(AccessPoint);
    d->uni = path;
    if (d->iface.isValid()) {
        d->capabilities = convertCapabilities(d->iface.flags());
        d->wpaFlags = convertWpaFlags(d->iface.wpaFlags());
        d->rsnFlags = convertWpaFlags(d->iface.rsnFlags());
        d->signalStrength = d->iface.strength();
        d->rawSsid = d->iface.ssid();
        d->ssid = QString::fromUtf8(d->rawSsid);
        d->frequency = d->iface.frequency();
        d->hardwareAddress = d->iface.hwAddress();
        d->maxBitRate = d->iface.maxBitrate();
        // make this a static on WirelessNetworkInterface
        d->mode = convertOperationMode(d->iface.mode());
        connect(&d->iface, SIGNAL(PropertiesChanged(QVariantMap)),
                this, SLOT(propertiesChanged(QVariantMap)));
    }
}

NetworkManager::AccessPoint::~AccessPoint()
{
    Q_D(AccessPoint);
    delete d;
}

QString NetworkManager::AccessPoint::uni() const
{
    Q_D(const AccessPoint);
    return d->uni;
}

QString NetworkManager::AccessPoint::hardwareAddress() const
{
    Q_D(const AccessPoint);
    return d->hardwareAddress;
}

NetworkManager::AccessPoint::Capabilities NetworkManager::AccessPoint::capabilities() const
{
    Q_D(const AccessPoint);
    return d->capabilities;
}

NetworkManager::AccessPoint::WpaFlags NetworkManager::AccessPoint::wpaFlags() const
{
    Q_D(const AccessPoint);
    return d->wpaFlags;
}

NetworkManager::AccessPoint::WpaFlags NetworkManager::AccessPoint::rsnFlags() const
{
    Q_D(const AccessPoint);
    return d->rsnFlags;
}

QString NetworkManager::AccessPoint::ssid() const
{
    Q_D(const AccessPoint);
    return d->ssid;
}

QByteArray NetworkManager::AccessPoint::rawSsid() const
{
    Q_D(const AccessPoint);
    return d->rawSsid;
}

uint NetworkManager::AccessPoint::frequency() const
{
    Q_D(const AccessPoint);
    return d->frequency;
}

uint NetworkManager::AccessPoint::maxBitRate() const
{
    Q_D(const AccessPoint);
    return d->maxBitRate;
}

NetworkManager::AccessPoint::OperationMode NetworkManager::AccessPoint::mode() const
{
    Q_D(const AccessPoint);
    return d->mode;
}

int NetworkManager::AccessPoint::signalStrength() const
{
    Q_D(const AccessPoint);
    return d->signalStrength;
}

NetworkManager::AccessPoint::OperationMode NetworkManager::AccessPoint::convertOperationMode(uint mode)
{
    NetworkManager::AccessPoint::OperationMode ourMode = NetworkManager::AccessPoint::Unknown;
    switch (mode) {
    case NM_802_11_MODE_UNKNOWN:
        ourMode = NetworkManager::AccessPoint::Unknown;
        break;
    case NM_802_11_MODE_ADHOC:
        ourMode = NetworkManager::AccessPoint::Adhoc;
        break;
    case NM_802_11_MODE_INFRA:
        ourMode = NetworkManager::AccessPoint::Infra;
        break;
    case NM_802_11_MODE_AP:
        ourMode = NetworkManager::AccessPoint::ApMode;
        break;
    default:
        nmDebug() << "Unhandled mode" << mode;
    }
    return ourMode;
}

void NetworkManager::AccessPoint::propertiesChanged(const QVariantMap &properties)
{
    Q_D(AccessPoint);

    QVariantMap::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        const QString property = it.key();
        if (property == QLatin1String("Flags")) {
            d->capabilities = convertCapabilities(it->toUInt());
            emit capabilitiesChanged(d->capabilities);
        } else if (property == QLatin1String("WpaFlags")) {
            d->wpaFlags = convertWpaFlags(it->toUInt());
            emit wpaFlagsChanged(d->wpaFlags);
        } else if (property == QLatin1String("RsnFlags")) {
            d->rsnFlags = convertWpaFlags(it->toUInt());
            emit rsnFlagsChanged(d->rsnFlags);
        } else if (property == QLatin1String("Ssid")) {
            d->rawSsid = it->toByteArray();
            d->ssid = QString::fromUtf8(d->rawSsid);
            emit ssidChanged(d->ssid);
        } else if (property == QLatin1String("Frequency")) {
            d->frequency = it->toUInt();
            emit frequencyChanged(d->frequency);
        } else if (property == QLatin1String("HwAddress")) {
            d->hardwareAddress = it->toString();
        } else if (property == QLatin1String("Mode")) {
            d->mode = convertOperationMode(it->toUInt());
        } else if (property == QLatin1String("MaxBitrate")) {
            d->maxBitRate = it->toUInt();
            emit bitRateChanged(d->maxBitRate);
        } else if (property == QLatin1String("Strength")) {
            d->signalStrength = it->toInt();
            emit signalStrengthChanged(d->signalStrength);
        } else {
            qWarning() << Q_FUNC_INFO << "Unhandled property" << property;
        }
        ++it;
    }
}

#include "accesspoint.moc"
