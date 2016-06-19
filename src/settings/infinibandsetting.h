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

#ifndef NETWORKMANAGERQT_INFINIBAND_SETTING_H
#define NETWORKMANAGERQT_INFINIBAND_SETTING_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{

class InfinibandSettingPrivate;

/**
 * Represents infiniband setting
 */
class NETWORKMANAGERQT_EXPORT InfinibandSetting : public Setting
{
public:
    typedef QSharedPointer<InfinibandSetting> Ptr;
    typedef QList<Ptr> List;
    enum TransportMode {Unknown = 0, Datagram, Connected};

    InfinibandSetting();
    explicit InfinibandSetting(const Ptr &other);
    ~InfinibandSetting();

    QString name() const Q_DECL_OVERRIDE;

    void setMacAddress(const QByteArray &address);
    QByteArray macAddress() const;

    void setMtu(quint32 mtu);
    quint32 mtu() const;

    void setTransportMode(TransportMode mode);
    TransportMode transportMode() const;

    void setPKey(qint32 key);
    qint32 pKey() const;

    void setParent(const QString &parent);
    QString parent() const;

    void fromMap(const QVariantMap &setting) Q_DECL_OVERRIDE;

    QVariantMap toMap() const Q_DECL_OVERRIDE;

protected:
    InfinibandSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(InfinibandSetting)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const InfinibandSetting &setting);

}

#endif // NETWORKMANAGERQT_INFINIBAND_SETTING_H
