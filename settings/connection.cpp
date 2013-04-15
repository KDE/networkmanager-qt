/*
    Copyright 2012-2013  Jan Grulich <jgrulich@redhat.com>
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


#include "connection.h"
#include "connection_p.h"

#include "802-3-ethernet.h"
#include "802-11-olpc-mesh.h"
#include "802-11-wireless.h"
#include "802-11-wireless-security.h"
#include "802-1x.h"
#include "adsl.h"
#include "bluetooth.h"
#include "bond.h"
#include "bridge.h"
#include "bridgeport.h"
#include "gsm.h"
#include "cdma.h"
#include "infiniband.h"
#include "ipv4.h"
#include "ipv6.h"
#include "ppp.h"
#include "pppoe.h"
#include "serial.h"
#include "vlan.h"
#include "vpn.h"
#include "wimax.h"

#include <nm-setting-adsl.h>
#include <nm-setting-bond.h>
#include <nm-setting-bluetooth.h>
#include <nm-setting-bridge.h>
#include <nm-setting-cdma.h>
#include <nm-setting-connection.h>
#include <nm-setting-gsm.h>
#include <nm-setting-infiniband.h>
#include <nm-setting-olpc-mesh.h>
#include <nm-setting-pppoe.h>
#include <nm-setting-vlan.h>
#include <nm-setting-vpn.h>
#include <nm-setting-wimax.h>
#include <nm-setting-wired.h>
#include <nm-setting-wireless.h>


#include <QtCore/QUuid>

NetworkManager::Settings::ConnectionSettingsPrivate::ConnectionSettingsPrivate():
    name(QString("connection")),
    uuid(QUuid().toString()),
    type(ConnectionSettings::Wired),
    autoconnect(true),
    readOnly(false)
{ }

NetworkManager::Settings::ConnectionSettings::ConnectionType NetworkManager::Settings::ConnectionSettings::typeFromString(const QString & typeString)
{
    ConnectionSettings::ConnectionType type = Wired;

    if (typeString == QLatin1String(NM_SETTING_ADSL_SETTING_NAME)) {
        type = Adsl;
    } else if (typeString == QLatin1String(NM_SETTING_BLUETOOTH_SETTING_NAME)) {
        type = Bluetooth;
    } else if (typeString == QLatin1String(NM_SETTING_BOND_SETTING_NAME)) {
        type = Bond;
    } else if (typeString == QLatin1String(NM_SETTING_BRIDGE_SETTING_NAME)) {
        type = Bridge;
    } else if (typeString == QLatin1String(NM_SETTING_CDMA_SETTING_NAME)) {
        type = Cdma;
    } else if (typeString == QLatin1String(NM_SETTING_GSM_SETTING_NAME)) {
        type = Gsm;
    } else if (typeString == QLatin1String(NM_SETTING_INFINIBAND_SETTING_NAME)) {
        type = Infiniband;
    } else if (typeString == QLatin1String(NM_SETTING_OLPC_MESH_SETTING_NAME)) {
        type = OLPCMesh;
    } else if (typeString == QLatin1String(NM_SETTING_PPPOE_SETTING_NAME)) {
        type = Pppoe;
    } else if (typeString == QLatin1String(NM_SETTING_VLAN_SETTING_NAME)) {
        type = Vlan;
    } else if (typeString == QLatin1String(NM_SETTING_VPN_SETTING_NAME)) {
        type = Vpn;
    } else if (typeString == QLatin1String(NM_SETTING_WIMAX_SETTING_NAME)) {
        type = Wimax;
    } else if (typeString == QLatin1String(NM_SETTING_WIRED_SETTING_NAME)) {
        type = Wired;
    } else if (typeString == QLatin1String(NM_SETTING_WIRELESS_SETTING_NAME)) {
        type = Wireless;
    }

    return type;
}

QString NetworkManager::Settings::ConnectionSettings::typeAsString(NetworkManager::Settings::ConnectionSettings::ConnectionType type)
{
    QString typeString;

    switch (type) {
    case Adsl:
        typeString = QLatin1String(NM_SETTING_ADSL_SETTING_NAME);
        break;
    case Bond:
        typeString = QLatin1String(NM_SETTING_BOND_SETTING_NAME);
        break;
    case Bluetooth:
        typeString = QLatin1String(NM_SETTING_BLUETOOTH_SETTING_NAME);
        break;
    case Bridge:
        typeString = QLatin1String(NM_SETTING_BRIDGE_SETTING_NAME);
        break;
    case Cdma:
        typeString = QLatin1String(NM_SETTING_CDMA_SETTING_NAME);
        break;
    case Gsm:
        typeString = QLatin1String(NM_SETTING_GSM_SETTING_NAME);
        break;
    case Infiniband:
        typeString = QLatin1String(NM_SETTING_INFINIBAND_SETTING_NAME);
        break;
    case OLPCMesh:
        typeString = QLatin1String(NM_SETTING_OLPC_MESH_SETTING_NAME);
        break;
    case Pppoe:
        typeString = QLatin1String(NM_SETTING_PPPOE_SETTING_NAME);
        break;
    case Vlan:
        typeString = QLatin1String(NM_SETTING_VLAN_SETTING_NAME);
        break;
    case Vpn:
        typeString = QLatin1String(NM_SETTING_VPN_SETTING_NAME);
        break;
    case Wimax:
        typeString = QLatin1String(NM_SETTING_WIRED_SETTING_NAME);
        break;
    case Wired:
        typeString = QLatin1String(NM_SETTING_WIRED_SETTING_NAME);
        break;
    case Wireless:
        typeString = QLatin1String(NM_SETTING_WIRELESS_SETTING_NAME);
        break;
    default:
        break;
    };

    return typeString;
}

QString NetworkManager::Settings::ConnectionSettings::createNewUuid()
{
    return QUuid::createUuid().toString().mid(1, QUuid::createUuid().toString().length() - 2);
}

NetworkManager::Settings::ConnectionSettings::ConnectionSettings():
    d_ptr(new ConnectionSettingsPrivate())
{ }

NetworkManager::Settings::ConnectionSettings::ConnectionSettings(NetworkManager::Settings::ConnectionSettings::ConnectionType type, NMBluetoothCapabilities bt_cap):
    d_ptr(new ConnectionSettingsPrivate())
{
    setConnectionType(type, bt_cap);
}

NetworkManager::Settings::ConnectionSettings::ConnectionSettings(const NetworkManager::Settings::ConnectionSettings::Ptr &other):
    d_ptr(new ConnectionSettingsPrivate())
{
    setId(other->id());
    setUuid(other->uuid());
    setConnectionType(other->connectionType());
    setPermissions(other->permissions());
    setAutoconnect(other->autoconnect());
    setTimestamp(other->timestamp());
    setReadOnly(other->readOnly());
    setZone(other->zone());
    setMaster(other->master());
    setSlaveType(other->slaveType());

    initSettings(other);
}

NetworkManager::Settings::ConnectionSettings::ConnectionSettings(const NMVariantMapMap &map) :
    d_ptr(new ConnectionSettingsPrivate())
{
    fromMap(map);
}

NetworkManager::Settings::ConnectionSettings::~ConnectionSettings()
{
    clearSettings();

    delete d_ptr;
}

void NetworkManager::Settings::ConnectionSettings::initSettings(NMBluetoothCapabilities bt_cap)
{
    clearSettings();

    switch (connectionType()) {
    case Adsl:
        addSetting(Setting::Ptr(new AdslSetting()));
        addSetting(Setting::Ptr(new Ipv4Setting()));
        addSetting(Setting::Ptr(new Ipv6Setting()));
        break;
    case Bond:
        addSetting(Setting::Ptr(new BondSetting()));
        addSetting(Setting::Ptr(new Ipv4Setting()));
        addSetting(Setting::Ptr(new Ipv6Setting()));
        break;
    case Bluetooth:
        addSetting(Setting::Ptr(new BluetoothSetting()));
        addSetting(Setting::Ptr(new Ipv4Setting()));
        //addSetting(Setting::Ptr(new Ipv6Setting()));
        if (bt_cap == NM_BT_CAPABILITY_DUN) {
            addSetting(Setting::Ptr(new GsmSetting()));
            addSetting(Setting::Ptr(new PppSetting()));
            addSetting(Setting::Ptr(new SerialSetting()));
        }
        break;
    case Bridge:
        addSetting(Setting::Ptr(new BridgeSetting()));
        addSetting(Setting::Ptr(new Ipv4Setting()));
        addSetting(Setting::Ptr(new Ipv6Setting()));
        break;
    case Cdma:
        addSetting(Setting::Ptr(new CdmaSetting()));
        addSetting(Setting::Ptr(new Ipv4Setting()));
        //addSetting(Setting::Ptr(new Ipv6Setting()));
        addSetting(Setting::Ptr(new PppSetting()));
        break;
    case Gsm:
        addSetting(Setting::Ptr(new GsmSetting()));
        addSetting(Setting::Ptr(new Ipv4Setting()));
        //addSetting(Setting::Ptr(new Ipv6Setting()));
        addSetting(Setting::Ptr(new PppSetting()));
        break;
    case Infiniband:
        addSetting(Setting::Ptr(new InfinibandSetting()));
        addSetting(Setting::Ptr(new Ipv4Setting()));
        addSetting(Setting::Ptr(new Ipv6Setting()));
        break;
    case OLPCMesh:
        addSetting(Setting::Ptr(new Ipv4Setting()));
        addSetting(Setting::Ptr(new Ipv6Setting()));
        addSetting(Setting::Ptr(new OlpcMeshSetting()));
        break;
    case Pppoe:
        addSetting(Setting::Ptr(new Ipv4Setting()));
        //addSetting(new Ipv6Setting()));
        addSetting(Setting::Ptr(new PppSetting()));
        addSetting(Setting::Ptr(new PppoeSetting()));
        addSetting(Setting::Ptr(new WiredSetting()));
        break;
    case Vlan:
        addSetting(Setting::Ptr(new Ipv4Setting()));
        addSetting(Setting::Ptr(new Ipv6Setting()));
        addSetting(Setting::Ptr(new VlanSetting()));
        break;
    case Vpn:
        addSetting(Setting::Ptr(new Ipv4Setting()));
        //addSetting(Setting::Ptr(new Ipv6Setting()));
        addSetting(Setting::Ptr(new VpnSetting()));
        break;
    case Wimax:
        addSetting(Setting::Ptr(new Ipv4Setting()));
        addSetting(Setting::Ptr(new Ipv6Setting()));
        addSetting(Setting::Ptr(new WimaxSetting()));
        break;
    case Wired:
        addSetting(Setting::Ptr(new Ipv4Setting()));
        addSetting(Setting::Ptr(new Ipv6Setting()));
        addSetting(Setting::Ptr(new Security8021xSetting()));
        addSetting(Setting::Ptr(new WiredSetting()));
        break;
    case Wireless:
        addSetting(Setting::Ptr(new Ipv4Setting()));
        addSetting(Setting::Ptr(new Ipv6Setting()));
        addSetting(Setting::Ptr(new Security8021xSetting()));
        addSetting(Setting::Ptr(new WirelessSetting()));
        addSetting(Setting::Ptr(new WirelessSecuritySetting()));
        break;
    case Unknown:
        break;
    default:
        break;
    }
}

void NetworkManager::Settings::ConnectionSettings::initSettings(const NetworkManager::Settings::ConnectionSettings::Ptr &connectionSettings)
{
    clearSettings();

    switch (connectionType()) {
    case Adsl:
        addSetting(connectionSettings->setting(Setting::Adsl));
        addSetting(connectionSettings->setting(Setting::Ipv4));
        addSetting(connectionSettings->setting(Setting::Ipv6));
        break;
    case Bond:
        addSetting(connectionSettings->setting(Setting::Bond));
        addSetting(connectionSettings->setting(Setting::Ipv4));
        addSetting(connectionSettings->setting(Setting::Ipv6));
        break;
    case Bluetooth:
        addSetting(connectionSettings->setting(Setting::Bluetooth));
        addSetting(connectionSettings->setting(Setting::Ipv4));
        //addSetting(connectionSettings->setting(Setting::Ipv6));
        if (setting(Setting::Gsm) && setting(Setting::Ppp) && setting(Setting::Serial)) {
            addSetting(connectionSettings->setting(Setting::Gsm));
            addSetting(connectionSettings->setting(Setting::Ppp));
            addSetting(connectionSettings->setting(Setting::Serial));
        }
        break;
    case Bridge:
        addSetting(connectionSettings->setting(Setting::Bridge));
        addSetting(connectionSettings->setting(Setting::Ipv4));
        addSetting(connectionSettings->setting(Setting::Ipv6));
        break;
    case Cdma:
        addSetting(connectionSettings->setting(Setting::Cdma));
        addSetting(connectionSettings->setting(Setting::Ipv4));
        addSetting(connectionSettings->setting(Setting::Ipv6));
        addSetting(connectionSettings->setting(Setting::Ppp));
        break;
    case Gsm:
        addSetting(connectionSettings->setting(Setting::Gsm));
        addSetting(connectionSettings->setting(Setting::Ipv4));
        addSetting(connectionSettings->setting(Setting::Ipv6));
        addSetting(connectionSettings->setting(Setting::Ppp));
        break;
    case Infiniband:
        addSetting(connectionSettings->setting(Setting::Infiniband));
        addSetting(connectionSettings->setting(Setting::Ipv4));
        addSetting(connectionSettings->setting(Setting::Ipv6));
        break;
    case OLPCMesh:
        addSetting(connectionSettings->setting(Setting::Ipv4));
        addSetting(connectionSettings->setting(Setting::Ipv6));
        addSetting(connectionSettings->setting(Setting::OlpcMesh));
        break;
    case Pppoe:
        addSetting(connectionSettings->setting(Setting::Ipv4));
        //addSetting(Ipv6Setting(connectionSettings->setting(Setting::Ipv6));
        addSetting(connectionSettings->setting(Setting::Ppp));
        addSetting(connectionSettings->setting(Setting::Pppoe));
        addSetting(connectionSettings->setting(Setting::Wired));
        break;
    case Vlan:
        addSetting(connectionSettings->setting(Setting::Ipv4));
        addSetting(connectionSettings->setting(Setting::Ipv6));
        addSetting(connectionSettings->setting(Setting::Vlan));
        break;
    case Vpn:
        addSetting(connectionSettings->setting(Setting::Ipv4));
        addSetting(connectionSettings->setting(Setting::Ipv6));
        addSetting(connectionSettings->setting(Setting::Vpn));
        break;
    case Wimax:
        addSetting(connectionSettings->setting(Setting::Ipv4));
        addSetting(connectionSettings->setting(Setting::Ipv6));
        addSetting(connectionSettings->setting(Setting::Wimax));
        break;
    case Wired:
        addSetting(connectionSettings->setting(Setting::Ipv4));
        addSetting(connectionSettings->setting(Setting::Ipv6));
        addSetting(connectionSettings->setting(Setting::Security8021x));
        addSetting(connectionSettings->setting(Setting::Wired));
        break;
    case Wireless:
        addSetting(connectionSettings->setting(Setting::Ipv4));
        addSetting(connectionSettings->setting(Setting::Ipv6));
        addSetting(connectionSettings->setting(Setting::Security8021x));
        addSetting(connectionSettings->setting(Setting::Wireless));
        addSetting(connectionSettings->setting(Setting::WirelessSecurity));
        break;
    case Unknown:
        break;
    default:
        break;
    }
}

void NetworkManager::Settings::ConnectionSettings::fromMap(const NMVariantMapMap& map)
{
    QVariantMap connectionSettings = map.value(QLatin1String(NM_SETTING_CONNECTION_SETTING_NAME));

    setId(connectionSettings.value(QLatin1String(NM_SETTING_CONNECTION_ID)).toString());
    setUuid(connectionSettings.value(QLatin1String(NM_SETTING_CONNECTION_UUID)).toString());
    setConnectionType(typeFromString(connectionSettings.value(QLatin1String(NM_SETTING_CONNECTION_TYPE)).toString()));

    if (connectionSettings.contains(QLatin1String(NM_SETTING_CONNECTION_PERMISSIONS))) {
        QStringList permissions = connectionSettings.value(QLatin1String(NM_SETTING_CONNECTION_PERMISSIONS)).toStringList();
        foreach(const QString & permission, permissions) {
            QStringList split = permission.split(QLatin1String(":"), QString::KeepEmptyParts);
            addToPermissions(split.at(1), split.at(2));
        }
    }

    if (connectionSettings.contains(QLatin1String(NM_SETTING_CONNECTION_AUTOCONNECT))) {
        setAutoconnect(connectionSettings.value(QLatin1String(NM_SETTING_CONNECTION_AUTOCONNECT)).toBool());
    }

    if (connectionSettings.contains(QLatin1String(NM_SETTING_CONNECTION_TIMESTAMP))) {
        int timestamp = connectionSettings.value(QLatin1String(NM_SETTING_CONNECTION_TIMESTAMP)).toInt();
        QDateTime dateTime;
        dateTime.setTime_t(timestamp);
        setTimestamp(dateTime);
    }

    if (connectionSettings.contains(QLatin1String(NM_SETTING_CONNECTION_READ_ONLY))) {
        setReadOnly(connectionSettings.value(QLatin1String(NM_SETTING_CONNECTION_READ_ONLY)).toBool());
    }

    if (connectionSettings.contains(QLatin1String(NM_SETTING_CONNECTION_ZONE))) {
        setZone(connectionSettings.value(QLatin1String(NM_SETTING_CONNECTION_ZONE)).toString());
    }

    if (connectionSettings.contains(QLatin1String(NM_SETTING_CONNECTION_MASTER))) {
        setMaster(connectionSettings.value(QLatin1String(NM_SETTING_CONNECTION_MASTER)).toString());
    }

    if (connectionSettings.contains(QLatin1String(NM_SETTING_CONNECTION_SLAVE_TYPE))) {
        setSlaveType(connectionSettings.value(QLatin1String(NM_SETTING_CONNECTION_SLAVE_TYPE)).toString());
    }

    if (connectionSettings.contains(QLatin1String(NM_SETTING_CONNECTION_SECONDARIES))) {
        setSecondaries(connectionSettings.value(QLatin1String(NM_SETTING_CONNECTION_SECONDARIES)).toStringList());
    }

    foreach(const Setting::Ptr &setting, settings()) {
        if (map.contains(setting->name())) {
            setting->fromMap(map.value(setting->name()));
            setting->setInitialized(true);
        } else {
            setting->setInitialized(false);
        }
    }
}

NMVariantMapMap NetworkManager::Settings::ConnectionSettings::toMap() const
{
    NMVariantMapMap result;
    QVariantMap connectionSetting;

    if (!id().isEmpty()) {
        connectionSetting.insert(QLatin1String(NM_SETTING_CONNECTION_ID), id());
    }

    if (!uuid().isEmpty()) {
        connectionSetting.insert(QLatin1String(NM_SETTING_CONNECTION_UUID), uuid());
    }

    if (connectionType()) {
        connectionSetting.insert(QLatin1String(NM_SETTING_CONNECTION_TYPE), typeAsString(connectionType()));
    }

    if (!permissions().isEmpty()) {
        QStringList perm;
        foreach(const QString & key, permissions().keys()) {
            QString tmp = "user:" + key + ':' + permissions().value(key);
            perm << tmp;
        }

        connectionSetting.insert(QLatin1String(NM_SETTING_CONNECTION_PERMISSIONS), perm);
    }

    if (!autoconnect()) {
        connectionSetting.insert(QLatin1String(NM_SETTING_CONNECTION_AUTOCONNECT), autoconnect());
    }

    if (timestamp().isValid()) {
        connectionSetting.insert(QLatin1String(NM_SETTING_CONNECTION_TIMESTAMP), timestamp().toTime_t());
    }

    if (readOnly()) {
        connectionSetting.insert(QLatin1String(NM_SETTING_CONNECTION_READ_ONLY), readOnly());
    }

    if (!zone().isEmpty()) {
        connectionSetting.insert(QLatin1String(NM_SETTING_CONNECTION_ZONE), zone());
    }

    if (!master().isEmpty()) {
        connectionSetting.insert(QLatin1String(NM_SETTING_CONNECTION_MASTER), master());
    }

    if (!slaveType().isEmpty()) {
        connectionSetting.insert(QLatin1String(NM_SETTING_CONNECTION_SLAVE_TYPE), slaveType());
    }

    if (!secondaries().isEmpty()) {
        connectionSetting.insert(QLatin1String(NM_SETTING_CONNECTION_SECONDARIES), secondaries());
    }

    result.insert(QLatin1String(NM_SETTING_CONNECTION_SETTING_NAME), connectionSetting);

    foreach(const Setting::Ptr &setting, settings()) {
        QVariantMap map = setting->toMap();
        if (!setting->isNull()) {
            result.insert(setting->name(), map);
        }
    }
    return result;
}

QString NetworkManager::Settings::ConnectionSettings::name() const
{
    Q_D(const ConnectionSettings);

    return d->name;
}

void NetworkManager::Settings::ConnectionSettings::setId(const QString& id)
{
    Q_D(ConnectionSettings);

    d->id = id;
}

QString NetworkManager::Settings::ConnectionSettings::id() const
{
    Q_D(const ConnectionSettings);

    return d->id;
}

void NetworkManager::Settings::ConnectionSettings::setUuid(const QString& uuid)
{
    Q_D(ConnectionSettings);

    d->uuid = uuid;
}

QString NetworkManager::Settings::ConnectionSettings::uuid() const
{
    Q_D(const ConnectionSettings);

    return d->uuid;
}

void NetworkManager::Settings::ConnectionSettings::setConnectionType(NetworkManager::Settings::ConnectionSettings::ConnectionType type, NMBluetoothCapabilities bt_cap)
{
    Q_D(ConnectionSettings);

    d->type = type;

    initSettings(bt_cap);
}

NetworkManager::Settings::ConnectionSettings::ConnectionType NetworkManager::Settings::ConnectionSettings::connectionType() const
{
    Q_D(const ConnectionSettings);

    return d->type;
}

void NetworkManager::Settings::ConnectionSettings::addToPermissions(const QString& user, const QString& type)
{
    Q_D(ConnectionSettings);

    d->permissions.insert(user, type);
}

void NetworkManager::Settings::ConnectionSettings::setPermissions(const QHash< QString, QString >& perm)
{
    Q_D(ConnectionSettings);

    d->permissions = perm;
}

QHash< QString, QString > NetworkManager::Settings::ConnectionSettings::permissions() const
{
    Q_D(const ConnectionSettings);

    return d->permissions;
}

void NetworkManager::Settings::ConnectionSettings::setAutoconnect(bool autoconnect)
{
    Q_D(ConnectionSettings);

    d->autoconnect = autoconnect;
}

bool NetworkManager::Settings::ConnectionSettings::autoconnect() const
{
    Q_D(const ConnectionSettings);

    return d->autoconnect;
}

void NetworkManager::Settings::ConnectionSettings::setTimestamp(const QDateTime& timestamp)
{
    Q_D(ConnectionSettings);

    d->timestamp = timestamp;
}

QDateTime NetworkManager::Settings::ConnectionSettings::timestamp() const
{
    Q_D(const ConnectionSettings);

    return d->timestamp;
}

void NetworkManager::Settings::ConnectionSettings::setReadOnly(bool readonly)
{
    Q_D(ConnectionSettings);

    d->readOnly = readonly;
}

bool NetworkManager::Settings::ConnectionSettings::readOnly() const
{
    Q_D(const ConnectionSettings);

    return d->readOnly;
}

void NetworkManager::Settings::ConnectionSettings::setZone(const QString& zone)
{
    Q_D(ConnectionSettings);

    d->zone = zone;
}

QString NetworkManager::Settings::ConnectionSettings::zone() const
{
    Q_D(const ConnectionSettings);

    return d->zone;
}

void NetworkManager::Settings::ConnectionSettings::setMaster(const QString & master)
{
    Q_D(ConnectionSettings);

    d->master = master;
}

QString NetworkManager::Settings::ConnectionSettings::master() const
{
    Q_D(const ConnectionSettings);

    return d->master;
}

void NetworkManager::Settings::ConnectionSettings::setSlaveType(const QString& type)
{
    Q_D(ConnectionSettings);

    d->slaveType = type;
}

QString NetworkManager::Settings::ConnectionSettings::slaveType() const
{
    Q_D(const ConnectionSettings);

    return d->slaveType;
}

void NetworkManager::Settings::ConnectionSettings::setSecondaries(const QStringList& secondaries)
{
    Q_D(ConnectionSettings);

    d->secondaries = secondaries;
}

QStringList NetworkManager::Settings::ConnectionSettings::secondaries() const
{
    Q_D(const ConnectionSettings);

    return d->secondaries;
}

NetworkManager::Settings::Setting::Ptr NetworkManager::Settings::ConnectionSettings::setting(NetworkManager::Settings::Setting::SettingType type) const
{
    foreach(const Setting::Ptr &setting, settings()) {
        if (setting->type() == type) {
            return setting;
        }
    }

    return Setting::Ptr();
}

NetworkManager::Settings::Setting::List NetworkManager::Settings::ConnectionSettings::settings() const
{
    Q_D(const ConnectionSettings);

    return d->settings;
}

//FOR DEBUG
void NetworkManager::Settings::ConnectionSettings::printSetting()
{
    qDebug() << "CONNECTION SETTINGS";
    qDebug() << "===================";

    qDebug() << NM_SETTING_CONNECTION_ID << ": " << id();
    qDebug() << NM_SETTING_CONNECTION_UUID << ": " << uuid();
    qDebug() << NM_SETTING_CONNECTION_TYPE << ": " << typeAsString(connectionType());
    qDebug() << NM_SETTING_CONNECTION_PERMISSIONS << ": " << permissions();
    qDebug() << NM_SETTING_CONNECTION_AUTOCONNECT << ": " << autoconnect();
    qDebug() << NM_SETTING_CONNECTION_TIMESTAMP << ": " << timestamp().toTime_t();
    qDebug() << NM_SETTING_CONNECTION_READ_ONLY << ": " << readOnly();
    qDebug() << NM_SETTING_CONNECTION_ZONE << ": " << zone();
    qDebug() << NM_SETTING_CONNECTION_MASTER << ": " << master();
    qDebug() << NM_SETTING_CONNECTION_SLAVE_TYPE << ": " << slaveType();
    qDebug() << NM_SETTING_CONNECTION_SECONDARIES << ": " << secondaries();
    qDebug() << "===================";
    foreach(const Setting::Ptr &setting, settings()) {
        qDebug() << setting->typeAsString(setting->type()).toUpper() << " SETTINGS";
        qDebug() << "---------------------------";
        setting->printSetting();
        qDebug() << "\n";
    }
}

void NetworkManager::Settings::ConnectionSettings::addSetting(const NetworkManager::Settings::Setting::Ptr &setting)
{
    Q_D(ConnectionSettings);

    d->settings.push_back(setting);
}

void NetworkManager::Settings::ConnectionSettings::clearSettings()
{
    Q_D(ConnectionSettings);

    d->settings.clear();
}
