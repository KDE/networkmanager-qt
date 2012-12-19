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

#include <QtNetworkManager/generic-types.h>

#include <nm-setting-wired.h>

#include <QDebug>

NetworkManager::Settings::WiredSettingPrivate::WiredSettingPrivate():
    name("802-3-ethernet"),
    port(NetworkManager::Settings::WiredSetting::Mii),
    speed(0),
    duplex(NetworkManager::Settings::WiredSetting::Full),
    autoNegotiate(true),
    macAddress(QByteArray()),
    clonedMacAddress(QByteArray()),
    macAddressBlacklist(QStringList()),
    mtu(0),
    s390Subchannels(QStringList()),
    s390NetType(NetworkManager::Settings::WiredSetting::Undefined),
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

void NetworkManager::Settings::WiredSetting::setPort(NetworkManager::Settings::WiredSetting::PortType port)
{
    Q_D(WiredSetting);

    d->port = port;
}

NetworkManager::Settings::WiredSetting::PortType NetworkManager::Settings::WiredSetting::port() const
{
    Q_D(const WiredSetting);

    return d->port;
}

void NetworkManager::Settings::WiredSetting::setSpeed(quint32 speed)
{
    Q_D(WiredSetting);

    d->speed = speed;
}

quint32 NetworkManager::Settings::WiredSetting::speed() const
{
    Q_D(const WiredSetting);

    return d->speed;
}

void NetworkManager::Settings::WiredSetting::setDuplexType(NetworkManager::Settings::WiredSetting::DuplexType type)
{
    Q_D(WiredSetting);

    d->duplex = type;
}

NetworkManager::Settings::WiredSetting::DuplexType NetworkManager::Settings::WiredSetting::duplexType() const
{
    Q_D(const WiredSetting);

    return d->duplex;
}

void NetworkManager::Settings::WiredSetting::setAutoNegotiate(bool autoNegotiate)
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

void NetworkManager::Settings::WiredSetting::setMtu(quint32 mtu)
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

void NetworkManager::Settings::WiredSetting::setS390NetType(NetworkManager::Settings::WiredSetting::S390Nettype type)
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
    if (setting.contains(QLatin1String(NM_SETTING_WIRED_PORT))) {
	QString port = setting.value(QLatin1String(NM_SETTING_WIRED_PORT)).toString();

	if (port == "tp") {
	    setPort(Tp);
	} else if (port == "aui") {
	    setPort(Aui);
	} else if (port == "bnc") {
	    setPort(Bnc);
	} else if (port == "mii") {
	    setPort(Mii);
	}
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIRED_SPEED))) {
	setSpeed(setting.value(QLatin1String(NM_SETTING_WIRED_SPEED)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIRED_DUPLEX))) {
	QString duplex = setting.value(QLatin1String(NM_SETTING_WIRED_DUPLEX)).toString();

	if (duplex == "half") {
	    setDuplexType(Half);
	} else if (duplex == "full") {
            setDuplexType(Full);
	}
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIRED_AUTO_NEGOTIATE))) {
	setAutoNegotiate(setting.value(QLatin1String(NM_SETTING_WIRED_AUTO_NEGOTIATE)).toBool());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIRED_MAC_ADDRESS))) {
	setMacAddress(setting.value(QLatin1String(NM_SETTING_WIRED_MAC_ADDRESS)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIRED_CLONED_MAC_ADDRESS))) {
	setClonedMacAddress(setting.value(QLatin1String(NM_SETTING_WIRED_CLONED_MAC_ADDRESS)).toByteArray());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIRED_MAC_ADDRESS_BLACKLIST))) {
	setMacAddressBlacklist(setting.value(QLatin1String(NM_SETTING_WIRED_MAC_ADDRESS_BLACKLIST)).toStringList());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIRED_MTU))) {
	setMtu(setting.value(QLatin1String(NM_SETTING_WIRED_MTU)).toUInt());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIRED_S390_SUBCHANNELS))) {
	setS390Subchannels(setting.value(QLatin1String(NM_SETTING_WIRED_S390_SUBCHANNELS)).toStringList());
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIRED_S390_NETTYPE))) {
	QString nettype = setting.value(QLatin1String(NM_SETTING_WIRED_S390_NETTYPE)).toString();

	if (nettype == "qeth") {
	    setS390NetType(Qeth);
	} else if (nettype == "lcs") {
	    setS390NetType(Lcs);
	} else if (nettype == "ctc") {
	    setS390NetType(Ctc);
	}
    }

    if (setting.contains(QLatin1String(NM_SETTING_WIRED_S390_OPTIONS))) {
        QMap<QString, QString> tmp;
	if (setting.value(QLatin1String(NM_SETTING_WIRED_S390_OPTIONS)).canConvert<QDBusArgument>()) {
            QDBusArgument arg = setting.value(QLatin1String(NM_SETTING_WIRED_S390_OPTIONS)).value<QDBusArgument>();
            tmp = qdbus_cast<QStringMap>(arg);
        } else {
            tmp = setting.value(QLatin1String(NM_SETTING_WIRED_S390_OPTIONS)).value<QStringMap>();
        }
        setS390Options(tmp);
    }
}

