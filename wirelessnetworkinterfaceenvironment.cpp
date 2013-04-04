/*
Copyright 2008,2009,2011 Will Stephenson <wstephenson@kde.org>

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

#include "wirelessnetworkinterfaceenvironment.h"

#include <QMutableHashIterator>

#include "manager.h"
#include "accesspoint.h"
#include "wirelessdevice.h"
#include "wirelessnetwork.h"

#include "nmdebug.h"

namespace NetworkManager {
class WirelessNetworkInterfaceEnvironmentPrivate
{
public:
    virtual ~WirelessNetworkInterfaceEnvironmentPrivate() {}
    QHash<QString, WirelessNetwork::Ptr> networks;
    WirelessDevice::Ptr iface;
};

}
NetworkManager::WirelessNetworkInterfaceEnvironment::WirelessNetworkInterfaceEnvironment(const WirelessDevice::Ptr &iface)
: QObject(iface.data()), d_ptr(new WirelessNetworkInterfaceEnvironmentPrivate)
{
    Q_D(WirelessNetworkInterfaceEnvironment);
    d->iface = iface;
    foreach (const QString &apUni, iface->accessPoints()) {
        accessPointAppearedInternal(apUni);
    }
    // for managing our list of wireless networks
    connect(iface.data(), SIGNAL(accessPointAppeared(QString)),
            SLOT(accessPointAppeared(QString)));
    connect(NetworkManager::notifier(), SIGNAL(wirelessEnabledChanged(bool)),
            SLOT(wirelessEnabledChanged(bool)));
}

NetworkManager::WirelessNetworkInterfaceEnvironment::~WirelessNetworkInterfaceEnvironment()
{
    nmDebug();
    delete d_ptr;
}

NetworkManager::WirelessDevice::Ptr NetworkManager::WirelessNetworkInterfaceEnvironment::interface() const
{
    Q_D(const WirelessNetworkInterfaceEnvironment);
    return d->iface;
}

QStringList NetworkManager::WirelessNetworkInterfaceEnvironment::networks() const
{
    Q_D(const WirelessNetworkInterfaceEnvironment);
    return d->networks.keys();
}

NetworkManager::WirelessNetwork::Ptr NetworkManager::WirelessNetworkInterfaceEnvironment::findNetwork(const QString & ssid) const
{
    Q_D(const WirelessNetworkInterfaceEnvironment);
    NetworkManager::WirelessNetwork::Ptr wifiNetwork;
    if (d->networks.contains(ssid)) {
        wifiNetwork = d->networks.value(ssid);
    }
    return wifiNetwork;
}

void NetworkManager::WirelessNetworkInterfaceEnvironment::accessPointAppeared(const QString &uni)
{
    Q_UNUSED(uni);
    //nmDebug() << d->iface->interfaceName() << " found " << uni;
    accessPointAppearedInternal(uni);
}

void NetworkManager::WirelessNetworkInterfaceEnvironment::accessPointAppearedInternal(const QString &uni)
{
    Q_D(WirelessNetworkInterfaceEnvironment);
    NetworkManager::AccessPoint * ap = d->iface->findAccessPoint(uni);
    QString ssid = ap->ssid();
    //nmDebug() << ssid << d->networks.contains(ssid);
    if (ssid.isEmpty()) {
        //nmDebug() << "ignoring hidden AP with BSSID:" << ap->hardwareAddress();
    } else if (!d->networks.contains(ssid)) {
        NetworkManager::WirelessNetwork::Ptr wifiNetwork(new NetworkManager::WirelessNetwork(ap, d->iface));
        d->networks.insert(ssid, wifiNetwork);
        connect(wifiNetwork.data(), SIGNAL(disappeared(QString)), SLOT(removeNetwork(QString)));
        emit networkAppeared(ssid);
    }

    //dump();
}
/*
void NetworkManager::WirelessNetworkInterfaceEnvironment::dump()
{
   Q_D(WirelessNetworkInterfaceEnvironment);
   nmDebug() << d->networks.count();
   QHashIterator<QString, WirelessNetwork*> i(d->networks);
   while (i.hasNext()) {
       i.next();
       i.value()->dump();
   }
}
*/
void NetworkManager::WirelessNetworkInterfaceEnvironment::removeNetwork(const QString &ssid)
{
    Q_D(WirelessNetworkInterfaceEnvironment);
    //nmDebug() << ssid;
    if (d->networks.contains(ssid)) {
        NetworkManager::WirelessNetwork::Ptr wifiNetwork = d->networks.take(ssid);
        if (wifiNetwork) {
            emit networkDisappeared(ssid);
        }
    }
    //dump();
}

void NetworkManager::WirelessNetworkInterfaceEnvironment::wirelessEnabledChanged(bool enabled)
{
    Q_D(WirelessNetworkInterfaceEnvironment);
    if (!enabled) {
        foreach (const QString &ssid, d->networks.keys()) {
            emit networkDisappeared(ssid);
        }
        d->networks.clear();
    }
}

// vim: sw=4 sts=4 et tw=100

#include "wirelessnetworkinterfaceenvironment.moc"
