/*
    Copyright 2009,2011 Will Stephenson <wstephenson@kde.org>
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

#include "wirelessnetwork.h"
#include "wirelessnetwork_p.h"

#include "wirelessdevice.h"
#include "manager.h"

NetworkManager::WirelessNetworkPrivate::WirelessNetworkPrivate(WirelessNetwork *q, WirelessDevice *device)
    : q_ptr(q)
    , wirelessNetworkInterface(device)
{
    QObject::connect(device, SIGNAL(accessPointAppeared(QString)),
            q, SLOT(accessPointAppeared(QString)));
    QObject::connect(device, SIGNAL(accessPointDisappeared(QString)),
            q, SLOT(accessPointDisappeared(QString)));
}

NetworkManager::WirelessNetworkPrivate::~WirelessNetworkPrivate()
{
}

void NetworkManager::WirelessNetworkPrivate::addAccessPointInternal(const NetworkManager::AccessPoint::Ptr &accessPoint)
{
    Q_Q(WirelessNetwork);

    QObject::connect(accessPoint.data(), SIGNAL(signalStrengthChanged(int)),
            q, SLOT(updateStrength()));
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
        emit q->disappeared(ssid);
    } else {
        updateStrength();
    }
}

void NetworkManager::WirelessNetworkPrivate::updateStrength()
{
    Q_Q(WirelessNetwork);
    int maximumStrength = -1;
    foreach (const NetworkManager::AccessPoint::Ptr &iface, aps) {
        maximumStrength = qMax(maximumStrength, iface->signalStrength());
    }
    if (maximumStrength != strength) {
        strength = maximumStrength;
        emit q->signalStrengthChanged(strength);
    }
    //TODO: update the networks delayed
    //kDebug() << "update strength" << ssid << strength;
}


NetworkManager::WirelessNetwork::WirelessNetwork(const AccessPoint::Ptr &accessPoint, WirelessDevice *device) :
    d_ptr(new WirelessNetworkPrivate(this, device))
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
    int maximumStrength = -1;
    NetworkManager::AccessPoint::Ptr strongest;
    foreach (const NetworkManager::AccessPoint::Ptr &iface, d->aps) {
        int oldMax = maximumStrength;
        maximumStrength = qMax(maximumStrength, iface->signalStrength());
        if (oldMax <= maximumStrength) {
            strongest = iface;
        }
    }
    return strongest;
}

NetworkManager::AccessPoint::List NetworkManager::WirelessNetwork::accessPoints() const
{
    Q_D(const WirelessNetwork);
    return d->aps.values();
}

// vim: sw=4 sts=4 et tw=100

#include "wirelessnetwork.moc"
