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

#include "ppp.h"
#include "ppp_p.h"

#include <nm-setting-ppp.h>

#include <QtCore/QDebug>

NetworkManager::Settings::PppSettingPrivate::PppSettingPrivate():
    name(NM_SETTING_PPP_SETTING_NAME),
    noauth(true),
    refuseEap(false),
    refusePap(false),
    refuseChap(false),
    refuseMschap(false),
    refuseMschapv2(false),
    nobsdcomp(false),
    nodeflate(false),
    noVjComp(false),
    requireMppe(false),
    requireMppe128(false),
    mppeStateful(false),
    crtscts(false),
    baud(0),
    mru(0),
    mtu(0),
    lcpEchoFailure(0),
    lcpEchoInterval(0)
{ }

NetworkManager::Settings::PppSetting::PppSetting():
    Setting(Setting::Ppp),
    d_ptr(new PppSettingPrivate())
{ }

NetworkManager::Settings::PppSetting::PppSetting(const Ptr &other):
    Setting(other),
    d_ptr(new PppSettingPrivate())
{
    setNoAuth(other->noAuth());
    setRefuseEap(other->refuseEap());
    setRefusePap(other->refusePap());
    setRefuseChap(other->refuseChap());
    setRefuseMschap(other->refuseMschap());
    setRefuseMschapv2(other->refuseMschapv2());
    setNoBsdComp(other->noBsdComp());
    setNoDeflate(other->noDeflate());
    setNoVjComp(other->noVjComp());
    setRequireMppe(other->requireMppe());
    setRequireMppe128(other->requireMppe128());
    setMppeStateful(other->mppeStateful());
    setCRtsCts(other->cRtsCts());
    setBaud(other->baud());
    setMru(other->mru());
    setMtu(other->mtu());
    setLcpEchoFailure(other->lcpEchoFailure());
    setLcpEchoInterval(other->lcpEchoInterval());
}

NetworkManager::Settings::PppSetting::~PppSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::PppSetting::name() const
{
    Q_D(const PppSetting);

    return d->name;
}

void NetworkManager::Settings::PppSetting::setNoAuth(bool require)
{
    Q_D(PppSetting);

    d->noauth = require;
}

bool NetworkManager::Settings::PppSetting::noAuth() const
{
    Q_D(const PppSetting);

    return d->noauth;
}

void NetworkManager::Settings::PppSetting::setRefuseEap(bool refuse)
{
    Q_D(PppSetting);

    d->refuseEap = refuse;
}

bool NetworkManager::Settings::PppSetting::refuseEap() const
{
    Q_D(const PppSetting);

    return d->refuseEap;
}

void NetworkManager::Settings::PppSetting::setRefusePap(bool refuse)
{
    Q_D(PppSetting);

    d->refusePap = refuse;
}

bool NetworkManager::Settings::PppSetting::refusePap() const
{
    Q_D(const PppSetting);

    return d->refusePap;
}

void NetworkManager::Settings::PppSetting::setRefuseChap(bool refuse)
{
    Q_D(PppSetting);

    d->refuseChap = refuse;
}

bool NetworkManager::Settings::PppSetting::refuseChap() const
{
    Q_D(const PppSetting);

    return d->refuseChap;
}

void NetworkManager::Settings::PppSetting::setRefuseMschap(bool refuse)
{
    Q_D(PppSetting);

    d->refuseMschap = refuse;
}

bool NetworkManager::Settings::PppSetting::refuseMschap() const
{
    Q_D(const PppSetting);

    return d->refuseMschap;
}

void NetworkManager::Settings::PppSetting::setRefuseMschapv2(bool refuse)
{
    Q_D(PppSetting);

    d->refuseMschapv2 = refuse;
}

bool NetworkManager::Settings::PppSetting::refuseMschapv2() const
{
    Q_D(const PppSetting);

    return d->refuseMschapv2;
}

void NetworkManager::Settings::PppSetting::setNoBsdComp(bool require)
{
    Q_D(PppSetting);

    d->nobsdcomp = require;
}

bool NetworkManager::Settings::PppSetting::noBsdComp() const
{
    Q_D(const PppSetting);

    return d->nobsdcomp;
}

void NetworkManager::Settings::PppSetting::setNoDeflate(bool require)
{
    Q_D(PppSetting);

    d->nodeflate = require;
}

bool NetworkManager::Settings::PppSetting::noDeflate() const
{
    Q_D(const PppSetting);

    return d->nodeflate;
}

void NetworkManager::Settings::PppSetting::setNoVjComp(bool require)
{
    Q_D(PppSetting);

    d->noVjComp = require;
}

