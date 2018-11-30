/*
    Copyright 2018 Pranav Gade <pranavgade20@gmail.com>

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

#ifndef NETWORKMANAGERQT_OVS_INTERFACE_SETTING_H
#define NETWORKMANAGERQT_OVS_INTERFACE_SETTING_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "setting.h"

#include <QString>

namespace NetworkManager
{

class OvsInterfaceSettingPrivate;

/**
 * Represents ovs-interface setting
 */
class NETWORKMANAGERQT_EXPORT OvsInterfaceSetting : public Setting
{
public:
    typedef QSharedPointer<OvsInterfaceSetting> Ptr;
    typedef QList<Ptr> List;

    OvsInterfaceSetting();
    explicit OvsInterfaceSetting(const Ptr &other);
    ~OvsInterfaceSetting() override;

    QString name() const override;

    void setInterfaceType(const QString &type);
    QString interfaceType() const;

    void fromMap(const QVariantMap &setting) override;

    QVariantMap toMap() const override;

protected:
    OvsInterfaceSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(OvsInterfaceSetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const OvsInterfaceSetting &setting);

}

#endif // NETWORKMANAGERQT_OVS_INTERFACE_SETTING_H
