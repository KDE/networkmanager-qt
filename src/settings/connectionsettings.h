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

#ifndef NETWORKMANAGERQT_CONNECTION_SETTINGS_H
#define NETWORKMANAGERQT_CONNECTION_SETTINGS_H

#include <networkmanagerqt/networkmanagerqt_export.h>
#include "setting.h"

#undef signals
#include <libnm/NetworkManager.h>
#define signals Q_SIGNALS

#include <QString>
#include <QDateTime>
#include <QHash>

namespace NetworkManager
{

class ConnectionSettingsPrivate;

/**
 * Represents collection of all connection settings
 */
class NETWORKMANAGERQT_EXPORT ConnectionSettings
{
    Q_ENUMS(ConnectionType)
public:
    typedef QSharedPointer<ConnectionSettings> Ptr;
    typedef QList<Ptr> List;
    enum ConnectionType {
        Unknown = 0,
        Adsl,
        Bluetooth,
        Bond,
        Bridge,
        Cdma,
        Gsm,
        Infiniband,
        OLPCMesh,
        Pppoe,
        Vlan,
        Vpn,
        Wimax,
        Wired,
        Wireless,
        Team,
        Generic,
        Tun,
        IpTunnel,
        WireGuard
    };

    enum AutoconnectSlaves {
        SlavesDefault = -1,
        DoNotConnectSlaves = 0,
        ConnectAllSlaves = 1
    };

    enum Lldp {
        LldpDefault = -1,
        LldpDisable = 0,
        LldpEnableRx = 1,
    };

    // FIXME same enum as in device.h, unfortunately it's not possible to use that one
    // maybe in future move all enums into one header so they can be used across all classes
    enum Metered {
        MeteredUnknown = 0,
        MeteredYes = 1,
        MeteredNo = 2,
        MeteredGuessYes = 3,
        MeteredGuessNo = 4
    };


    static ConnectionType typeFromString(const QString &typeString);
    static QString typeAsString(const ConnectionType type);
    static QString createNewUuid();

    ConnectionSettings();
    explicit ConnectionSettings(ConnectionType type, NMBluetoothCapabilities bt_cap = NM_BT_CAPABILITY_DUN);
    explicit ConnectionSettings(const ConnectionSettings::Ptr &other);
    explicit ConnectionSettings(const NMVariantMapMap &map);
    virtual ~ConnectionSettings();

    QString name() const;

    void fromMap(const NMVariantMapMap &map);

    NMVariantMapMap toMap() const;

    void setId(const QString &id);
    QString id() const;

    void setUuid(const QString &uuid);
    QString uuid() const;

    void setInterfaceName(const QString &interfaceName);
    QString interfaceName() const;

    void setConnectionType(ConnectionType type, NMBluetoothCapabilities bt_cap = NM_BT_CAPABILITY_DUN);
    ConnectionType connectionType() const;

    void addToPermissions(const QString &user, const QString &type);
    void setPermissions(const QHash<QString, QString> &perm);
    QHash<QString, QString> permissions() const;

    void setAutoconnect(bool autoconnect);
    bool autoconnect() const;

    void setAutoconnectPriority(int priority);
    int autoconnectPriority() const;

    void setTimestamp(const QDateTime &timestamp);
    QDateTime timestamp() const;

    void setReadOnly(bool readonly);
    bool readOnly() const;

    void setZone(const QString &zone);
    QString zone() const;

    bool isSlave() const;

    void setMaster(const QString &master);
    QString master() const;

    void setSlaveType(const QString &type);
    QString slaveType() const;

    void setSecondaries(const QStringList &secondaries);
    QStringList secondaries() const;

    void setGatewayPingTimeout(quint32 timeout);
    quint32 gatewayPingTimeout() const;

    void setAutoconnectRetries(int retries);
    int autoconnectRetries() const;

    void setAutoconnectSlaves(AutoconnectSlaves autoconnectSlaves);
    AutoconnectSlaves autoconnectSlaves() const;

    void setLldp(Lldp lldp);
    Lldp lldp() const;

    void setMetered(Metered metered);
    Metered metered() const;

    void setStableId(const QString &stableId);
    QString stableId() const;

    Setting::Ptr setting(Setting::SettingType type) const;
    Setting::Ptr setting(const QString &type) const;

    Setting::List settings() const;

protected:
    ConnectionSettingsPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(ConnectionSettings)
};

NETWORKMANAGERQT_EXPORT QDebug operator<<(QDebug dbg, const ConnectionSettings &setting);

}

#endif // NETWORKMANAGERQT_CONNECTION_SETTINGS_H
