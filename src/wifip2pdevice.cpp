/*
    SPDX-FileCopyrightText: 2025 Martin Rodriguez Reboredo <yakoyoku@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "wifip2pdevice.h"
#include "wifip2pdevice_p.h"
#include "wifip2ppeer.h"

#undef signals
#include <libnm/NetworkManager.h>
#define signals Q_SIGNALS

#include "manager.h"
#include "manager_p.h"

#include "nmdebug.h"
#include "utils.h"

#include <QDBusMetaType>

using namespace Qt::Literals;

NetworkManager::WifiP2PDevicePrivate::WifiP2PDevicePrivate(const QString &path, WifiP2PDevice *q)
    : DevicePrivate(path, q)
#ifdef NMQT_STATIC
    , wifip2pIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    , wifip2pIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
{
}

NetworkManager::WifiP2PDevice::WifiP2PDevice(const QString &path, QObject *parent)
    : Device(*new WifiP2PDevicePrivate(path, this), parent)
{
    Q_D(WifiP2PDevice);

    qDBusRegisterMetaType<QList<QDBusObjectPath>>();

#ifdef NMQT_STATIC
    connect(&d->wifip2pIface, &OrgFreedesktopNetworkManagerDeviceWifiP2PInterface::PropertiesChanged, d, &WifiP2PDevicePrivate::propertiesChanged);
#endif

    connect(&d->wifip2pIface, &OrgFreedesktopNetworkManagerDeviceWifiP2PInterface::PeerAdded, d, &WifiP2PDevicePrivate::peerAdded);
    connect(&d->wifip2pIface, &OrgFreedesktopNetworkManagerDeviceWifiP2PInterface::PeerRemoved, d, &WifiP2PDevicePrivate::peerRemoved);

    const QList<QDBusObjectPath> peers = d->wifip2pIface.peers();
    for (const QDBusObjectPath &op : peers) {
        d->peerAdded(op);
    }

    // Retrieve all WifiP2PDevice's properties at once
    const QVariantMap initialProperties = NetworkManagerPrivate::retrieveInitialProperties(d->wifip2pIface.staticInterfaceName(), path);
    if (!initialProperties.isEmpty()) {
        d->propertiesChanged(initialProperties);
    }
}

NetworkManager::WifiP2PDevice::~WifiP2PDevice() = default;

NetworkManager::Device::Type NetworkManager::WifiP2PDevice::type() const
{
    return NetworkManager::Device::WifiP2P;
}

QString NetworkManager::WifiP2PDevice::hardwareAddress() const
{
    Q_D(const WifiP2PDevice);
    return d->hardwareAddress;
}

QStringList NetworkManager::WifiP2PDevice::peers() const
{
    Q_D(const WifiP2PDevice);
    return d->peerMap.keys();
}

NetworkManager::WifiP2PPeer::Ptr NetworkManager::WifiP2PDevice::findPeer(const QString &uni)
{
    Q_D(WifiP2PDevice);
    NetworkManager::WifiP2PPeer::Ptr peer;

    QMap<QString, NetworkManager::WifiP2PPeer::Ptr>::ConstIterator mapIt = d->peerMap.constFind(uni);
    if (mapIt != d->peerMap.constEnd()) {
        peer = mapIt.value();
    } else if (!uni.isEmpty() && uni != QLatin1String("/")) {
        d->peerAdded(QDBusObjectPath(uni));
        mapIt = d->peerMap.constFind(uni);
        if (mapIt != d->peerMap.constEnd()) {
            peer = mapIt.value();
        }
    }

    return peer;
}

QDBusPendingReply<> NetworkManager::WifiP2PDevice::startFind(const QVariantMap &options)
{
    Q_D(WifiP2PDevice);
    return d->wifip2pIface.StartFind(options);
}

QDBusPendingReply<> NetworkManager::WifiP2PDevice::stopFind()
{
    Q_D(WifiP2PDevice);
    return d->wifip2pIface.StopFind();
}

void NetworkManager::WifiP2PDevicePrivate::peerAdded(const QDBusObjectPath &peer)
{
    Q_Q(WifiP2PDevice);
    if (!peerMap.contains(peer.path())) {
        NetworkManager::WifiP2PPeer::Ptr peerPtr(new NetworkManager::WifiP2PPeer(peer.path()), &QObject::deleteLater);
        peerMap.insert(peer.path(), peerPtr);
        Q_EMIT q->peerAppeared(peer.path());
    }
}

void NetworkManager::WifiP2PDevicePrivate::peerRemoved(const QDBusObjectPath &peer)
{
    Q_Q(WifiP2PDevice);
    if (!peerMap.contains(peer.path())) {
        qCDebug(NMQT) << "Peer list lookup failed for " << peer.path();
    }
    Q_EMIT q->peerDisappeared(peer.path());
    peerMap.remove(peer.path());
}

void NetworkManager::WifiP2PDevicePrivate::propertyChanged(const QString &property, const QVariant &value)
{
    Q_Q(WifiP2PDevice);

    if (property == "HwAddress"_L1) {
        hardwareAddress = value.toString();
        Q_EMIT q->hardwareAddressChanged(hardwareAddress);
    } else if (property == "Peers"_L1) {
    } else {
        DevicePrivate::propertyChanged(property, value);
    }
}

#include "moc_wifip2pdevice.cpp"
#include "moc_wifip2pdevice_p.cpp"
