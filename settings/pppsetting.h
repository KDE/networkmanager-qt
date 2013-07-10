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

#ifndef NMQT_SETTINGS_PPP_H
#define NMQT_SETTINGS_PPP_H

#include "../NetworkManagerQt-export.h"
#include "setting.h"

#include <QtCore/QString>

namespace NetworkManager
{

class PppSettingPrivate;
class NETWORKMANAGERQT_EXPORT PppSetting : public Setting
{
public:
    typedef QSharedPointer<PppSetting> Ptr;
    typedef QList<Ptr> List;
    PppSetting();
    explicit PppSetting(const Ptr &other);
    ~PppSetting();

    QString name() const;

    void setNoAuth(bool require);
    bool noAuth() const;

    void setRefuseEap(bool refuse);
    bool refuseEap() const;

    void setRefusePap(bool refuse);
    bool refusePap() const;

    void setRefuseChap(bool refuse);
    bool refuseChap() const;

    void setRefuseMschap(bool refuse);
    bool refuseMschap() const;

    void setRefuseMschapv2(bool refuse);
    bool refuseMschapv2() const;

    void setNoBsdComp(bool require);
    bool noBsdComp() const;

    void setNoDeflate(bool require);
    bool noDeflate() const;

    void setNoVjComp(bool require);
    bool noVjComp() const;

    void setRequireMppe(bool require);
    bool requireMppe() const;

    void setRequireMppe128(bool require);
    bool requireMppe128() const;

    void setMppeStateful(bool used);
    bool mppeStateful() const;

    void setCRtsCts(bool control);
    bool cRtsCts() const;

    void setBaud(quint32 baud);
    quint32 baud() const;

    void setMru(quint32 mru);
    quint32 mru() const;

    void setMtu(quint32 mtu);
    quint32 mtu() const;

    void setLcpEchoFailure(quint32 number);
    quint32 lcpEchoFailure() const;

    void setLcpEchoInterval(quint32 interval);
    quint32 lcpEchoInterval() const;

    void fromMap(const QVariantMap &setting);

    QVariantMap toMap() const;

protected:
    PppSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(PppSetting)
};

QDebug operator<<(QDebug dbg, const PppSetting &setting);

}

#endif // NMQT_SETTINGS_PPP_H