bool NetworkManager::Settings::PppSetting::noVjComp() const
{
    Q_D(const PppSetting);

    return d->noVjComp;
}

void NetworkManager::Settings::PppSetting::setRequireMppe(bool require)
{
    Q_D(PppSetting);

    d->requireMppe = require;
}

bool NetworkManager::Settings::PppSetting::requireMppe() const
{
    Q_D(const PppSetting);

    return d->requireMppe;
}

void NetworkManager::Settings::PppSetting::setRequireMppe128(bool require)
{
    Q_D(PppSetting);

    d->requireMppe128 = require;
}

bool NetworkManager::Settings::PppSetting::requireMppe128() const
{
    Q_D(const PppSetting);

    return d->requireMppe128;
}

void NetworkManager::Settings::PppSetting::setMppeStateful(bool used)
{
    Q_D(PppSetting);

    d->mppeStateful = used;
}

bool NetworkManager::Settings::PppSetting::mppeStateful() const
{
    Q_D(const PppSetting);

    return d->mppeStateful;
}

void NetworkManager::Settings::PppSetting::setCRtsCts(bool control)
{
    Q_D(PppSetting);

    d->crtscts = control;
}

bool NetworkManager::Settings::PppSetting::cRtsCts() const
{
    Q_D(const PppSetting);

    return d->crtscts;
}

void NetworkManager::Settings::PppSetting::setBaud(quint32 baud)
{
    Q_D(PppSetting);

    d->baud = baud;
}

quint32 NetworkManager::Settings::PppSetting::baud() const
{
    Q_D(const PppSetting);

    return d->baud;
}

void NetworkManager::Settings::PppSetting::setMru(quint32 mru)
{
    Q_D(PppSetting);

    d->mru = mru;
}

quint32 NetworkManager::Settings::PppSetting::mru() const
{
    Q_D(const PppSetting);

    return d->mru;
}

void NetworkManager::Settings::PppSetting::setMtu(quint32 mtu)
{
    Q_D(PppSetting);

    d->mtu = mtu;
}

quint32 NetworkManager::Settings::PppSetting::mtu() const
{
    Q_D(const PppSetting);

    return d->mtu;
}

void NetworkManager::Settings::PppSetting::setLcpEchoFailure(quint32 number)
{
    Q_D(PppSetting);

    d->lcpEchoFailure = number;
}

quint32 NetworkManager::Settings::PppSetting::lcpEchoFailure() const
{
    Q_D(const PppSetting);

    return d->lcpEchoFailure;
}

void NetworkManager::Settings::PppSetting::setLcpEchoInterval(quint32 interval)
{
    Q_D(PppSetting);

    d->lcpEchoInterval = interval;
}

quint32 NetworkManager::Settings::PppSetting::lcpEchoInterval() const
{
    Q_D(const PppSetting);

    return d->lcpEchoInterval;
}

void NetworkManager::Settings::PppSetting::fromMap(const QVariantMap& setting)
{
    if (setting.contains(QLatin1String(NM_SETTING_PPP_NOAUTH))) {
        setNoAuth(setting.value(QLatin1String(NM_SETTING_PPP_NOAUTH)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_REFUSE_EAP))) {
        setRefuseEap(setting.value(QLatin1String(NM_SETTING_PPP_REFUSE_EAP)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_REFUSE_PAP))) {
        setRefusePap(setting.value(QLatin1String(NM_SETTING_PPP_REFUSE_PAP)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_REFUSE_CHAP))) {
        setRefuseChap(setting.value(QLatin1String(NM_SETTING_PPP_REFUSE_CHAP)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_REFUSE_MSCHAP))) {
        setRefuseMschap(setting.value(QLatin1String(NM_SETTING_PPP_REFUSE_MSCHAP)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_REFUSE_MSCHAPV2))) {
        setRefuseMschapv2(setting.value(QLatin1String(NM_SETTING_PPP_REFUSE_MSCHAPV2)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_NOBSDCOMP))) {
        setNoBsdComp(setting.value(QLatin1String(NM_SETTING_PPP_NOBSDCOMP)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_NODEFLATE))) {
        setNoDeflate(setting.value(QLatin1String(NM_SETTING_PPP_NODEFLATE)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_NO_VJ_COMP))) {
        setNoVjComp(setting.value(QLatin1String(NM_SETTING_PPP_NO_VJ_COMP)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_REQUIRE_MPPE))) {
        setRequireMppe(setting.value(QLatin1String(NM_SETTING_PPP_REQUIRE_MPPE)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_REQUIRE_MPPE_128))) {
        setRequireMppe128(setting.value(QLatin1String(NM_SETTING_PPP_REQUIRE_MPPE_128)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_MPPE_STATEFUL))) {
        setMppeStateful(setting.value(QLatin1String(NM_SETTING_PPP_MPPE_STATEFUL)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_CRTSCTS))) {
        setCRtsCts(setting.value(QLatin1String(NM_SETTING_PPP_CRTSCTS)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_BAUD))) {
        setBaud(setting.value(QLatin1String(NM_SETTING_PPP_BAUD)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_MRU))) {
        setMru(setting.value(QLatin1String(NM_SETTING_PPP_MRU)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_MTU))) {
        setMtu(setting.value(QLatin1String(NM_SETTING_PPP_MTU)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_LCP_ECHO_FAILURE))) {
        setLcpEchoFailure(setting.value(QLatin1String(NM_SETTING_PPP_LCP_ECHO_FAILURE)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_PPP_LCP_ECHO_INTERVAL))) {
        setLcpEchoInterval(setting.value(QLatin1String(NM_SETTING_PPP_LCP_ECHO_INTERVAL)).toUInt());
    }
}

