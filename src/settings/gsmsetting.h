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

#ifndef NETWORKMANAGERQT_GSM_SETTING_H
#define NETWORKMANAGERQT_GSM_SETTING_H

#include <networkmanagerqt_export.h>
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{

class GsmSettingPrivate;
class NETWORKMANAGERQT_EXPORT GsmSetting : public Setting
{
public:
    typedef QSharedPointer<GsmSetting> Ptr;
    typedef QList<Ptr> List;
    enum NetworkType { Any = -1, Only3G, GprsEdgeOnly, Prefer3G, Prefer2G, Prefer4GLte, Only4GLte };

    GsmSetting();
    explicit GsmSetting(const Ptr &other);
    ~GsmSetting();

    QString name() const Q_DECL_OVERRIDE;

    void setNumber(const QString &number);
    QString number() const;

    void setUsername(const QString &username);
    QString username() const;

    void setPassword(const QString &password);
    QString password() const;

    void setPasswordFlags(SecretFlags flags);
    SecretFlags passwordFlags() const;

    void setApn(const QString &apn);
    QString apn() const;

    void setNetworkId(const QString &id);
    QString networkId() const;

    void setNetworkType(NetworkType type);
    NetworkType networkType() const;

    void setPin(const QString &pin);
    QString pin() const;

    void setPinFlags(SecretFlags flags);
    SecretFlags pinFlags() const;

    void setAllowedBand(quint32 band);
    quint32 allowedBand() const;

    void setHomeOnly(bool homeOnly);
    bool homeOnly() const;

    void secretsFromMap(const QVariantMap &secrets) Q_DECL_OVERRIDE;

    QVariantMap secretsToMap() const Q_DECL_OVERRIDE;

    QStringList needSecrets(bool requestNew = false) const Q_DECL_OVERRIDE;

    void fromMap(const QVariantMap &setting) Q_DECL_OVERRIDE;

    QVariantMap toMap() const Q_DECL_OVERRIDE;

protected:
    GsmSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(GsmSetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const GsmSetting &setting);

}

#endif // NETWORKMANAGERQT_GSM_SETTING_H
