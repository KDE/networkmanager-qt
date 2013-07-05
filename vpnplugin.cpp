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

#include "vpnplugin.h"
#include "vpnconnection.h"

#include "manager_p.h"
#include "nm-vpn-plugininterface.h"

class NetworkManager::VpnPluginPrivate
{
public:
    VpnPluginPrivate(const QString &path);

    VpnConnection::State state;
    OrgFreedesktopNetworkManagerVPNPluginInterface iface;
};

NetworkManager::VpnPluginPrivate::VpnPluginPrivate(const QString &path):
    iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

NetworkManager::VpnPlugin::VpnPlugin(const QString &path, QObject *parent):
    QObject(parent), d_ptr(new VpnPluginPrivate(path))
{
    Q_D(VpnPlugin);
    d->state = (NetworkManager::VpnConnection::State)d->iface.state();

    QObject::connect(&d->iface, SIGNAL(Config(QVariantMap)),
                     this, SLOT(setConfig(QVariantMap)));
    QObject::connect(&d->iface, SIGNAL(Failure(uint)),
                     this, SLOT(setFailure(QString)));
    QObject::connect(&d->iface, SIGNAL(Ip4Config(QVariantMap)),
                     this, SLOT(setIp4Config(QVariantMap)));
    QObject::connect(&d->iface, SIGNAL(Ip6Config(QVariantMap)),
                     this, SLOT(setIp6Config(QVariantMap)));
    //QObject::connect(&d->iface, SIGNAL(LoginBanner(QString)),
    //        this, SLOT(onLoginBanner(QString)));
    QObject::connect(&d->iface, SIGNAL(StateChanged(uint)),
                     this, SLOT(onStateChanged(uint)));
}

NetworkManager::VpnPlugin::~VpnPlugin()
{
    delete d_ptr;
}

void NetworkManager::VpnPlugin::connect(const NMVariantMapMap &connection)
{
    Q_D(VpnPlugin);

    QDBusPendingReply<> reply = d->iface.Connect(connection);
}

void NetworkManager::VpnPlugin::disconnect()
{
    Q_D(VpnPlugin);

    QDBusPendingReply<> reply = d->iface.Disconnect();
}

QString NetworkManager::VpnPlugin::needSecrets(const NMVariantMapMap &connection)
{
    Q_D(VpnPlugin);

    QDBusPendingReply<QString> reply = d->iface.NeedSecrets(connection);

    return reply.value();
}

void NetworkManager::VpnPlugin::setConfig(const QVariantMap &configuration)
{
    Q_D(VpnPlugin);

    QDBusPendingReply<QString> reply = d->iface.SetConfig(configuration);

    emit configChanged(configuration);
}

void NetworkManager::VpnPlugin::setFailure(const QString &reason)
{
    Q_D(VpnPlugin);

    QDBusPendingReply<QString> reply = d->iface.SetFailure(reason);

    //TODO
    //emit failureChanged(reason);
}

void NetworkManager::VpnPlugin::setIp4Config(const QVariantMap &config)
{
    Q_D(VpnPlugin);

    QDBusPendingReply<> reply = d->iface.SetIp4Config(config);

    emit ip4ConfigChanged(config);
}

void NetworkManager::VpnPlugin::setIp6Config(const QVariantMap &config)
{
    Q_D(VpnPlugin);

    QDBusPendingReply<> reply = d->iface.SetIp6Config(config);

    emit ip6ConfigChanged(config);
}

void NetworkManager::VpnPlugin::onStateChanged(uint state)
{
    Q_D(VpnPlugin);

    d->state = (VpnConnection::State) state;

    emit stateChanged(d->state);
}

#include "vpnplugin.moc"