QVariantMap NetworkManager::Settings::WiredSetting::toMap() const
{
    QVariantMap setting;

    switch (port()) {
        case Tp:
            setting.insert(QLatin1String(NM_SETTING_WIRED_PORT), "tp");
            break;
        case Aui:
            setting.insert(QLatin1String(NM_SETTING_WIRED_PORT), "aui");
            break;
        case Bnc:
            setting.insert(QLatin1String(NM_SETTING_WIRED_PORT), "bnc");
            break;
        case Mii:
            setting.insert(QLatin1String(NM_SETTING_WIRED_PORT), "mii");
            break;
    }

    if (speed()) {
        setting.insert(QLatin1String(NM_SETTING_WIRED_SPEED), speed());
    }

    switch (duplexType()) {
        case Half:
            setting.insert(QLatin1String(NM_SETTING_WIRED_DUPLEX), "half");
            break;
        case Full:
            setting.insert(QLatin1String(NM_SETTING_WIRED_DUPLEX), "full");
            break;
    }

    if (!autoNegotiate()) {
        setting.insert(QLatin1String(NM_SETTING_WIRED_AUTO_NEGOTIATE), autoNegotiate());
    }

    if (!macAddress().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_WIRED_MAC_ADDRESS), macAddress());
    }
    if (!clonedMacAddress().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_WIRED_CLONED_MAC_ADDRESS), clonedMacAddress());
    }
    if (!macAddressBlacklist().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_WIRED_MAC_ADDRESS_BLACKLIST), macAddressBlacklist());
    }
    if (mtu()) {
        setting.insert(QLatin1String(NM_SETTING_WIRED_MTU), mtu());
    }
    if (!s390Subchannels().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_WIRED_S390_SUBCHANNELS), s390Subchannels());
    }

    switch (s390NetType()) {
        case Qeth:
            setting.insert(QLatin1String(NM_SETTING_WIRED_S390_NETTYPE), "qeth");
            break;
        case Lcs:
            setting.insert(QLatin1String(NM_SETTING_WIRED_S390_NETTYPE), "lcs");
            break;
        case Ctc:
            setting.insert(QLatin1String(NM_SETTING_WIRED_S390_NETTYPE), "ctc");
            break;
    }

    if (!s390Options().isEmpty()) {
        setting.insert(QLatin1String(NM_SETTING_WIRED_S390_OPTIONS), QVariant::fromValue(s390Options()));
    }

    return setting;
}

void NetworkManager::Settings::WiredSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    qDebug() << "PORT - " << port();
    qDebug() << "SPEED - " << speed();
    qDebug() << "DUPLEX TYPE - " << duplexType();
    qDebug() << "AUTO NEGOTIATE - " << autoNegotiate();
    qDebug() << "MAC ADDRESS - " << macAddress();
    qDebug() << "CLONED MAC ADDRESS - " << clonedMacAddress();
    qDebug() << "MAC ADDRES BLACKLIST - " << macAddressBlacklist();
    qDebug() << "MTU - " << mtu();
    qDebug() << "S390 SUBCHANNELS - " << s390Subchannels();
    qDebug() << "S390 TYPE - " << s390NetType();
    qDebug() << "S390 OPTIONS - " << s390Options();
}

