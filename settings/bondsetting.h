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

#ifndef NMQT_SETTINGS_BOND_H
#define NMQT_SETTINGS_BOND_H

#include "../NetworkManagerQt-export.h"
#include "setting.h"

#include <QtCore/QString>

/* Valid options for the 'options' property */
#define NM_SETTING_BOND_OPTION_MODE          "mode"
#define NM_SETTING_BOND_OPTION_MIIMON        "miimon"
#define NM_SETTING_BOND_OPTION_DOWNDELAY     "downdelay"
#define NM_SETTING_BOND_OPTION_UPDELAY       "updelay"
#define NM_SETTING_BOND_OPTION_ARP_INTERVAL  "arp_interval"
#define NM_SETTING_BOND_OPTION_ARP_IP_TARGET "arp_ip_target"

namespace NetworkManager
{

class BondSettingPrivate;
class NETWORKMANAGERQT_EXPORT BondSetting : public Setting
{
public:
    typedef QSharedPointer<BondSetting> Ptr;
    typedef QList<Ptr> List;
    BondSetting();
    explicit BondSetting(const Ptr &other);
    ~BondSetting();

    QString name() const;

    void setInterfaceName(const QString &name);
    QString interfaceName() const;

    void addOption(const QString &option, const QString &value);
    void setOptions(const NMStringMap &options);
    NMStringMap options() const;

    void fromMap(const QVariantMap &setting);

    QVariantMap toMap() const;

protected:
    BondSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(BondSetting)
};

QDebug operator<<(QDebug dbg, const BondSetting &setting);

}

#endif // NMQT_SETTINGS_BOND_H
