/*
    Copyright 2018 Pranav Gade <pranavgade20@gmail.com>

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

#ifndef NETWORKMANAGERQT_MACSECSETTING_H
#define NETWORKMANAGERQT_MACSECSETTING_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "setting.h"

namespace NetworkManager
{

class MacsecSettingPrivate;

/**
 * Represents Macsec setting
 */
class NETWORKMANAGERQT_EXPORT MacsecSetting : public Setting
{
public:
    typedef QSharedPointer<MacsecSetting> Ptr;
    typedef QList<Ptr> List;

    enum Mode {Psk, Eap};

    enum Validation {Disable, Check, Strict};

    MacsecSetting();
    explicit MacsecSetting(const Ptr &other);
    ~MacsecSetting() override;

    QString name() const override;

    void setEncrypt(bool encrypt);
    bool encrypt() const;

    void setMkaCak(const QString &mkaCak);
    QString mkaCak() const;

    void setMkaCkn(const QString &mkaCkn);
    QString mkaCkn() const;

    void setMode(Mode mode);
    Mode mode() const;

    void setParent(const QString &parent);
    QString parent() const;

    void setPort(qint32 port);
    qint32 port() const;

    void setSendSci(bool sendSci);
    bool sendSci() const;

    void setValidation(Validation validation);
    Validation validation() const;

    void setMkaCakFlags(Setting::SecretFlags flags);
    Setting::SecretFlags mkaCakFlags() const;

    QStringList needSecrets(bool requestNew = false) const override;

    void secretsFromMap(const QVariantMap &secrets) override;

    QVariantMap secretsToMap() const override;

    void fromMap(const QVariantMap &setting) override;

    QVariantMap toMap() const override;

protected:
    MacsecSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(MacsecSetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const MacsecSetting &setting);

}

#endif // NETWORKMANAGERQT_MACSECSETTING_H

