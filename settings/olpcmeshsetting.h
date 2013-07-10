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

#ifndef NMQT_SETTINGS_802_11_OLPC_MESH_H
#define NMQT_SETTINGS_802_11_OLPC_MESH_H

#include "../NetworkManagerQt-export.h"
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{

class OlpcMeshSettingPrivate;
class NETWORKMANAGERQT_EXPORT OlpcMeshSetting : public Setting
{
public:
    typedef QSharedPointer<OlpcMeshSetting> Ptr;
    typedef QList<Ptr> List;
    OlpcMeshSetting();
    explicit OlpcMeshSetting(const Ptr &other);
    ~OlpcMeshSetting();

    QString name() const;

    void setSsid(const QByteArray &ssid);
    QByteArray ssid() const;

    void setChannel(quint32 channel);
    quint32 channel() const;

    void setDhcpAnycastAddress(const QByteArray &address);
    QByteArray dhcpAnycastAddress() const;

    void fromMap(const QVariantMap &setting);

    QVariantMap toMap() const;

protected:
    OlpcMeshSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(OlpcMeshSetting)
};

QDebug operator<<(QDebug dbg, const OlpcMeshSetting &setting);

}

#endif // NMQT_SETTINGS_802_11_OLPC_MESH_H
