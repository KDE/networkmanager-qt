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

#ifndef NM_WIREDSETTING_H
#define NM_WIREDSETTING_H

#include "QtNetworkManager-export.h"
#include "setting.h"

#include <QStringList>
#include <QMap>

namespace NetworkManager
{
namespace Settings
{
    class WiredSettingPrivate;

    class NMQT_EXPORT WiredSetting : public Setting
    {
    Q_DECLARE_PRIVATE(WiredSetting)
    public:
	enum PortType {Tp, Aui, Bnc, Mii};
	enum DuplexType {Half, Full};
	enum S390Nettype {Qeth, Lcs, Ctc};

	WiredSetting();
	WiredSetting(WiredSetting *);
	~WiredSetting();

	QString name() const;

	void setPort(const PortType port);
	PortType port() const;

	void setSpeed(const quint32 speed);
	quint32 speed() const;

	void setDuplexType(const DuplexType type);
	DuplexType duplexType() const;

	void setAutoNegotiate(const bool autoNegotiate);
	bool autoNegotiate() const;

	void setMacAddress(const QByteArray & address);
	QByteArray macAddress() const;

	void setClonedMacAddress(const QByteArray & address);
	QByteArray clonedMacAddress() const;

	void setMacAddressBlacklist(const QStringList & list);
	QStringList macAddressBlacklist() const;

	void setMtu(const quint32 mtu);
	quint32 mtu() const;

	void setS390Subchannels(const QStringList & channels);
	QStringList s390Subchannels() const;

	void setS390NetType(const S390Nettype type);
	S390Nettype s390NetType() const;

	void setS390Options(const QMap<QString, QString> & options);
	QMap<QString, QString> s390Options() const;

	void fromMap(const QVariantMap & setting);

	//FOR DEBUG
	void printSetting();
    protected:
	WiredSettingPrivate *d_ptr;
    };
}
}

#endif // NM_WIREDSETTING_H
