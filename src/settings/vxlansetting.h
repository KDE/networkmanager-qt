/*
    Copyright 2018 Billy Laws <blaws05@gmail.com>

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

#ifndef NETWORKMANAGERQT_VXLAN_SETTING_H
#define NETWORKMANAGERQT_VXLAN_SETTING_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "setting.h"

#include <QString>

namespace NetworkManager
{

class VxlanSettingPrivate;

/**
 * Represents vxlan setting
 */
class NETWORKMANAGERQT_EXPORT VxlanSetting : public Setting
{
public:
    typedef QSharedPointer<VxlanSetting> Ptr;
    typedef QList<Ptr> List;
    VxlanSetting();
    explicit VxlanSetting(const Ptr &other);
    ~VxlanSetting() override;

    QString name() const override;

    void setAgeing(quint32 ageing);
    quint32 ageing() const;

    void setDestinationPort(quint32 port);
    quint32 destinationPort() const;

    void setId(quint32 id);
    quint32 id() const;

    void setL2Miss(bool enable);
    bool l2Miss() const;

    void setL3Miss(bool enable);
    bool l3Miss() const;

    void setLearning(bool enable);
    bool learning() const;

    void setLimit(quint32 limit);
    quint32 limit() const;

    void setLocal(QString local);
    QString local() const;

    void setParent(QString parent);
    QString parent() const;

    void setProxy(bool enable);
    bool proxy() const;

    void setRemote(QString remote);
    QString remote() const;

    void setRsc(bool enable);
    bool rsc() const;

    void setSourcePortMax(quint32 maxPort);
    quint32 sourcePortMax() const;

    void setSourcePortMin(quint32 minPort);
    quint32 sourcePortMin() const;

    void setTos(quint32 tos);
    quint32 tos() const;

    void setTtl(quint32 ttl);
    quint32 ttl() const;

    void fromMap(const QVariantMap &setting) override;

    QVariantMap toMap() const override;

protected:
    VxlanSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(VxlanSetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const VxlanSetting &setting);

}

#endif // NETWORKMANAGERQT_VXLAN_SETTING_H
