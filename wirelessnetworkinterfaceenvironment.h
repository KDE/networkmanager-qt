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

#ifndef NMQT_WIRELESSNETWORKINTERFACEENVIRONMENT_H
#define NMQT_WIRELESSNETWORKINTERFACEENVIRONMENT_H

#include "QtNetworkManager-export.h"

#include <QObject>

#include "wirelessdevice.h"
#include "wirelessnetwork.h"


namespace NetworkManager {
/**
 * Models the wireless environment as seen by a particular network interface.
 * This may be merged into WirelessDevice
 */
class WirelessNetworkInterfaceEnvironmentPrivate;
class NMQT_EXPORT WirelessNetworkInterfaceEnvironment : public QObject
{
Q_OBJECT
Q_DECLARE_PRIVATE(WirelessNetworkInterfaceEnvironment)
public:
    /**
     * Construct this using a WirelessDevice::Ptr iface which is the QObject
     * parent
     */
    WirelessNetworkInterfaceEnvironment(const WirelessDevice::Ptr &iface);
    virtual ~WirelessNetworkInterfaceEnvironment();
    virtual QStringList networks() const;
    virtual WirelessNetwork * findNetwork(const QString&) const;
    void dump();
    WirelessDevice::Ptr interface() const;

private Q_SLOTS:
    void accessPointAppeared(const QString&);
    void removeNetwork(const QString&);
    void wirelessEnabledChanged(bool);

Q_SIGNALS:
    void networkAppeared(const QString &ssid);
    void networkDisappeared(const QString &ssid);
private:
    void accessPointAppearedInternal(const QString&);
    WirelessNetworkInterfaceEnvironmentPrivate * const d_ptr;
};

}
#endif // NMQT_WIRELESSNETWORKINTERFACEENVIRONMENT_H
