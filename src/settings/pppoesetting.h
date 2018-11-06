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

#ifndef NETWORKMANAGERQT_PPPOE_SETTING_H
#define NETWORKMANAGERQT_PPPOE_SETTING_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "setting.h"

#include <QString>

namespace NetworkManager
{

class PppoeSettingPrivate;

/**
 * Represents pppoe setting
 */
class NETWORKMANAGERQT_EXPORT PppoeSetting : public Setting
{
public:
    typedef QSharedPointer<PppoeSetting> Ptr;
    typedef QList<Ptr> List;
    PppoeSetting();
    explicit PppoeSetting(const Ptr &other);
    ~PppoeSetting() override;

    QString name() const override;

    void setService(const QString &service);
    QString service() const;

    void setUsername(const QString &username);
    QString username() const;

    void setPassword(const QString &password);
    QString password() const;

    void setPasswordFlags(Setting::SecretFlags flags);
    Setting::SecretFlags passwordFlags() const;

    QStringList needSecrets(bool requestNew = false) const override;

    void secretsFromMap(const QVariantMap &secrets) override;

    QVariantMap secretsToMap() const override;

    void fromMap(const QVariantMap &setting) override;

    QVariantMap toMap() const override;

protected:
    PppoeSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(PppoeSetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const PppoeSetting &setting);

}

#endif // NETWORKMANAGERQT_PPPOE_SETTING_H
