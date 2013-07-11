/*
Copyright 2011 Ilia Kats <ilia-kats@gmx.net>
Copyright 2011-2013 Lamarque V. Souza <lamarque@kde.org>
Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>

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

#ifndef NMQT_WIMAXNSP_H
#define NMQT_WIMAXNSP_H

#include "NetworkManagerQt-export.h"

#include <QSharedPointer>
#include <QVariantMap>

namespace NetworkManager {

class WimaxNspPrivate;

class NETWORKMANAGERQT_EXPORT WimaxNsp : public QObject
{
    Q_OBJECT
public:
    typedef QSharedPointer<WimaxNsp> Ptr;
    typedef QList<Ptr> List;
    /**
     * network types a NSP can have
     */
    enum NetworkType { Unknown = 0x1, Home = 0x2, Partner = 0x3, RoamingPartner = 0x4 };

    explicit WimaxNsp(const QString &path, QObject *parent = 0);
    virtual ~WimaxNsp();

    QString uni() const;
    NetworkType networkType() const;
    QString name() const;
    uint signalQuality() const;

Q_SIGNALS:
    /**
     * This signal is emitted when the network type of this NSP has changed.
     *
     * @param type the new type
     */
    void networkTypeChanged(NetworkType type);

    /**
     * This signal is emitted when the name of this NSP has changed
     *
     * @param name the new name for this NSP
     */
    void nameChanged(const QString &name);

    /**
     * This signal is emitted when the signal quality of this NSP has changed.
     *
     * @param quality the new quality
     */
    void signalQualityChanged(uint quality);

protected Q_SLOTS:
    void propertiesChanged(const QVariantMap &properties);

private:
    Q_DECLARE_PRIVATE(WimaxNsp)

    WimaxNspPrivate *const d_ptr;
};
}
#endif


