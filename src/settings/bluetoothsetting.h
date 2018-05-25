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

#ifndef NETWORKMANAGERQT_BLUETOOTH_SETTING_H
#define NETWORKMANAGERQT_BLUETOOTH_SETTING_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{

class BluetoothSettingPrivate;

/**
 * Represents bluetooth setting
 */
class NETWORKMANAGERQT_EXPORT BluetoothSetting : public Setting
{
public:
    typedef QSharedPointer<BluetoothSetting> Ptr;
    typedef QList<Ptr> List;
    enum ProfileType {Unknown = 0, Dun, Panu};

    BluetoothSetting();
    explicit BluetoothSetting(const Ptr &other);
    ~BluetoothSetting() override;

    QString name() const override;

    void setBluetoothAddress(const QByteArray &address);
    QByteArray bluetoothAddress() const;

    void setProfileType(ProfileType type);
    ProfileType profileType() const;

    void fromMap(const QVariantMap &setting) override;

    QVariantMap toMap() const override;

protected:
    BluetoothSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(BluetoothSetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const BluetoothSetting &setting);

}

#endif // NETWORKMANAGERQT_SETTINGS_BLUETOOTH_H
