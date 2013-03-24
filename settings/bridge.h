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

#ifndef NMQT_SETTINGS_BRIDGE_H
#define NMQT_SETTINGS_BRIDGE_H

#include "../QtNetworkManager-export.h"
#include "generic-types.h"
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{
namespace Settings
{
class BridgeSettingPrivate;

class NMQT_EXPORT BridgeSetting : public Setting
{
    Q_DECLARE_PRIVATE(BridgeSetting)
public:
    BridgeSetting();
    BridgeSetting(BridgeSetting *);
    ~BridgeSetting();

    QString name() const;

    void setInterfaceName(const QString & name);
    QString interfaceName() const;

    void setStp(bool enabled);
    bool stp() const;

    void setPriority(quint16 priority);
    quint16 priority() const;

    void setForwardDelay(quint16 delay);
    quint16 forwardDelay() const;

    void setHelloTime(quint16 time);
    quint16 helloTime() const;

    void setMaxAge(quint16 age);
    quint16 maxAge() const;

    void setAgeintTime(quint32 time);
    quint32 ageintTime() const;

    void fromMap(const QVariantMap & setting);

    QVariantMap toMap() const;

    void printSetting();

protected:
    BridgeSettingPrivate * d_ptr;
};
}
}

#endif // NMQT_SETTINGS_BRIDGE_H
