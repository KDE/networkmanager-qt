/*
Copyright 2009,2011 Will Stephenson <wstephenson@kde.org>

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

#include "wirelessnetwork.h"
#include "wirelessnetwork_p.h"

NetworkManager::WirelessNetwork::WirelessNetwork(NetworkManager::AccessPoint *ap, WirelessDevice *wni, QObject * parent)
    : QObject(parent), d_ptr(new WirelessNetworkPrivate)
{
    Q_D(WirelessNetwork);
    d->ssid = ap->ssid();
    d->strength = -1;
    d->wirelessNetworkInterface = wni;
    connect(d->wirelessNetworkInterface, SIGNAL(accessPointAppeared(const QString&)), this, SLOT(accessPointAppeared(const QString&)));
    connect(d->wirelessNetworkInterface, SIGNAL(accessPointDisappeared(const QString&)), this, SLOT(accessPointDisappeared(const QString&)));
    addAccessPointInternal(ap);
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

void NetworkManager::WirelessNetwork::accessPointAppeared(const QString &uni)
{
    Q_D(const WirelessNetwork);
    if (!d->aps.contains(uni)) {
        NetworkManager::AccessPoint * ap = d->wirelessNetworkInterface->findAccessPoint(uni);
        if (ap->ssid() == d->ssid) {
            addAccessPointInternal(ap);
        }
    }
}

void NetworkManager::WirelessNetwork::addAccessPointInternal(NetworkManager::AccessPoint * ap)
{
    Q_D(WirelessNetwork);
    connect(ap, SIGNAL(signalStrengthChanged(int)),
            SLOT(updateStrength()));
    d->aps.insert(ap->uni(), ap);
    updateStrength();
}

void NetworkManager::WirelessNetwork::accessPointDisappeared(const QString &uni)
{
    Q_D(WirelessNetwork);
    d->aps.remove(uni);
    if (d->aps.isEmpty()) {
        emit disappeared(d->ssid);
    } else {
        updateStrength();
    }
}

void NetworkManager::WirelessNetwork::updateStrength()
{
    Q_D(WirelessNetwork);
    int maximumStrength = -1;
    foreach (NetworkManager::AccessPoint* iface, d->aps) {
        maximumStrength = qMax(maximumStrength, iface->signalStrength());
    }
    if (maximumStrength != d->strength) {
        d->strength = maximumStrength;
        emit signalStrengthChanged(d->strength);
    }
    //TODO: update the networks delayed
    //kDebug() << "update strength" << d->ssid << d->strength;
}

QString NetworkManager::WirelessNetwork::referenceAccessPoint() const
{
    Q_D(const WirelessNetwork);
    int maximumStrength = -1;
    NetworkManager::AccessPoint* strongest = 0;
    foreach (NetworkManager::AccessPoint* iface, d->aps) {
        int oldMax = maximumStrength;
        maximumStrength = qMax(maximumStrength, iface->signalStrength());
        if ( oldMax <= maximumStrength ) {
            strongest = iface;
        }
    }
    return strongest->uni();
}

NetworkManager::AccessPointList NetworkManager::WirelessNetwork::accessPoints() const
{
    Q_D(const WirelessNetwork);
    NetworkManager::AccessPointList aps;
    foreach (NetworkManager::AccessPoint* iface, d->aps) {
        aps.append(iface->uni());
    }
    return aps;
}

// vim: sw=4 sts=4 et tw=100
