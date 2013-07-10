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

#ifndef NMQT_BOND_DEVICE_H
#define NMQT_BOND_DEVICE_H

#include "device.h"
#include "NetworkManagerQt-export.h"

namespace NetworkManager {

class BondDevicePrivate;

class NETWORKMANAGERQT_EXPORT BondDevice : public Device
{
    Q_OBJECT
    Q_PROPERTY(bool carrier READ carrier NOTIFY carrierChanged)
    Q_PROPERTY(QString hwAddress READ hwAddress NOTIFY hwAddressChanged)
    Q_PROPERTY(QList<QDBusObjectPath> slaves READ slaves NOTIFY slavesChanged)

public:
    typedef QSharedPointer<BondDevice> Ptr;
    typedef QList<Ptr> List;
    explicit BondDevice(const QString &path, QObject *parent = 0);
    virtual ~BondDevice();

    virtual Type type() const;
    bool carrier() const;
    QString hwAddress() const;
    QList<QDBusObjectPath> slaves() const;

Q_SIGNALS:
    void carrierChanged(bool plugged);
    void hwAddressChanged(const QString &address);
    void slavesChanged(const QList<QDBusObjectPath> &slaves);

protected:
    /**
     * When subclassing make sure to call the parent class method
     * if the property was not useful to your new class
     */
    virtual void propertyChanged(const QString &property, const QVariant &value);

private:
    Q_DECLARE_PRIVATE(BondDevice)
};

}

#endif
