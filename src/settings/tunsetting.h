/*
    Copyright 2016 Jan Grulich <jgrulich@redhat.com>

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

#ifndef NETWORKMANAGERQT_TUN_SETTING_H
#define NETWORKMANAGERQT_TUN_SETTING_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "setting.h"

#include <QString>

namespace NetworkManager
{

class TunSettingPrivate;

/**
 * Represents generic setting
 */
class NETWORKMANAGERQT_EXPORT TunSetting : public Setting
{
public:
    typedef QSharedPointer<TunSetting> Ptr;
    typedef QList<Ptr> List;
    enum Mode { Tun, Tap };


    TunSetting();
    explicit TunSetting(const Ptr &other);
    ~TunSetting() override;

    QString name() const override;

    void setGroup(const QString& group);
    QString group() const;

    void setMode(Mode mode);
    Mode mode() const;

    void setMultiQueue(bool multiQueue);
    bool multiQueue() const;

    void setOwner(const QString& owner);
    QString owner() const;

    void setPi(bool pi);
    bool pi() const;

    void setVnetHdr(bool vnetHdr);
    bool vnetHdr() const;

    void fromMap(const QVariantMap &setting) override;

    QVariantMap toMap() const override;

protected:
    TunSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(TunSetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const TunSetting &setting);

}

#endif // NETWORKMANAGERQT_TUN_SETTING_H

