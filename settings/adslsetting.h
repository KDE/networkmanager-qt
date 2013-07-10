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

#ifndef NMQT_SETTINGS_ADSL_H
#define NMQT_SETTINGS_ADSL_H

#include "../NetworkManagerQt-export.h"
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{

class AdslSettingPrivate;
class NETWORKMANAGERQT_EXPORT AdslSetting : public Setting
{
public:
    typedef QSharedPointer<AdslSetting> Ptr;
    typedef QList<Ptr> List;
    enum Protocol {UnknownProtocol = 0, Pppoa, Pppoe, Ipoatm};
    enum Encapsulation {UnknownEncapsulation = 0, Vcmux, Llc};

    AdslSetting();
    explicit AdslSetting(const Ptr &other);
    ~AdslSetting();

    QString name() const;

    void setUsername(const QString &username);
    QString username() const;

    void setPassword(const QString &password);
    QString password() const;

    void setPasswordFlags(SecretFlags flags);
    SecretFlags passwordFlags() const;

    void setProtocol(Protocol protocol);
    Protocol protocol() const;

    void setEncapsulation(Encapsulation encapsulation);
    Encapsulation encapsulation() const;

    void setVpi(quint32 vpi);
    quint32 vpi() const;

    void setVci(quint32 vci);
    quint32 vci() const;

    QStringList needSecrets(bool requestNew = false) const;

    void fromMap(const QVariantMap &setting);

    QVariantMap toMap() const;

protected:
    AdslSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(AdslSetting)
};

QDebug operator<<(QDebug dbg, const AdslSetting &setting);

}

#endif // NMQT_SETTINGS_ADSL_H
