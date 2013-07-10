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

#ifndef NMQT_SETTINGS_VPN_H
#define NMQT_SETTINGS_VPN_H

#include "../NetworkManagerQt-export.h"
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{

class VpnSettingPrivate;
class NETWORKMANAGERQT_EXPORT VpnSetting : public Setting
{
public:
    typedef QSharedPointer<VpnSetting> Ptr;
    typedef QList<Ptr> List;
    VpnSetting();
    explicit VpnSetting(const Ptr &other);
    ~VpnSetting();

    QString name() const;

    void setServiceType(const QString &type);
    QString serviceType() const;

    void setUsername(const QString &username);
    QString username() const;

    void setData(const NMStringMap &data);
    NMStringMap data() const;

    void setSecrets(const NMStringMap &secrets);
    NMStringMap secrets() const;

    void secretsFromMap(const QVariantMap &secrets);
    QVariantMap secretsToMap() const;

    void secretsFromStringMap(const NMStringMap &map);
    NMStringMap secretsToStringMap() const;

    void fromMap(const QVariantMap &setting);

    QVariantMap toMap() const;

protected:
    VpnSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(VpnSetting)
};

QDebug operator<<(QDebug dbg, const VpnSetting &setting);

}

#endif // NMQT_SETTINGS_VPN_H
