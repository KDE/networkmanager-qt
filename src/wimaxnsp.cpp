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

#include "wimaxnsp_p.h"
#include "manager_p.h"
#include "wimaxdevice.h"
#include "nmdebug.h"

namespace NetworkManager {

NetworkManager::WimaxNsp::NetworkType convertNetworkType(uint type)
{
    switch (type) {
    case 0:
        return NetworkManager::WimaxNsp::Unknown;
    case 1:
        return NetworkManager::WimaxNsp::Home;
    case 2:
        return NetworkManager::WimaxNsp::Partner;
    case 3:
        return NetworkManager::WimaxNsp::RoamingPartner;
    }
    return NetworkManager::WimaxNsp::Unknown;
}

}

NetworkManager::WimaxNspPrivate::WimaxNspPrivate(const QString &path, WimaxNsp *q)
#ifdef NMQT_STATIC
    : iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    : iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , networkType(WimaxNsp::Unknown)
    , signalQuality(0)
    , q_ptr(q)
{
}

NetworkManager::WimaxNsp::WimaxNsp(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new WimaxNspPrivate(path, this))
{
    Q_D(WimaxNsp);
    d->uni = path;
    if (d->iface.isValid()) {
        d->networkType = convertNetworkType(d->iface.networkType());
        d->name = d->iface.name();
        d->signalQuality = d->iface.signalQuality();
        connect(&d->iface, &OrgFreedesktopNetworkManagerWiMaxNspInterface::PropertiesChanged, d, &WimaxNspPrivate::propertiesChanged);
    }
}

NetworkManager::WimaxNsp::~WimaxNsp()
{
    Q_D(WimaxNsp);
    delete d;
}

QString NetworkManager::WimaxNsp::uni() const
{
    Q_D(const WimaxNsp);
    return d->uni;
}

NetworkManager::WimaxNsp::NetworkType NetworkManager::WimaxNsp::networkType() const
{
    Q_D(const WimaxNsp);
    return d->networkType;
}

QString NetworkManager::WimaxNsp::name() const
{
    Q_D(const WimaxNsp);
    return d->name;
}

uint NetworkManager::WimaxNsp::signalQuality() const
{
    Q_D(const WimaxNsp);
    return d->signalQuality;
}

void NetworkManager::WimaxNspPrivate::propertiesChanged(const QVariantMap &properties)
{
    Q_Q(WimaxNsp);

    QVariantMap::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        const QString property = it.key();
        if (property == QLatin1String("Name")) {
            name = it->toString();
            emit q->nameChanged(name);
        } else if (property == QLatin1String("NetworkType")) {
            networkType = convertNetworkType(it->toUInt());
            emit q->networkTypeChanged(networkType);
        } else if (property == QLatin1String("SignalQuality")) {
            signalQuality = it->toUInt();
            emit q->signalQualityChanged(signalQuality);
        } else {
            qCWarning(NMQT) << Q_FUNC_INFO << "Unhandled property" << property;
        }
        ++it;
    }
}
