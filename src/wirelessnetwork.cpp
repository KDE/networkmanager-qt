/*
    SPDX-FileCopyrightText: 2009, 2011 Will Stephenson <wstephenson@kde.org>
    SPDX-FileCopyrightText: 2013 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "wirelessnetwork.h"
#include "wirelessnetwork_p.h"

#include "manager.h"
#include "wirelessdevice.h"

NetworkManager::WirelessNetworkPrivate::WirelessNetworkPrivate(WirelessNetwork *q, WirelessDevice *device)
    : q_ptr(q)
    , wirelessNetworkInterface(device)
{
    QObject::connect(device, SIGNAL(accessPointAppeared(QString)), q, SLOT(accessPointAppeared(QString)));
    QObject::connect(device, SIGNAL(accessPointDisappeared(QString)), q, SLOT(accessPointDisappeared(QString)));
}

NetworkManager::WirelessNetworkPrivate::~WirelessNetworkPrivate()
{
}

void NetworkManager::WirelessNetworkPrivate::addAccessPointInternal(const NetworkManager::AccessPoint::Ptr &accessPoint)
{
    Q_Q(WirelessNetwork);

    QObject::connect(accessPoint.data(), SIGNAL(signalStrengthChanged(int)), q, SLOT(updateStrength()));
    aps.insert(accessPoint->uni(), accessPoint);
    updateStrength();
}

void NetworkManager::WirelessNetworkPrivate::accessPointAppeared(const QString &uni)
{
    if (!aps.contains(uni) && wirelessNetworkInterface) {
        NetworkManager::AccessPoint::Ptr accessPoint = wirelessNetworkInterface->findAccessPoint(uni);
        if (accessPoint && accessPoint->ssid() == ssid) {
            addAccessPointInternal(accessPoint);
        }
    }
}

void NetworkManager::WirelessNetworkPrivate::accessPointDisappeared(const QString &uni)
{
    Q_Q(WirelessNetwork);
    aps.remove(uni);
    if (aps.isEmpty()) {
        Q_EMIT q->disappeared(ssid);
    } else {
        updateStrength();
    }
}

void NetworkManager::WirelessNetworkPrivate::updateStrength()
{
    Q_Q(WirelessNetwork);

    int maximumStrength = -1;
    NetworkManager::AccessPoint::Ptr strongestAp;
    for (const NetworkManager::AccessPoint::Ptr &iface : std::as_const(aps)) {
        if (maximumStrength <= iface->signalStrength()) {
            maximumStrength = iface->signalStrength();
            strongestAp = iface;
        }
    }
    if (maximumStrength != strength) {
        strength = maximumStrength;
        Q_EMIT q->signalStrengthChanged(strength);
    }

    if (strongestAp && referenceAp != strongestAp) {
        referenceAp = strongestAp;
        Q_EMIT q->referenceAccessPointChanged(referenceAp->uni());
    }
    // TODO: update the networks delayed
    // kDebug() << "update strength" << ssid << strength;
}

NetworkManager::WirelessNetwork::WirelessNetwork(const AccessPoint::Ptr &accessPoint, WirelessDevice *device)
    : d_ptr(new WirelessNetworkPrivate(this, device))
{
    Q_D(WirelessNetwork);

    d->strength = -1;
    d->ssid = accessPoint->ssid();
    d->addAccessPointInternal(accessPoint);
}

NetworkManager::WirelessNetwork::~WirelessNetwork()
{
    delete d_ptr;
}

QString NetworkManager::WirelessNetwork::ssid() const
{
    Q_D(const WirelessNetwork);
    return d->ssid;
}

int NetworkManager::WirelessNetwork::signalStrength() const
{
    Q_D(const WirelessNetwork);
    return d->strength;
}

NetworkManager::AccessPoint::Ptr NetworkManager::WirelessNetwork::referenceAccessPoint() const
{
    Q_D(const WirelessNetwork);
    return d->referenceAp;
}

NetworkManager::AccessPoint::List NetworkManager::WirelessNetwork::accessPoints() const
{
    Q_D(const WirelessNetwork);
    return d->aps.values();
}

QString NetworkManager::WirelessNetwork::device() const
{
    Q_D(const WirelessNetwork);

    if (d->wirelessNetworkInterface) {
        return d->wirelessNetworkInterface->uni();
    } else {
        return QString();
    }
}

#include "moc_wirelessnetwork.cpp"
