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

#ifndef NMQT_SETTINGS_CDMA_H
#define NMQT_SETTINGS_CDMA_H

#include "../NetworkManagerQt-export.h"
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{

class CdmaSettingPrivate;
class NETWORKMANAGERQT_EXPORT CdmaSetting : public Setting
{
public:
    typedef QSharedPointer<CdmaSetting> Ptr;
    typedef QList<Ptr> List;
    CdmaSetting();
    explicit CdmaSetting(const Ptr &other);
    ~CdmaSetting();

    QString name() const;

    void setNumber(const QString &number);
    QString number() const;

    void setUsername(const QString &username);
    QString username() const;

    void setPassword(const QString &password);
    QString password() const;

    void setPasswordFlags(SecretFlags flags);
    SecretFlags passwordFlags() const;

    QStringList needSecrets(bool requestNew = false) const;

    void secretsFromMap(const QVariantMap &secrets);

    QVariantMap secretsToMap() const;

    void fromMap(const QVariantMap &setting);

    QVariantMap toMap() const;

protected:
    CdmaSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(CdmaSetting)
};

QDebug operator<<(QDebug dbg, const CdmaSetting &setting);

}

#endif // NMQT_SETTINGS_CDMASETTING_H
