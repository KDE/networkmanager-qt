/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>

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

NetworkManager::WimaxDevicePrivate::WimaxDevicePrivate(const QString & path, QObject * owner)
    : DevicePrivate(path, owner), wimaxIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
{

}

NetworkManager::WimaxDevice::WimaxDevice(const QString & path, QObject * parent)
    : Device(*new WimaxDevicePrivate(path, this), parent)
{
    Q_D(WimaxDevice);
    d->hardwareAddress = d->wimaxIface.hwAddress();
    d->centerFrequency = d->wimaxIface.centerFrequency();
    d->activeNsp = d->wimaxIface.activeNsp().path();
    d->bsid = d->wimaxIface.bsid();
    d->cinr = d->wimaxIface.cinr();
    d->rssi = d->wimaxIface.rssi();
    d->txPower = d->wimaxIface.txPower();

    connect( &d->wimaxIface, SIGNAL(PropertiesChanged(QVariantMap)),
                this, SLOT(wimaxPropertiesChanged(QVariantMap)));
    connect( &d->wimaxIface, SIGNAL(NspAdded(QDBusObjectPath)),
                this, SLOT(nspAdded(QDBusObjectPath)));
    connect( &d->wimaxIface, SIGNAL(NspRemoved(QDBusObjectPath)),
                this, SLOT(nspRemoved(QDBusObjectPath)));


    qDBusRegisterMetaType<QList<QDBusObjectPath> >();
    QDBusReply< QList <QDBusObjectPath> > nspPathList = d->wimaxIface.GetNspList();
    if (nspPathList.isValid())
    {
        //nmDebug() << "Got device list";
        QList <QDBusObjectPath> nsps = nspPathList.value();
        foreach (const QDBusObjectPath &op, nsps)
        {
            d->nspMap.insert(op.path(), 0);
            //nmDebug() << "  " << op.path();
        }
    }
    else {
        nmDebug() << "Error getting NSP list: " << nspPathList.error().name() << ": " << nspPathList.error().message();
    }
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

NetworkManager::WimaxNsp * NetworkManager::WimaxDevice::activeNsp() const
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

NetworkManager::WimaxNsp * NetworkManager::WimaxDevice::findNsp(const QString & uni) const
{
    Q_D(const WimaxDevice);
    NetworkManager::WimaxNsp * nsp = 0;
    QMap<QString,NetworkManager::WimaxNsp *>::ConstIterator mapIt = d->nspMap.constFind(uni);
    if (mapIt != d->nspMap.constEnd() && mapIt.value() != 0) {
        nsp = mapIt.value();
    } else {
        nsp = new NetworkManager::WimaxNsp(uni, 0);
        d->nspMap.insert(uni, nsp);
    }

    return nsp;
}

void NetworkManager::WimaxDevice::wimaxPropertiesChanged(const QVariantMap & changedProperties)
{
    //nmDebug() << changedProperties.keys();
    QStringList propKeys = changedProperties.keys();
    Q_D(WimaxDevice);
    QLatin1String activeNspKey("ActiveNsp"),
                  hwAddrKey("HwAddress"),
                  bsidKey("Bsid"),
                  centerFrequencyKey("CenterFrequency"),
                  cinrKey("Cinr"),
                  rssiKey("Rssi"),
                  txPowerKey("TxPower");
    QVariantMap::const_iterator it = changedProperties.find(activeNspKey);
    if (it != changedProperties.end()) {
        d->activeNsp = qdbus_cast<QDBusObjectPath>(*it).path();
        emit activeNspChanged(d->activeNsp);
        propKeys.removeOne(activeNspKey);
    }
    it = changedProperties.find(hwAddrKey);
    if (it != changedProperties.end()) {
        d->hardwareAddress = it->toString();
        emit hardwareAddressChanged(d->hardwareAddress);
        propKeys.removeOne(hwAddrKey);
    }
    it = changedProperties.find(bsidKey);
    if (it != changedProperties.end()) {
        d->bsid = it->toString();
        emit bsidChanged(d->bsid);
        propKeys.removeOne(bsidKey);
    }
    it = changedProperties.find(centerFrequencyKey);
    if (it != changedProperties.end()) {
        d->centerFrequency = it->toUInt();
        emit centerFrequencyChanged(d->centerFrequency);
        propKeys.removeOne(centerFrequencyKey);
    }
    it = changedProperties.find(cinrKey);
    if (it != changedProperties.end()) {
        d->cinr = it->toInt();
        emit cinrChanged(d->cinr);
        propKeys.removeOne(cinrKey);
    }
    it = changedProperties.find(rssiKey);
    if (it != changedProperties.end()) {
        d->rssi = it->toInt();
        emit rssiChanged(d->rssi);
        propKeys.removeOne(rssiKey);
    }
    it = changedProperties.find(txPowerKey);
    if (it != changedProperties.end()) {
        d->txPower = it->toInt();
        emit txPowerChanged(d->txPower);
        propKeys.removeOne(txPowerKey);
    }
    //if (propKeys.count()) {
    //    nmDebug() << "Unhandled properties: " << propKeys;
    //}
}

void NetworkManager::WimaxDevice::nspAdded(const QDBusObjectPath &nspPath)
{
    //nmDebug() << nspPath.path();
    Q_D(WimaxDevice);
    if (!d->nspMap.contains(nspPath.path())) {
        d->nspMap.insert(nspPath.path(), 0);
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
    NetworkManager::WimaxNsp * nsp = d->nspMap.take(nspPath.path());
    delete nsp;
    emit nspDisappeared(nspPath.path());
}

#include "wimaxdevice.moc"
