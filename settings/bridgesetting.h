/*
    Copyright 2012-2013  Jan Grulich <jgrulich@redhat.com>

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

#ifndef NMQT_SETTINGS_BRIDGE_H
#define NMQT_SETTINGS_BRIDGE_H

#include "../NetworkManagerQt-export.h"
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{

class BridgeSettingPrivate;
class NETWORKMANAGERQT_EXPORT BridgeSetting : public Setting
{
public:
    typedef QSharedPointer<BridgeSetting> Ptr;
    typedef QList<Ptr> List;
    BridgeSetting();
    explicit BridgeSetting(const Ptr &other);
    ~BridgeSetting();

    QString name() const;

    void setInterfaceName(const QString &name);
    QString interfaceName() const;

    void setStp(bool enabled);
    bool stp() const;

    void setPriority(quint32 priority);
    quint32 priority() const;

    void setForwardDelay(quint32 delay);
    quint32 forwardDelay() const;

    void setHelloTime(quint32 time);
    quint32 helloTime() const;

    void setMaxAge(quint32 age);
    quint32 maxAge() const;

    void setAgingTime(quint32 time);
    quint32 agingTime() const;

    void fromMap(const QVariantMap &setting);

    QVariantMap toMap() const;

protected:
    BridgeSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(BridgeSetting)
};

QDebug operator<<(QDebug dbg, const BridgeSetting &setting);

}

#endif // NMQT_SETTINGS_BRIDGE_H
