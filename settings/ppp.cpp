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

#include "ppp.h"
#include "ppp_p.h"

#include <nm-setting-ppp.h>

#include <QDebug>

NetworkManager::Settings::PPPSettingPrivate::PPPSettingPrivate():
    name(QString("ppp")),
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

NetworkManager::Settings::PPPSetting::PPPSetting():
    Setting(Setting::Ppp),
    d_ptr(new PPPSettingPrivate())
{ }

NetworkManager::Settings::PPPSetting::PPPSetting(PPPSetting * setting):
    Setting(setting),
    d_ptr(new PPPSettingPrivate())
{
    setNoAuth(setting->noAuth());
    setRefuseEap(setting->refuseEap());
    setRefusePap(setting->refusePap());
    setRefuseChap(setting->refuseChap());
    setRefuseMschap(setting->refuseMschap());
    setRefuseMschapv2(setting->refuseMschapv2());
    setNoBsdComp(setting->noBsdComp());
    setNoDeflate(setting->noDeflate());
    setNoVjComp(setting->noVjComp());
    setRequireMppe(setting->requireMppe());
    setRequireMppe128(setting->requireMppe128());
    setMppeStateful(setting->mppeStateful());
    setCRtsCts(setting->cRtsCts());
    setBaud(setting->baud());
    setMru(setting->mru());
    setMtu(setting->mtu());
    setLcpEchoFailure(setting->lcpEchoFailure());
    setLcpEchoInterval(setting->lcpEchoInterval());
}

NetworkManager::Settings::PPPSetting::~PPPSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::PPPSetting::name() const
{
    Q_D(const PPPSetting);

    return d->name;
}

void NetworkManager::Settings::PPPSetting::setNoAuth(bool require)
{
    Q_D(PPPSetting);

    d->noauth = require;
}

bool NetworkManager::Settings::PPPSetting::noAuth() const
{
    Q_D(const PPPSetting);

    return d->noauth;
}

void NetworkManager::Settings::PPPSetting::setRefuseEap(bool refuse)
{
    Q_D(PPPSetting);

    d->refuseEap = refuse;
}

bool NetworkManager::Settings::PPPSetting::refuseEap() const
{
    Q_D(const PPPSetting);

    return d->refuseEap;
}

void NetworkManager::Settings::PPPSetting::setRefusePap(bool refuse)
{
    Q_D(PPPSetting);

    d->refusePap = refuse;
}

bool NetworkManager::Settings::PPPSetting::refusePap() const
{
    Q_D(const PPPSetting);

    return d->refusePap;
}

void NetworkManager::Settings::PPPSetting::setRefuseChap(bool refuse)
{
    Q_D(PPPSetting);

    d->refuseChap = refuse;
}

bool NetworkManager::Settings::PPPSetting::refuseChap() const
{
    Q_D(const PPPSetting);

    return d->refuseChap;
}

void NetworkManager::Settings::PPPSetting::setRefuseMschap(bool refuse)
{
    Q_D(PPPSetting);

    d->refuseMschap = refuse;
}

bool NetworkManager::Settings::PPPSetting::refuseMschap() const
{
    Q_D(const PPPSetting);

    return d->refuseMschap;
}

void NetworkManager::Settings::PPPSetting::setRefuseMschapv2(bool refuse)
{
    Q_D(PPPSetting);

    d->refuseMschapv2 = refuse;
}

bool NetworkManager::Settings::PPPSetting::refuseMschapv2() const
{
    Q_D(const PPPSetting);

    return d->refuseMschapv2;
}

void NetworkManager::Settings::PPPSetting::setNoBsdComp(bool require)
{
    Q_D(PPPSetting);

    d->nobsdcomp = require;
}

bool NetworkManager::Settings::PPPSetting::noBsdComp() const
{
    Q_D(const PPPSetting);

    return d->nobsdcomp;
}

void NetworkManager::Settings::PPPSetting::setNoDeflate(bool require)
{
    Q_D(PPPSetting);

    d->nodeflate = require;
}

bool NetworkManager::Settings::PPPSetting::noDeflate() const
{
    Q_D(const PPPSetting);

    return d->nodeflate;
}

void NetworkManager::Settings::PPPSetting::setNoVjComp(bool require)
{
    Q_D(PPPSetting);

    d->noVjComp = require;
}

bool NetworkManager::Settings::PPPSetting::noVjComp() const
{
    Q_D(const PPPSetting);

    return d->noVjComp;
}

