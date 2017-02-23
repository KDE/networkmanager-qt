/*
    Copyright 2014 Jan Grulich <jgrulich@redhat.com>

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

#ifndef NETWORKMANAGERQT_WIMAXNSP_P_H
#define NETWORKMANAGERQT_WIMAXNSP_P_H

#include "wimaxnsp.h"
#include "dbus/wimaxnspinterface.h"

namespace NetworkManager
{
class WimaxNspPrivate : public QObject
{
    Q_OBJECT
public:
    WimaxNspPrivate(const QString &path, WimaxNsp *q);

    OrgFreedesktopNetworkManagerWiMaxNspInterface iface;
    QString uni;
    WimaxNsp::NetworkType networkType;
    QString name;
    uint signalQuality;

    Q_DECLARE_PUBLIC(WimaxNsp)
    WimaxNsp *q_ptr;
private Q_SLOTS:
    void propertiesChanged(const QVariantMap &properties);
};
}

#endif // NETWORKMANAGERQT_WIMAXNSP_P_H
