/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>

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

NetworkManager::WirelessDevicePrivate::WirelessDevicePrivate(const QString & path, QObject * owner)
    : DevicePrivate(path, owner), wirelessIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
      , bitRate(0)
{

}

NetworkManager::WirelessDevice::WirelessDevice(const QString & path, QObject * parent)
    : Device(*new WirelessDevicePrivate(path, this), parent)
{
    Q_D(WirelessDevice);
    d->hardwareAddress = d->wirelessIface.hwAddress();
    d->permanentHardwareAddress = d->wirelessIface.permHwAddress();
    d->mode = convertOperationMode(d->wirelessIface.mode());
    d->bitRate = d->wirelessIface.bitrate();
    d->activeAccessPoint = d->wirelessIface.activeAccessPoint().path();
    d->wirelessCapabilities = convertCapabilities(d->wirelessIface.wirelessCapabilities());

    connect( &d->wirelessIface, SIGNAL(PropertiesChanged(QVariantMap)),
                this, SLOT(wirelessPropertiesChanged(QVariantMap)));
    connect( &d->wirelessIface, SIGNAL(AccessPointAdded(QDBusObjectPath)),
                this, SLOT(accessPointAdded(QDBusObjectPath)));
    connect( &d->wirelessIface, SIGNAL(AccessPointRemoved(QDBusObjectPath)),
                this, SLOT(accessPointRemoved(QDBusObjectPath)));


    qDBusRegisterMetaType<QList<QDBusObjectPath> >();
    QDBusReply< QList <QDBusObjectPath> > apPathList = d->wirelessIface.GetAccessPoints();
    if (apPathList.isValid())
    {
        //nmDebug() << "Got device list";
        QList <QDBusObjectPath> aps = apPathList.value();
        foreach (const QDBusObjectPath &op, aps)
        {
            d->apMap.insert(op.path(), NetworkManager::AccessPoint::Ptr());
            //nmDebug() << "  " << op.path();
        }
    }
    else {
        nmDebug() << "Error getting access point list: " << apPathList.error().name() << ": " << apPathList.error().message();
    }
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

QDBusPendingReply<> NetworkManager::WirelessDevice::requestScan(QVariantMap & options)
{
    Q_D(WirelessDevice);
    return d->wirelessIface.RequestScan(options);
}

QString NetworkManager::WirelessDevice::activeAccessPoint() const
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

NetworkManager::AccessPoint::Ptr NetworkManager::WirelessDevice::findAccessPoint(const QString & uni) const
{
    Q_D(const WirelessDevice);
    NetworkManager::AccessPoint::Ptr accessPoint;
    QMap<QString,NetworkManager::AccessPoint::Ptr>::ConstIterator mapIt = d->apMap.constFind(uni);
    if (mapIt != d->apMap.constEnd() && mapIt.value() != 0) {
        accessPoint = mapIt.value();
    } else if (mapIt != d->apMap.constEnd()) {
        accessPoint = NetworkManager::AccessPoint::Ptr(new NetworkManager::AccessPoint(uni, 0));
        d->apMap.insert(uni, accessPoint);
    }

    return accessPoint;
}

void NetworkManager::WirelessDevice::wirelessPropertiesChanged(const QVariantMap & changedProperties)
{
    Q_D(WirelessDevice);

    QVariantMap::const_iterator it = changedProperties.constBegin();
    while (it != changedProperties.constEnd()) {
        QString property = it.key();
        if (property == QLatin1String("ActiveAccessPoint")) {
            d->activeAccessPoint = qdbus_cast<QDBusObjectPath>(*it).path();
            emit activeAccessPointChanged(d->activeAccessPoint);
        } else if (property == QLatin1String("HwAddress")) {
            d->hardwareAddress = it->toString();
            emit hardwareAddressChanged(d->hardwareAddress);
        } else if (property == QLatin1String("PermHwAddress")) {
            d->permanentHardwareAddress = it->toString();
            emit permanentHardwareAddressChanged(d->permanentHardwareAddress);
        } else if (property == QLatin1String("Bitrate")) {
            d->bitRate = it->toUInt();
            emit bitRateChanged(d->bitRate);
        } else if (property == QLatin1String("Mode")) {
            d->mode = convertOperationMode(it->toUInt());
            emit modeChanged(d->mode);
        } else if (property == QLatin1String("WirelessCapabilities")) {
            d->wirelessCapabilities = convertCapabilities(it->toUInt());
            emit wirelessCapabilitiesChanged(d->wirelessCapabilities);
        } else {
            qWarning() << "Unhandled property" << property;
        }
        ++it;
    }
}

void NetworkManager::WirelessDevice::accessPointAdded(const QDBusObjectPath &apPath)
{
    //kDebug(1441) << apPath.path();
    Q_D(WirelessDevice);
    if (!d->apMap.contains(apPath.path())) {
        d->apMap.insert(apPath.path(), NetworkManager::AccessPoint::Ptr());
        emit accessPointAppeared(apPath.path());
    }
}

void NetworkManager::WirelessDevice::accessPointRemoved(const QDBusObjectPath &apPath)
{
    //kDebug(1441) << apPath.path();
    Q_D(WirelessDevice);
    if (!d->apMap.contains(apPath.path())) {
        nmDebug() << "Access point list lookup failed for " << apPath.path();
    }
    d->apMap.remove(apPath.path());
    emit accessPointDisappeared(apPath.path());
}

NetworkManager::WirelessDevice::OperationMode NetworkManager::WirelessDevice::convertOperationMode(uint theirMode)
{
    NetworkManager::WirelessDevice::OperationMode ourMode = NetworkManager::WirelessDevice::Unknown;
    switch ( theirMode ) {
        case NM_802_11_MODE_UNKNOWN:
            ourMode = NetworkManager::WirelessDevice::Unknown;
            break;
        case NM_802_11_MODE_ADHOC:
            ourMode = NetworkManager::WirelessDevice::Adhoc;
            break;
        case NM_802_11_MODE_INFRA:
            ourMode = NetworkManager::WirelessDevice::Infra;
            break;
#if NM_CHECK_VERSION(0, 9, 7)
        case NM_802_11_MODE_AP:
            ourMode = NetworkManager::WirelessDevice::ApMode;
            break;
#endif
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