void NetworkManager::Settings::PPPSetting::setRequireMppe(bool require)
{
    Q_D(PPPSetting);

    d->requireMppe = require;
}

bool NetworkManager::Settings::PPPSetting::requireMppe() const
{
    Q_D(const PPPSetting);

    return d->requireMppe;
}

void NetworkManager::Settings::PPPSetting::setRequireMppe128(bool require)
{
    Q_D(PPPSetting);

    d->requireMppe128 = require;
}

bool NetworkManager::Settings::PPPSetting::requireMppe128() const
{
    Q_D(const PPPSetting);

    return d->requireMppe128;
}

void NetworkManager::Settings::PPPSetting::setMppeStateful(bool used)
{
    Q_D(PPPSetting);

    d->mppeStateful = used;
}

bool NetworkManager::Settings::PPPSetting::mppeStateful() const
{
    Q_D(const PPPSetting);

    return d->mppeStateful;
}

void NetworkManager::Settings::PPPSetting::setCRtsCts(bool control)
{
    Q_D(PPPSetting);

    d->crtscts = control;
}

bool NetworkManager::Settings::PPPSetting::cRtsCts() const
{
    Q_D(const PPPSetting);

    return d->crtscts;
}

void NetworkManager::Settings::PPPSetting::setBaud(quint32 baud)
{
    Q_D(PPPSetting);

    d->baud = baud;
}

quint32 NetworkManager::Settings::PPPSetting::baud() const
{
    Q_D(const PPPSetting);

    return d->baud;
}

void NetworkManager::Settings::PPPSetting::setMru(quint32 mru)
{
    Q_D(PPPSetting);

    d->mru = mru;
}

quint32 NetworkManager::Settings::PPPSetting::mru() const
{
    Q_D(const PPPSetting);

    return d->mru;
}

void NetworkManager::Settings::PPPSetting::setMtu(quint32 mtu)
{
    Q_D(PPPSetting);

    d->mtu = mtu;
}

quint32 NetworkManager::Settings::PPPSetting::mtu() const
{
    Q_D(const PPPSetting);

    return d->mtu;
}

void NetworkManager::Settings::PPPSetting::setLcpEchoFailure(quint32 number)
{
    Q_D(PPPSetting);

    d->lcpEchoFailure = number;
}

quint32 NetworkManager::Settings::PPPSetting::lcpEchoFailure() const
{
    Q_D(const PPPSetting);

    return d->lcpEchoFailure;
}

void NetworkManager::Settings::PPPSetting::setLcpEchoInterval(quint32 interval)
{
    Q_D(PPPSetting);

    d->lcpEchoInterval = interval;
}

quint32 NetworkManager::Settings::PPPSetting::lcpEchoInterval() const
{
    Q_D(const PPPSetting);

    return d->lcpEchoInterval;
}

bool NetworkManager::Settings::PPPSetting::hasSecrets() const
{
    return false;
}

void NetworkManager::Settings::PPPSetting::fromMap(const QVariantMap& setting)
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

QVariantMap NetworkManager::Settings::PPPSetting::toMap() const
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

void NetworkManager::Settings::PPPSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << "NO AUTH - " << noAuth();
    qDebug() << "REFUSE EAP - " << refuseEap();
    qDebug() << "REFUSE PAP - " << refusePap();
    qDebug() << "REFUSE CHAP - " << refuseChap();
    qDebug() << "REFUSE MSCHAP - " << refuseMschap();
    qDebug() << "REFUSE MSCHAP2 - " << refuseMschapv2();
    qDebug() << "NO BSD COMPRESSION - " << noBsdComp();
    qDebug() << "NO DEFLATE COMPRESSION - " << noDeflate();
    qDebug() << "NO VJ COMPRESSION - " << noVjComp();
    qDebug() << "REQUIRE MPPE - " << requireMppe();
    qDebug() << "REQUIRE MPPE128 - " << requireMppe128();
    qDebug() << "MPPE STATEFUL - " << mppeStateful();
    qDebug() << "CONTROL RTS AND CTS - " << cRtsCts();
    qDebug() << "BAUDRATE - " << baud();
    qDebug() << "MRU - " << mru();
    qDebug() << "MTU - " << mtu();
    qDebug() << "LCP ECHO FAILURE - " << lcpEchoFailure();
    qDebug() << "LCP ECHO INTERVAL - " << lcpEchoInterval();
}
