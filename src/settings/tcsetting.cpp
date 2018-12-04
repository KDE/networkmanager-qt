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

#include "tcsetting.h"
#include "tcsetting_p.h"

#include <QDebug>

#if !NM_CHECK_VERSION(1, 10, 0)
#define NM_SETTING_TC_CONFIG_SETTING_NAME    "tc"

#define NM_SETTING_TC_CONFIG_QDISCS          "qdiscs"
#define NM_SETTING_TC_CONFIG_TFILTERS        "tfilters"
#endif

NetworkManager::TcSettingPrivate::TcSettingPrivate()
    : name(NM_SETTING_TC_CONFIG_SETTING_NAME)
{ }

NetworkManager::TcSetting::TcSetting()
    : Setting(Setting::Tc)
    , d_ptr(new TcSettingPrivate())
{ }

NetworkManager::TcSetting::TcSetting(const Ptr &other)
    : Setting(other)
    , d_ptr(new TcSettingPrivate())
{
    setQdiscs(other->qdiscs());
    setTfilters(other->tfilters());
}

NetworkManager::TcSetting::~TcSetting()
{
    delete d_ptr;
}

QString NetworkManager::TcSetting::name() const
{
    Q_D(const TcSetting);

    return d->name;
}

void NetworkManager::TcSetting::setQdiscs(const NMVariantMapList &qdiscs)
{
    Q_D(TcSetting);

    d->qdiscs = qdiscs;
}

NMVariantMapList NetworkManager::TcSetting::qdiscs() const
{
    Q_D(const TcSetting);

    return d->qdiscs;
}

void NetworkManager::TcSetting::setTfilters(const NMVariantMapList &tfilters)
{
    Q_D(TcSetting);

    d->tfilters = tfilters;
}

NMVariantMapList NetworkManager::TcSetting::tfilters() const
{
    Q_D(const TcSetting);

    return d->tfilters;
}

void NetworkManager::TcSetting::fromMap(const QVariantMap &setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_TC_CONFIG_QDISCS))) {
        setQdiscs(qdbus_cast<NMVariantMapList>(setting.value(QLatin1String(NM_SETTING_TC_CONFIG_QDISCS))));
    }

    if (setting.contains(QLatin1String(NM_SETTING_TC_CONFIG_TFILTERS))) {
        setTfilters(qdbus_cast<NMVariantMapList>(setting.value(QLatin1String(NM_SETTING_TC_CONFIG_TFILTERS))));
    }
}

QVariantMap NetworkManager::TcSetting::toMap() const
{
    QVariantMap setting;

    if (!qdiscs().empty()) {
        setting.insert(QLatin1String(NM_SETTING_TC_CONFIG_QDISCS), QVariant::fromValue(qdiscs()));
    }

    if (!tfilters().empty()) {
        setting.insert(QLatin1String(NM_SETTING_TC_CONFIG_TFILTERS), QVariant::fromValue(tfilters()));
    }

    return setting;
}

QDebug NetworkManager::operator <<(QDebug dbg, const NetworkManager::TcSetting &setting)
{
    dbg.nospace() << "type: " << setting.typeAsString(setting.type()) << '\n';
    dbg.nospace() << "initialized: " << !setting.isNull() << '\n';

    dbg.nospace() << NM_SETTING_TC_CONFIG_QDISCS << ": " << '\n';
    Q_FOREACH (const QVariantMap & qdisc, setting.qdiscs()) {
        QVariantMap::const_iterator i = qdisc.constBegin();
        while (i != qdisc.constEnd()) {
            dbg.nospace() << i.key() << ": " << i.value() << '\n';
        }
    }
    dbg.nospace() << NM_SETTING_TC_CONFIG_TFILTERS << ": " << '\n';
    Q_FOREACH (const QVariantMap & tfilter, setting.tfilters()) {
        QVariantMap::const_iterator i = tfilter.constBegin();
        while (i != tfilter.constEnd()) {
            dbg.nospace() << i.key() << ": " << i.value() << '\n';
        }
    }

    return dbg.maybeSpace();
}

