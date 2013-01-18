/*
    Copyright 2012  Jan Grulich <jgrulich@redhat.com>

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

#ifndef NM_CDMA_H
#define NM_CDMA_H

#include "../QtNetworkManager-export.h"
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{
namespace Settings
{
    class CdmaSettingPrivate;

    class NMQT_EXPORT CdmaSetting : public Setting
    {
    Q_DECLARE_PRIVATE(CdmaSetting)
    public:
        CdmaSetting();
        CdmaSetting(CdmaSetting *);
        ~CdmaSetting();

        QString name() const;

        void setNumber(const QString & number);
        QString number() const;

        void setUsername(const QString & username);
        QString username() const;

        void setPassword(const QString & password);
        QString password() const;

        void setPasswordFlags(SecretFlagType flags);
        SecretFlagType passwordFlags() const;

        QStringList needSecrets() const;

        void secretsFromMap(const QVariantMap & secrets);

        QVariantMap secretsToMap() const;

        void fromMap(const QVariantMap & setting);

        QVariantMap toMap() const;

        //FOR DEBUG
        void printSetting();

    private:
        CdmaSettingPrivate * d_ptr;
    };
}
}

#endif // NM_CDMA_H
