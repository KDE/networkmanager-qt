/*
    Copyright 2012-2013  Jan Grulich <jgrulich@redhat.com>
    Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>

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

#ifndef NMQT_SETTINGS_WIMAX_H
#define NMQT_SETTINGS_WIMAX_H

#include "../NetworkManagerQt-export.h"
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{

class WimaxSettingPrivate;
class NETWORKMANAGERQT_EXPORT WimaxSetting : public Setting
{
public:
    typedef QSharedPointer<WimaxSetting> Ptr;
    typedef QList<Ptr> List;
    WimaxSetting();
    explicit WimaxSetting(const Ptr &other);
    ~WimaxSetting();

    QString name() const;

    void setNetworkName(const QString &name);
    QString networkName() const;

    void setMacAddress(const QByteArray &address);
    QByteArray macAddress() const;

    void fromMap(const QVariantMap &setting);

    QVariantMap toMap() const;

protected:
    WimaxSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(WimaxSetting)
};

QDebug operator<<(QDebug dbg, const WimaxSetting &setting);

}

#endif // NMQT_SETTINGS_WIMAX_H
