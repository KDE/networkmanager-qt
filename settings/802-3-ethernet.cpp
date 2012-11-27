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

#include "802-3-ethernet.h"
#include "802-3-ethernet_p.h"

NetworkManager::Settings::WiredSettingPrivate::WiredSettingPrivate():
    name("802-3-ethernet"),
    speed(0),
    port(NetworkManager::Settings::WiredSetting::Mii),
    clonedMacAddress(QByteArray()),
    macAddressBlacklist(QStringList()),
    mtu(0),
    s390Subchannels(QStringList()),
    s390NetType(NetworkManager::Settings::WiredSetting::Ctc),
    s390Options(QMap<QString, QString>())
{
}

NetworkManager::Settings::WiredSetting::WiredSetting():
    Setting(Setting::Wired),
    d_ptr(new WiredSettingPrivate())
{ }

NetworkManager::Settings::WiredSetting::WiredSetting(NetworkManager::Settings::WiredSetting *setting):
    Setting(setting),
    d_ptr(new WiredSettingPrivate())
{
    setPort(setting->port());
    setSpeed(setting->speed());
    setDuplexType(setting->duplexType());
    setAutoNegotiate(setting->autoNegotiate());
    setMacAddress(setting->macAddress());
    setClonedMacAddress(setting->clonedMacAddress());
    setMacAddressBlacklist(setting->macAddressBlacklist());
    setMtu(setting->mtu());
    setS390Subchannels(setting->s390Subchannels());
    setS390NetType(setting->s390NetType());
    setS390Options(setting->s390Options());
}

NetworkManager::Settings::WiredSetting::~WiredSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::WiredSetting::name() const
{
    Q_D(const WiredSetting);

    return d->name;
}

void NetworkManager::Settings::WiredSetting::setPort(const NetworkManager::Settings::WiredSetting::PortType port)
{
    Q_D(WiredSetting);

    d->port = port;
}

NetworkManager::Settings::WiredSetting::PortType NetworkManager::Settings::WiredSetting::port() const
{
    Q_D(const WiredSetting);

    return d->port;
}

void NetworkManager::Settings::WiredSetting::setSpeed(const quint32 speed)
{
    Q_D(WiredSetting);

    d->speed = speed;
}

quint32 NetworkManager::Settings::WiredSetting::speed() const
{
    Q_D(const WiredSetting);

    return d->speed;
}

void NetworkManager::Settings::WiredSetting::setDuplexType(const NetworkManager::Settings::WiredSetting::DuplexType type)
{
    Q_D(WiredSetting);

    d->duplex = type;
}

NetworkManager::Settings::WiredSetting::DuplexType NetworkManager::Settings::WiredSetting::duplexType() const
{
    Q_D(const WiredSetting);

    return d->duplex;
}

void NetworkManager::Settings::WiredSetting::setAutoNegotiate(const bool autoNegotiate)
{
    Q_D(WiredSetting);

    d->autoNegotiate = autoNegotiate;
}

bool NetworkManager::Settings::WiredSetting::autoNegotiate() const
{
    Q_D(const WiredSetting);

    return d->autoNegotiate;
}

void NetworkManager::Settings::WiredSetting::setMacAddress(const QByteArray& address)
{
    Q_D(WiredSetting);

    d->macAddress = address;
}

QByteArray NetworkManager::Settings::WiredSetting::macAddress() const
{
    Q_D(const WiredSetting);

    return d->macAddress;
}

void NetworkManager::Settings::WiredSetting::setClonedMacAddress(const QByteArray& address)
{
    Q_D(WiredSetting);

    d->clonedMacAddress = address;
}

QByteArray NetworkManager::Settings::WiredSetting::clonedMacAddress() const
{
    Q_D(const WiredSetting);

    return d->clonedMacAddress;
}

void NetworkManager::Settings::WiredSetting::setMacAddressBlacklist(const QStringList& list)
{
    Q_D(WiredSetting);

    d->macAddressBlacklist = list;
}

QStringList NetworkManager::Settings::WiredSetting::macAddressBlacklist() const
{
    Q_D(const WiredSetting);

    return d->macAddressBlacklist;
}

void NetworkManager::Settings::WiredSetting::setMtu(const quint32 mtu)
{
    Q_D(WiredSetting);

    d->mtu = mtu;
}

quint32 NetworkManager::Settings::WiredSetting::mtu() const
{
    Q_D(const WiredSetting);

    return d->mtu;
}

void NetworkManager::Settings::WiredSetting::setS390Subchannels(const QStringList& channels)
{
    Q_D(WiredSetting);

    d->s390Subchannels = channels;
}

QStringList NetworkManager::Settings::WiredSetting::s390Subchannels() const
{
    Q_D(const WiredSetting);

    return d->s390Subchannels;
}

void NetworkManager::Settings::WiredSetting::setS390NetType(const NetworkManager::Settings::WiredSetting::S390Nettype type)
{
    Q_D(WiredSetting);

    d->s390NetType = type;
}

NetworkManager::Settings::WiredSetting::S390Nettype NetworkManager::Settings::WiredSetting::s390NetType() const
{
    Q_D(const WiredSetting);

    return d->s390NetType;
}

void NetworkManager::Settings::WiredSetting::setS390Options(const QMap< QString, QString > & options)
{
    Q_D(WiredSetting);

    d->s390Options = options;
}

QMap< QString, QString > NetworkManager::Settings::WiredSetting::s390Options() const
{
    Q_D(const WiredSetting);

    return d->s390Options;
}

void NetworkManager::Settings::WiredSetting::fromMap(const QVariantMap& setting)
{
    NetworkManager::Settings::Setting::fromMap(setting);
}
