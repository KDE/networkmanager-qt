/*
    Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
    Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>

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

#include "wimaxdevice.h"
#include "wimaxdevice_p.h"
#include "manager_p.h"
#include "wimaxnsp.h"

#include "nmdebug.h"

NetworkManager::WimaxDevicePrivate::WimaxDevicePrivate(const QString &path, WimaxDevice *q)
    : DevicePrivate(path, q)
    , wimaxIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
    hardwareAddress = wimaxIface.hwAddress();
    centerFrequency = wimaxIface.centerFrequency();
    activeNsp = wimaxIface.activeNsp().path();
    bsid = wimaxIface.bsid();
    cinr = wimaxIface.cinr();
    rssi = wimaxIface.rssi();
    txPower = wimaxIface.txPower();

    qDBusRegisterMetaType<QList<QDBusObjectPath> >();
#if NM_CHECK_VERSION(0, 9, 9)
    QList <QDBusObjectPath> nsps = wimaxIface.nsps();
    foreach (const QDBusObjectPath &op, nsps) {
        nspMap.insert(op.path(), NetworkManager::WimaxNsp::Ptr());
        //nmDebug() << "  " << op.path();
    }
#else
    QDBusReply< QList <QDBusObjectPath> > nspPathList = wimaxIface.GetNspList();
    if (nspPathList.isValid()) {
        //nmDebug() << "Got device list";
        QList <QDBusObjectPath> nsps = nspPathList.value();
        foreach (const QDBusObjectPath &op, nsps) {
            nspMap.insert(op.path(), NetworkManager::WimaxNsp::Ptr());
            //nmDebug() << "  " << op.path();
        }
    } else {
        nmDebug() << "Error getting NSP list: " << nspPathList.error().name() << ": " << nspPathList.error().message();
    }
#endif
}

NetworkManager::WimaxDevice::WimaxDevice(const QString &path, QObject *parent)
    : Device(*new WimaxDevicePrivate(path, this), parent)
{
    Q_D(WimaxDevice);
    connect(&d->wimaxIface, &OrgFreedesktopNetworkManagerDeviceWiMaxInterface::PropertiesChanged, this, &WimaxDevice::propertiesChanged);
    connect(&d->wimaxIface, &OrgFreedesktopNetworkManagerDeviceWiMaxInterface::NspAdded, this, &WimaxDevice::nspAdded);
    connect(&d->wimaxIface, &OrgFreedesktopNetworkManagerDeviceWiMaxInterface::NspRemoved, this, &WimaxDevice::nspRemoved);
}

NetworkManager::WimaxDevice::~WimaxDevice()
{

}

NetworkManager::Device::Type NetworkManager::WimaxDevice::type() const
{
    return NetworkManager::Device::Wimax;
}

QStringList NetworkManager::WimaxDevice::nsps() const
{
    Q_D(const WimaxDevice);
    return d->nspMap.keys();
}

NetworkManager::WimaxNsp::Ptr NetworkManager::WimaxDevice::activeNsp() const
{
    Q_D(const WimaxDevice);
    return findNsp(d->activeNsp);
}

QString NetworkManager::WimaxDevice::hardwareAddress() const
{
    Q_D(const WimaxDevice);
    return d->hardwareAddress;
}

QString NetworkManager::WimaxDevice::bsid() const
{
    Q_D(const WimaxDevice);
    return d->bsid;
}

uint NetworkManager::WimaxDevice::centerFrequency() const
{
    Q_D(const WimaxDevice);
    return d->centerFrequency;
}

int NetworkManager::WimaxDevice::cinr() const
{
    Q_D(const WimaxDevice);
    return d->cinr;
}

int NetworkManager::WimaxDevice::rssi() const
{
    Q_D(const WimaxDevice);
    return d->rssi;
}

int NetworkManager::WimaxDevice::txPower() const
{
    Q_D(const WimaxDevice);
    return d->txPower;
}

NetworkManager::WimaxNsp::Ptr NetworkManager::WimaxDevice::findNsp(const QString &uni) const
{
    Q_D(const WimaxDevice);
    NetworkManager::WimaxNsp::Ptr nsp;
    QMap<QString, NetworkManager::WimaxNsp::Ptr>::ConstIterator mapIt = d->nspMap.constFind(uni);
    if (mapIt != d->nspMap.constEnd() && !mapIt.value().isNull()) {
        nsp = mapIt.value();
    } else {
        nsp = NetworkManager::WimaxNsp::Ptr(new NetworkManager::WimaxNsp(uni), &QObject::deleteLater);
        d->nspMap.insert(uni, nsp);
    }

    return nsp;
}

void NetworkManager::WimaxDevice::nspAdded(const QDBusObjectPath &nspPath)
{
    //nmDebug() << nspPath.path();
    Q_D(WimaxDevice);
    if (!d->nspMap.contains(nspPath.path())) {
        d->nspMap.insert(nspPath.path(), NetworkManager::WimaxNsp::Ptr());
        emit nspAppeared(nspPath.path());
    }
}

void NetworkManager::WimaxDevice::nspRemoved(const QDBusObjectPath &nspPath)
{
    //nmDebug() << nspPath.path();
    Q_D(WimaxDevice);
    if (!d->nspMap.contains(nspPath.path())) {
        nmDebug() << "Access point list lookup failed for " << nspPath.path();
    }
    emit nspDisappeared(nspPath.path());
    d->nspMap.remove(nspPath.path());
}

void NetworkManager::WimaxDevice::propertyChanged(const QString &property, const QVariant &value)
{
    Q_D(WimaxDevice);

    if (property == QLatin1String("ActiveNsp")) {
        d->activeNsp = qdbus_cast<QDBusObjectPath>(value).path();
        emit activeNspChanged(d->activeNsp);
    } else if (property == QLatin1String("HwAddress")) {
        d->hardwareAddress = value.toString();
        emit hardwareAddressChanged(d->hardwareAddress);
    } else if (property == QLatin1String("Bsid")) {
        d->bsid = value.toString();
        emit bsidChanged(d->bsid);
    } else if (property == QLatin1String("CenterFrequency")) {
        d->centerFrequency = value.toUInt();
        emit centerFrequencyChanged(d->centerFrequency);
    } else if (property == QLatin1String("Cinr")) {
        d->cinr = value.toInt();
        emit cinrChanged(d->cinr);
    } else if (property == QLatin1String("Rssi")) {
        d->rssi = value.toInt();
        emit rssiChanged(d->rssi);
    } else if (property == QLatin1String("TxPower")) {
        d->txPower = value.toInt();
        emit txPowerChanged(d->txPower);
    } else {
        Device::propertyChanged(property, value);
    }
}
