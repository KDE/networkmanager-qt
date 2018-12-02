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

#ifndef NETWORKMANAGERQTOVS_PORT_SETTING_H
#define NETWORKMANAGERQTOVS_PORT_SETTING_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "setting.h"

namespace NetworkManager
{

class OvsPortSettingPrivate;

/**
 * Represents OvsPort setting
 */
class NETWORKMANAGERQT_EXPORT OvsPortSetting : public Setting
{
public:
    typedef QSharedPointer<OvsPortSetting> Ptr;
    typedef QList<Ptr> List;

    OvsPortSetting();
    explicit OvsPortSetting(const Ptr &other);
    ~OvsPortSetting() override;

    QString name() const override;

    void setBondDowndelay(quint32 delay);
    quint32 bondDowndelay() const;

    void setBondUpdelay(quint32 delay);
    quint32 bondUpdelay() const;

    void setTag(quint32 tag);
    quint32 tag() const;

    void setBondMode(const QString &mode);
    QString bondMode() const;

    void setLacp(const QString &lacp);
    QString lacp() const;

    void setVlanMode(const QString &mode);
    QString vlanMode() const;

    void fromMap(const QVariantMap &setting) override;

    QVariantMap toMap() const override;

protected:
    OvsPortSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(OvsPortSetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const OvsPortSetting &setting);

}

#endif // NETWORKMANAGERQT_OVS_PORT_SETTING_H
