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

#ifndef NETWORKMANAGERQT_DCBSETTING_H
#define NETWORKMANAGERQT_DCBSETTING_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "setting.h"

#include <QString>

namespace NetworkManager
{

class DcbSettingPrivate;

/**
 * Represents Dcb setting
 */
class NETWORKMANAGERQT_EXPORT DcbSetting : public Setting
{
public:
    typedef QSharedPointer<DcbSetting> Ptr;
    typedef QList<Ptr> List;

    enum DcbFlagType {
        None = 0,
        Enable = 0x01,
        Advertise = 0x02,
        Willing = 0x04
    };
    Q_DECLARE_FLAGS(DcbFlags, DcbFlagType)

    DcbSetting();
    explicit DcbSetting(const Ptr &other);
    ~DcbSetting() override;

    QString name() const override;

    void setAppFcoeMode(const QString &mode);
    QString appFcoeMode() const;

    void setAppFcoePriority(qint32 priority);
    qint32 appFcoePriority() const;

    void setAppFipPriority(qint32 priority);
    qint32 appFipPriority() const;

    void setAppIscsiPriority(qint32 priority);
    qint32 appIscsiPriority() const;

    void setAppFcoeFlags(DcbFlags flags);
    DcbFlags appFcoeFlags() const;

    void setAppFipFlags(DcbFlags flags);
    DcbFlags appFipFlags() const;

    void setAppIscsiFlags(DcbFlags flags);
    DcbFlags appIscsiFlags() const;

    void setPriorityGroupFlags(DcbFlags flags);
    DcbFlags priorityGroupFlags() const;

    void setPriorityFlowControlFlags(DcbFlags flags);
    DcbFlags priorityFlowControlFlags() const;

    void setPriorityFlowControl(quint32 userPriority, bool enabled);
    bool priorityFlowControl(quint32 userPriority) const;
    void setPriorityFlowControl(const UIntList &list);
    UIntList priorityFlowControl() const;

    void setPriorityBandwidth(quint32 userPriority, quint32 bandwidthPercent);
    quint32 priorityBandwidth(quint32 userPriority) const;
    void setPriorityBandwidth(const UIntList &list);
    UIntList priorityBandwidth() const;

    void setPriorityGroupBandwidth(quint32 groupId, quint32 bandwidthPercent);
    quint32 priorityGroupBandwidth(quint32 groupId) const;
    void setPriorityGroupBandwidth(const UIntList &list);
    UIntList priorityGroupBandwidth() const;

    void setPriorityGroupId(quint32 userPriority, quint32 groupId);
    quint32 priorityGroupId(quint32 userPriority) const;
    void setPriorityGroupId(const UIntList &list);
    UIntList priorityGroupId() const;

    void setPriorityStrictBandwidth(quint32 userPriority, bool strict);
    bool priorityStrictBandwidth(quint32 userPriority) const;
    void setPriorityStrictBandwidth(const UIntList &list);
    UIntList priorityStrictBandwidth() const;

    void setPriorityTrafficClass(quint32 userPriority, quint32 trafficClass);
    quint32 priorityTrafficClass(quint32 userPriority) const;
    void setPriorityTrafficClass(const UIntList &list);
    UIntList priorityTrafficClass() const;

    void fromMap(const QVariantMap &setting) override;

    QVariantMap toMap() const override;

protected:
    DcbSettingPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(DcbSetting)
};
Q_DECLARE_OPERATORS_FOR_FLAGS(DcbSetting::DcbFlags)

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const DcbSetting &setting);

}

#endif // NETWORKMANAGERQT_DCBSETTING_H
