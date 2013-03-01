/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>

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

#ifndef NMQT_SECRETAGENT_H
#define NMQT_SECRETAGENT_H

#include <QObject>
#include <QDBusContext>
#include <QDBusObjectPath>

#include "QtNetworkManager-export.h"
#include "generic-types.h"

namespace NetworkManager
{
    class SecretAgentPrivate;

    class NMQT_EXPORT SecretAgent : public QObject, protected QDBusContext
    {
    Q_OBJECT
    Q_DECLARE_PRIVATE(SecretAgent)
    public:
        explicit SecretAgent(const QString &, QObject * parent = 0);
        virtual ~SecretAgent();
    public Q_SLOTS:
        virtual QVariantMapMap GetSecrets(const QVariantMapMap&, const QDBusObjectPath&, const QString&, const QStringList&, uint) = 0;
        virtual void SaveSecrets(const QVariantMapMap&, const QDBusObjectPath&) = 0;
        virtual void DeleteSecrets(const QVariantMapMap &, const QDBusObjectPath &) = 0;
        virtual void CancelGetSecrets(const QDBusObjectPath &, const QString &) = 0;
    private:
        SecretAgentPrivate *d_ptr;
        Q_PRIVATE_SLOT(d_ptr, void registerAgent())
    };
}

#endif // NMQT_SECRETAGENT_H
