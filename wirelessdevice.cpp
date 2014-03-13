/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
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

#include "wirelessdevice.h"
#include "wirelessdevice_p.h"

#include <libnm-glib/nm-device-wifi.h>

#include "accesspoint.h"
#include "manager_p.h"

#include "nmdebug.h"

NetworkManager::WirelessDevicePrivate::WirelessDevicePrivate(const QString &path, WirelessDevice *q)
    : DevicePrivate(path, q)
    , wirelessIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
    , bitRate(0)
{
}

NetworkManager::WirelessDevice::WirelessDevice(const QString &path, QObject *parent)
    : Device(*new WirelessDevicePrivate(path, this), parent)
{
    Q_D(WirelessDevice);
    d->hardwareAddress = d->wirelessIface.hwAddress();
    d->permanentHardwareAddress = d->wirelessIface.permHwAddress();
    d->mode = convertOperationMode(d->wirelessIface.mode());
    d->bitRate = d->wirelessIface.bitrate();
    d->wirelessCapabilities = convertCapabilities(d->wirelessIface.wirelessCapabilities());

    connect(&d->wirelessIface, SIGNAL(PropertiesChanged(QVariantMap)),
            this, SLOT(propertiesChanged(QVariantMap)));
    connect(&d->wirelessIface, SIGNAL(AccessPointAdded(QDBusObjectPath)),
            this, SLOT(accessPointAdded(QDBusObjectPath)));
    connect(&d->wirelessIface, SIGNAL(AccessPointRemoved(QDBusObjectPath)),
            this, SLOT(accessPointRemoved(QDBusObjectPath)));


    qDBusRegisterMetaType<QList<QDBusObjectPath> >();
#if NM_CHECK_VERSION(0, 9, 9)
    QList <QDBusObjectPath> aps = d->wirelessIface.accessPoints();
    foreach (const QDBusObjectPath &op, aps) {
        accessPointAdded(op);
    }
#else
    QDBusReply< QList <QDBusObjectPath> > apPathList = d->wirelessIface.GetAccessPoints();
    if (apPathList.isValid()) {
        //nmDebug() << "Got device list";
        QList <QDBusObjectPath> aps = apPathList.value();
        foreach (const QDBusObjectPath &op, aps) {
            accessPointAdded(op);
        }
    } else {
        nmDebug() << "Error getting access point list: " << apPathList.error().name() << ": " << apPathList.error().message();
    }
#endif
    d->activeAccessPoint = findAccessPoint(d->wirelessIface.activeAccessPoint().path());
}

NetworkManager::WirelessDevice::~WirelessDevice()
{
}

NetworkManager::Device::Type NetworkManager::WirelessDevice::type() const
{
    return NetworkManager::Device::Wifi;
}

QStringList NetworkManager::WirelessDevice::accessPoints() const
{
    Q_D(const WirelessDevice);
    return d->apMap.keys();
}

QDBusPendingReply<> NetworkManager::WirelessDevice::requestScan(const QVariantMap &options)
{
    Q_D(WirelessDevice);
    return d->wirelessIface.RequestScan(options);
}

NetworkManager::AccessPoint::Ptr NetworkManager::WirelessDevice::activeAccessPoint() const
{
    Q_D(const WirelessDevice);
    return d->activeAccessPoint;
}

QString NetworkManager::WirelessDevice::hardwareAddress() const
{
    Q_D(const WirelessDevice);
    return d->hardwareAddress;
}

QString NetworkManager::WirelessDevice::permanentHardwareAddress() const
{
    Q_D(const WirelessDevice);
    return d->permanentHardwareAddress;
}

NetworkManager::WirelessDevice::OperationMode NetworkManager::WirelessDevice::mode() const
{
    Q_D(const WirelessDevice);
    return d->mode;
}

int NetworkManager::WirelessDevice::bitRate() const
{
    Q_D(const WirelessDevice);
    return d->bitRate;
}

NetworkManager::WirelessDevice::Capabilities NetworkManager::WirelessDevice::wirelessCapabilities() const
{
    Q_D(const WirelessDevice);
    return d->wirelessCapabilities;
}

NetworkManager::AccessPoint::Ptr NetworkManager::WirelessDevice::findAccessPoint(const QString &uni)
{
    Q_D(const WirelessDevice);
    NetworkManager::AccessPoint::Ptr accessPoint;

    QMap<QString, NetworkManager::AccessPoint::Ptr>::ConstIterator mapIt = d->apMap.constFind(uni);
    if (mapIt != d->apMap.constEnd()) {
        accessPoint = mapIt.value();
    } else if (!uni.isEmpty() && uni != QLatin1String("/")) {
        accessPointAdded(QDBusObjectPath(uni));
        mapIt = d->apMap.constFind(uni);
        if (mapIt != d->apMap.constEnd()) {
            accessPoint = mapIt.value();
        }
    }

    return accessPoint;
}

