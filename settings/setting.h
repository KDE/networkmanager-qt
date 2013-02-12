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

#ifndef NMQT_SETTING_H
#define NMQT_SETTING_H

#include "../QtNetworkManager-export.h"

#include <QtCore/QVariantMap>
#include <QtCore/QStringList>

namespace NetworkManager
{
namespace Settings
{
    class SettingPrivate;

    class NMQT_EXPORT Setting
    {
    Q_DECLARE_PRIVATE(Setting);
    public:
	enum SettingType {Adsl, Cdma, Gsm, Infiniband, Ipv4, Ipv6, Ppp, Pppoe, Security8021x, Serial,
			   Vpn, Wired, Wireless, WirelessSecurity, Bluetooth, OlpcMesh, Vlan, Wimax, Bond, Bridge, BridgePort};
        enum SecretFlagType {None = 0, AgentOwned = 0x01, NotSaved = 0x02, NotRequired = 0x04};

        static QString typeAsString(SettingType type);
	static SettingType typeFromString(const QString & type);

	Setting(SettingType type);
	Setting(Setting * setting);
	virtual ~Setting();

	virtual void fromMap(const QVariantMap & map) = 0;

        virtual QVariantMap toMap() const = 0;

        virtual bool hasSecrets() const;

        virtual void secretsFromMap(const QVariantMap & map);

        virtual QVariantMap secretsToMap() const;

        virtual QStringList needSecrets() const;

        virtual QString name() const = 0;

	void setInitialized(bool initialized);

	bool isNull() const;

	void setType(SettingType type);
	SettingType type() const;

	//FOR DEBUG
	virtual void printSetting();
    private:
	SettingPrivate * d_ptr;
    };
}
}

#endif // NMQT_SETTING_H
