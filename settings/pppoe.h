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

#ifndef NMQT_SETTINGS_PPPOE_H
#define NMQT_SETTINGS_PPPOE_H

#include "../QtNetworkManager-export.h"
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{
namespace Settings
{
class PppoeSettingPrivate;

class NMQT_EXPORT PppoeSetting : public Setting
{
    Q_DECLARE_PRIVATE(PppoeSetting)
public:
    PppoeSetting();
    PppoeSetting(PppoeSetting *);
    ~PppoeSetting();

    QString name() const;

    void setService(const QString & service);
    QString service() const;

    void setUsername(const QString & username);
    QString username() const;

    void setPassword(const QString & password);
    QString password() const;

    void setPasswordFlags(Setting::SecretFlags flags);
    Setting::SecretFlags passwordFlags() const;

    QStringList needSecrets(bool requestNew = false) const;

    void secretsFromMap(const QVariantMap & secrets);

    QVariantMap secretsToMap() const;

    void fromMap(const QVariantMap & setting);

    QVariantMap toMap() const;

    void printSetting();

protected:
    PppoeSettingPrivate * d_ptr;
};

}
}

#endif // NMQT_SETTINGS_PPPOE_H
