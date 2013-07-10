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


#ifndef NMQT_VPNPLUGIN_H
#define NMQT_VPNPLUGIN_H

#include "NetworkManagerQt-export.h"
#include "generic-types.h"
#include "vpnconnection.h"

#include <QtCore/QObject>
#include <QtDBus/QDBusObjectPath>

namespace NetworkManager
{
class VpnPluginPrivate;

class NETWORKMANAGERQT_EXPORT VpnPlugin : public QObject
{
    Q_OBJECT

public:
    enum FailureType {LoginFailed, ConnectFailed, BadIpConfig};

    explicit VpnPlugin(const QString &path, QObject *parent = 0);
    virtual ~VpnPlugin();

Q_SIGNALS:
    void configChanged(const QVariantMap &configuration);
    void failureChanged(uint reason);
    void ip4ConfigChanged(const QVariantMap &ip4config);
    void ip6ConfigChanged(const QVariantMap &ip6config);
    void loginBannerChanged(const QString &banner);
    void stateChanged(VpnConnection::State state);

protected Q_SLOTS:
    void connect(const NMVariantMapMap &connection);
    void disconnect();
    QString needSecrets(const NMVariantMapMap &connection);
    void setConfig(const QVariantMap &config);
    void setFailure(const QString &reason);
    void setIp4Config(const QVariantMap &config);
    void setIp6Config(const QVariantMap &config);
    void onStateChanged(uint state);

protected:
    VpnPluginPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(VpnPlugin)
};

} // namespace NetworkManager

#endif //NMQT_VPNPLUGIN_H
