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

#ifndef NMQT_SETTINGS_VLAN_H
#define NMQT_SETTINGS_VLAN_H

#include "../NetworkManagerQt-export.h"
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{

class VlanSettingPrivate;
class NETWORKMANAGERQT_EXPORT VlanSetting : public Setting
{
public:
    typedef QSharedPointer<VlanSetting> Ptr;
    typedef QList<Ptr> List;
    enum Flag {None = 0, ReorderHeaders = 0x1, Gvrp = 0x2, LooseBinding = 0x4};
    Q_DECLARE_FLAGS(Flags, Flag)

    VlanSetting();
    explicit VlanSetting(const Ptr &other);
    ~VlanSetting();

    QString name() const;

    void setInterfaceName(const QString &name);
    QString interfaceName() const;

    void setParent(const QString &parent);
    QString parent() const;

    void setId(quint32 id);
    quint32 id() const;

    void setFlags(Flags flags);
    Flags flags() const;

    void setIngressPriorityMap(const QStringList &map);
    QStringList ingressPriorityMap() const;

    void setEgressPriorityMap(const QStringList &map);
    QStringList egressPriorityMap() const;

    void fromMap(const QVariantMap &setting);

    QVariantMap toMap() const;

protected:
    VlanSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(VlanSetting)
};
Q_DECLARE_OPERATORS_FOR_FLAGS(VlanSetting::Flags)

QDebug operator<<(QDebug dbg, const VlanSetting &setting);

}

#endif // NMQT_SETTINGS_VLAN_H
