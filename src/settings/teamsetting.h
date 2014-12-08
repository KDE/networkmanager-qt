/*
    Copyright 2014 Lukáš Tinkl <ltinkl@redhat.com>

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

#ifndef NETWORKMANAGERQT_TEAM_SETTING_H
#define NETWORKMANAGERQT_TEAM_SETTING_H

#include <networkmanagerqt_export.h>
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{

class TeamSettingPrivate;

class NETWORKMANAGERQT_EXPORT TeamSetting : public Setting
{
public:
    typedef QSharedPointer<TeamSetting> Ptr;
    typedef QList<Ptr> List;
    TeamSetting();
    explicit TeamSetting(const Ptr &other);
    ~TeamSetting();

    QString name() const Q_DECL_OVERRIDE;

    void setInterfaceName(const QString &name);
    QString interfaceName() const;

    QString config() const;
    void setConfig(const QString &config);

    void fromMap(const QVariantMap &setting) Q_DECL_OVERRIDE;

    QVariantMap toMap() const Q_DECL_OVERRIDE;

protected:
    TeamSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(TeamSetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const TeamSetting &setting);

}

#endif // NETWORKMANAGERQT_TEAM_SETTING_H