NetworkManager::WirelessNetwork::List NetworkManager::WirelessDevice::networks() const
{
    Q_D(const WirelessDevice);
    return d->networks.values();
}

NetworkManager::WirelessNetwork::Ptr NetworkManager::WirelessDevice::findNetwork(const QString &ssid) const
{
    Q_D(const WirelessDevice);
    NetworkManager::WirelessNetwork::Ptr ret;
    if (d->networks.contains(ssid)) {
        ret = d->networks.value(ssid);
    }
    return ret;
}

void NetworkManager::WirelessDevice::accessPointAdded(const QDBusObjectPath &accessPoint)
{
    //kDebug(1441) << apPath.path();
    Q_D(WirelessDevice);

    if (!d->apMap.contains(accessPoint.path())) {
        NetworkManager::AccessPoint::Ptr accessPointPtr(new NetworkManager::AccessPoint(accessPoint.path()), &QObject::deleteLater);
        d->apMap.insert(accessPoint.path(), accessPointPtr);
        emit accessPointAppeared(accessPoint.path());

        const QString ssid = accessPointPtr->ssid();
        if (!ssid.isEmpty() && !d->networks.contains(ssid)) {
            NetworkManager::WirelessNetwork::Ptr wifiNetwork(new NetworkManager::WirelessNetwork(accessPointPtr, this), &QObject::deleteLater);
            d->networks.insert(ssid, wifiNetwork);
            connect(wifiNetwork.data(), SIGNAL(disappeared(QString)), SLOT(removeNetwork(QString)));
            emit networkAppeared(ssid);
        }
    }
}

void NetworkManager::WirelessDevice::accessPointRemoved(const QDBusObjectPath &accessPoint)
{
    //kDebug(1441) << apPath.path();
    Q_D(WirelessDevice);
    if (!d->apMap.contains(accessPoint.path())) {
        nmDebug() << "Access point list lookup failed for " << accessPoint.path();
    }
    emit accessPointDisappeared(accessPoint.path());
    d->apMap.remove(accessPoint.path());

}

void NetworkManager::WirelessDevice::removeNetwork(const QString &network)
{
    Q_D(WirelessDevice);

    if (d->networks.contains(network)) {
        d->networks.remove(network);
        emit networkDisappeared(network);
    }
}

void NetworkManager::WirelessDevice::propertyChanged(const QString &property, const QVariant &value)
{
    Q_D(WirelessDevice);

    if (property == QLatin1String("ActiveAccessPoint")) {
        QDBusObjectPath activeAccessPoint = qdbus_cast<QDBusObjectPath>(value);
        d->activeAccessPoint = findAccessPoint(activeAccessPoint.path());
        emit activeAccessPointChanged(activeAccessPoint.path());
    } else if (property == QLatin1String("HwAddress")) {
        d->hardwareAddress = value.toString();
        emit hardwareAddressChanged(d->hardwareAddress);
    } else if (property == QLatin1String("PermHwAddress")) {
        d->permanentHardwareAddress = value.toString();
        emit permanentHardwareAddressChanged(d->permanentHardwareAddress);
    } else if (property == QLatin1String("Bitrate")) {
        d->bitRate = value.toUInt();
        emit bitRateChanged(d->bitRate);
    } else if (property == QLatin1String("Mode")) {
        d->mode = convertOperationMode(value.toUInt());
        emit modeChanged(d->mode);
    } else if (property == QLatin1String("WirelessCapabilities")) {
        d->wirelessCapabilities = convertCapabilities(value.toUInt());
        emit wirelessCapabilitiesChanged(d->wirelessCapabilities);
    } else {
        Device::propertyChanged(property, value);
    }
}

NetworkManager::WirelessDevice::OperationMode NetworkManager::WirelessDevice::convertOperationMode(uint theirMode)
{
    NetworkManager::WirelessDevice::OperationMode ourMode = NetworkManager::WirelessDevice::Unknown;
    switch (theirMode) {
    case NM_802_11_MODE_UNKNOWN:
        ourMode = NetworkManager::WirelessDevice::Unknown;
        break;
    case NM_802_11_MODE_ADHOC:
        ourMode = NetworkManager::WirelessDevice::Adhoc;
        break;
    case NM_802_11_MODE_INFRA:
        ourMode = NetworkManager::WirelessDevice::Infra;
        break;
    case NM_802_11_MODE_AP:
        ourMode = NetworkManager::WirelessDevice::ApMode;
        break;
    default:
        nmDebug() << "Unhandled mode" << theirMode;
    }
    return ourMode;
}

NetworkManager::WirelessDevice::Capabilities NetworkManager::WirelessDevice::convertCapabilities(uint caps)
{
    return (NetworkManager::WirelessDevice::Capabilities)caps;
}

#include "wirelessdevice.moc"
