/*
    Copyright Pranav Gade <pranavgade20@gmail.com>

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

#include "ovspatchsetting.h"
#include "ovspatchsetting_p.h"

#include <QDebug>

#if !NM_CHECK_VERSION(1, 10, 0)
#define NM_SETTING_OVS_PATCH_SETTING_NAME   "ovs-patch"
#define NM_SETTING_OVS_PATCH_PEER           "peer"
#endif

NetworkManager::OvsPatchSettingPrivate::OvsPatchSettingPrivate()
    : name(NM_SETTING_OVS_PATCH_SETTING_NAME)
{ }

NetworkManager::OvsPatchSetting::OvsPatchSetting()
    : Setting(Setting::OvsPatch)
    , d_ptr(new OvsPatchSettingPrivate())
{ }

NetworkManager::OvsPatchSetting::OvsPatchSetting(const Ptr &other)
    : Setting(other)
    , d_ptr(new OvsPatchSettingPrivate())
{
    setPeer(other->peer());
}

NetworkManager::OvsPatchSetting::~OvsPatchSetting()
{
    delete d_ptr;
}

QString NetworkManager::OvsPatchSetting::name() const
{
    Q_D(const OvsPatchSetting);

    return d->name;
}

void NetworkManager::OvsPatchSetting::setPeer(const QString &peer)
{
    Q_D(OvsPatchSetting);

    d->peer = peer;
}

QString NetworkManager::OvsPatchSetting::peer() const
{
    Q_D(const OvsPatchSetting);

    return d->peer;
}

void NetworkManager::OvsPatchSetting::fromMap(const QVariantMap &setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_OVS_PATCH_PEER))) {
        setPeer(setting.value(QLatin1String(NM_SETTING_OVS_PATCH_PEER)).toString());
    }
}

QVariantMap NetworkManager::OvsPatchSetting::toMap() const
{
    QVariantMap setting;

    if (!peer().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_OVS_PATCH_PEER), peer());
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::OvsPatchSetting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    dbg.nospace() << NM_SETTING_OVS_PATCH_PEER << ": " << setting.peer() << '\n';

    return dbg.maybeSpace();
}
