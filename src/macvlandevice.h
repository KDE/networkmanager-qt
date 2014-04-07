/*
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

#ifndef NETWORKMANAGERQT_MACVLAN_DEVICE_H
#define NETWORKMANAGERQT_MACVLAN_DEVICE_H

#include "device.h"
#include <networkmanagerqt_export.h>

namespace NetworkManager {

class MacVlanDevicePrivate;

class NETWORKMANAGERQT_EXPORT MacVlanDevice : public Device
{
    Q_OBJECT
    Q_PROPERTY(QString mode READ mode NOTIFY modeChanged)
    Q_PROPERTY(bool noPromisc READ noPromisc NOTIFY noPromiscChanged)
    Q_PROPERTY(QString parent READ parent NOTIFY parentChanged)

public:
    typedef QSharedPointer<MacVlanDevice> Ptr;
    typedef QList<Ptr> List;
    explicit MacVlanDevice(const QString &path, QObject *parent = 0);
    virtual ~MacVlanDevice();

    virtual Type type() const;

    QString mode() const;
    bool noPromisc() const;
    QString parent() const;

Q_SIGNALS:
    void modeChanged();
    void noPromiscChanged();
    void parentChanged();

protected:
    /**
     * When subclassing make sure to call the parent class method
     * if the property was not useful to your new class
     */
    virtual void propertyChanged(const QString &property, const QVariant &value);

private:
    Q_DECLARE_PRIVATE(MacVlanDevice)
};

}

#endif
