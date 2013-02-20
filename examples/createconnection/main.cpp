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

#include <arpa/inet.h>

#include <QtNetworkManager/manager.h>
#include <QtNetworkManager/device.h>
#include <QtNetworkManager/wirelessdevice.h>
#include <QtNetworkManager/accesspoint.h>
#include <QtNetworkManager/generic-types.h>
#include <QtNetworkManager/connection.h>
#include <QtNetworkManager/settings/connection.h>
#include <QtNetworkManager/settings/802-11-wireless.h>
#include <QtNetworkManager/settings/802-11-wireless-security.h>
#include <QtNetworkManager/settings/ipv4.h>

#include <QtDBus/QDBusMetaType>
#include <QtCore/QTextStream>
#include <QtCore/QUuid>

using namespace NetworkManager;

int main()
{
    qDBusRegisterMetaType< QVariantMapMap >();
    QTextStream qout(stdout, QIODevice::WriteOnly);
    QTextStream qin(stdin, QIODevice::ReadOnly);

    Settings::ConnectionSettings * settings = new Settings::ConnectionSettings(Settings::ConnectionSettings::Wireless);
    DeviceList deviceList = NetworkManager::networkInterfaces();

    WirelessDevice * wifiDevice;

    foreach (Device * dev, deviceList) {
        if (dev->type() == Device::Wifi) {
            wifiDevice = dynamic_cast<WirelessDevice*>(dev);
            break;
        }
    }

    QStringList accessPointList = wifiDevice->accessPoints();
    QString ssid;
    QString result;
    QString accessPointPath;

    foreach (const QString & ap, accessPointList) {
        AccessPoint accessPoint(ap);
        // For simplification we use APs only with Wep security or without any security
        if (accessPoint.wpaFlags().testFlag(AccessPoint::PairWep40) ||
            accessPoint.wpaFlags().testFlag(AccessPoint::PairWep104) ||
            accessPoint.wpaFlags().testFlag(AccessPoint::GroupWep40) ||
            accessPoint.wpaFlags().testFlag(AccessPoint::GroupWep104) ||
            !accessPoint.wpaFlags()) {

            qout << "Do you want to connect to " << accessPoint.ssid() << "?" << endl;
            qout << "Yes/No: " << flush;
            qin >> result;

            if (result == "Yes" || result == "yes" || result == "y" || result == "YES") {
                ssid = accessPoint.ssid();
                accessPointPath = ap;
                break;
            }
        }
    }

    settings->setId(ssid);
    settings->setUuid(QUuid::createUuid().toString().mid(1, QUuid::createUuid().toString().length() - 2));

    Settings::WirelessSetting * wirelessSetting = dynamic_cast<Settings::WirelessSetting *>(settings->setting(Settings::Setting::Wireless));
    wirelessSetting->setSsid(ssid.toAscii());

    Settings::Ipv4Setting * ipv4Setting = dynamic_cast<Settings::Ipv4Setting *>(settings->setting(Settings::Setting::Ipv4));
    ipv4Setting->setMethod(Settings::Ipv4Setting::Automatic);

    QDBusPendingReply <QDBusObjectPath, QDBusObjectPath > reply = NetworkManager::addAndActivateConnection(settings->toMap(), wifiDevice->uni(), accessPointPath);

    reply.waitForFinished();

    if (reply.isValid()) {
        Settings::Connection connection(reply.value().path());
        Settings::ConnectionSettings * newSettings = new Settings::ConnectionSettings(Settings::ConnectionSettings::Wireless);
        newSettings->fromMap(connection.settings());
        // Print resulting settings
        newSettings->printSetting();

        Settings::WirelessSecuritySetting * wirelessSecuritySetting = dynamic_cast<Settings::WirelessSecuritySetting *>(newSettings->setting(Settings::Setting::WirelessSecurity));
        if (!wirelessSecuritySetting->needSecrets().isEmpty()) {
            qDebug() << "Need secrets: " << wirelessSecuritySetting->needSecrets();
            // TODO: fill missing secrets
        }

    } else {
        qDebug() << "Connection failed: " << reply.error();
    }
}