QVariantMap NetworkManager::Settings::PppSetting::toMap() const
{
    QVariantMap setting;

    if (!noAuth()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_NOAUTH), noAuth());
    }

    if (refuseEap()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_REFUSE_EAP), refuseEap());
    }

    if (refusePap()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_REFUSE_PAP), refusePap());
    }

    if (refuseChap()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_REFUSE_CHAP), refuseChap());
    }

    if (refuseMschap()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_REFUSE_MSCHAP), refuseMschap());
    }

    if (refuseMschapv2()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_REFUSE_MSCHAPV2), refuseMschapv2());
    }

    if (noBsdComp()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_NOBSDCOMP), noBsdComp());
    }

    if (noDeflate()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_NODEFLATE), noDeflate());
    }

    if (noVjComp()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_NO_VJ_COMP), noVjComp());
    }

    if (requireMppe()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_REQUIRE_MPPE), requireMppe());
    }

    if (requireMppe128()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_REQUIRE_MPPE_128), requireMppe128());
    }

    if (mppeStateful()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_MPPE_STATEFUL), mppeStateful());
    }

    if (cRtsCts()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_CRTSCTS), cRtsCts());
    }

    if (baud()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_BAUD), baud());
    }

    if (mru()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_MRU), mru());
    }

    if (mtu()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_MTU), mtu());
    }

    if (lcpEchoFailure()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_LCP_ECHO_FAILURE), lcpEchoFailure());
    }

    if (lcpEchoInterval()) {
        setting.insert(QLatin1String(NM_SETTING_PPP_LCP_ECHO_INTERVAL), lcpEchoInterval());
    }

    return setting;
}

void NetworkManager::Settings::PppSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << NM_SETTING_PPP_NOAUTH << ": " << noAuth();
    qDebug() << NM_SETTING_PPP_REFUSE_EAP << ": " << refuseEap();
    qDebug() << NM_SETTING_PPP_REFUSE_PAP << ": " << refusePap();
    qDebug() << NM_SETTING_PPP_REFUSE_CHAP << ": " << refuseChap();
    qDebug() << NM_SETTING_PPP_REFUSE_MSCHAP << ": " << refuseMschap();
    qDebug() << NM_SETTING_PPP_REFUSE_MSCHAPV2 << ": " << refuseMschapv2();
    qDebug() << NM_SETTING_PPP_NOBSDCOMP << ": " << noBsdComp();
    qDebug() << NM_SETTING_PPP_NODEFLATE << ": " << noDeflate();
    qDebug() << NM_SETTING_PPP_NO_VJ_COMP << ": " << noVjComp();
    qDebug() << NM_SETTING_PPP_REQUIRE_MPPE << ": " << requireMppe();
    qDebug() << NM_SETTING_PPP_REQUIRE_MPPE_128 << ": " << requireMppe128();
    qDebug() << NM_SETTING_PPP_MPPE_STATEFUL << ": " << mppeStateful();
    qDebug() << NM_SETTING_PPP_CRTSCTS << ": " << cRtsCts();
    qDebug() << NM_SETTING_PPP_BAUD << ": " << baud();
    qDebug() << NM_SETTING_PPP_MRU << ": " << mru();
    qDebug() << NM_SETTING_PPP_MTU << ": " << mtu();
    qDebug() << NM_SETTING_PPP_LCP_ECHO_FAILURE << ": " << lcpEchoFailure();
    qDebug() << NM_SETTING_PPP_LCP_ECHO_INTERVAL << ": " << lcpEchoInterval();
}
