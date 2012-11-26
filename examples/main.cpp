/*
Copyright 2011 Lamarque V. Souza <lamarque@kde.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <arpa/inet.h>

#include <QTextStream>

#include <QtNetworkManager/manager.h>
#include <QtNetworkManager/device.h>
#include <QtNetworkManager/activeconnection.h>
#include <QtNetworkManager/connection.h>
#include <QtNetworkManager/connectionsettings.h>

QString typeAsString(const int type)
{
    switch (type) {
    case 0x0: return QString("Unknown");
    case 0x1: return QString("Ethernet");
    case 0x2: return QString("Wifi");
    case 0x3: return QString("Unused1");
    case 0x4: return QString("Unused2");
    case 0x5: return QString("Bluetooth");
    case 0x6: return QString("OlpcMesh");
    case 0x7: return QString("Wimax");
    case 0x8: return QString("Modem");
    }
    return QString("Unknown");
}

int main()
{
    QTextStream qout(stdout, QIODevice::WriteOnly);

    NetworkManager::DeviceList list = NetworkManager::networkInterfaces();

    foreach (NetworkManager::Device *dev, list) {
        qout << "\n=====\n";
        qout << dev->uni() << "\n";
        qout << "type: " << typeAsString(dev->type()) << "\n";
        qout << "managed: " << dev->managed() << "\n";
        qout << "interface name: " << dev->interfaceName() << "\n";

        // dev->ipV4Config() is valid only in activated state
        if (dev->state() == NetworkManager::Device::Activated) {
            // static IPv4 configuration.
            if (dev->ipV4Config().addresses().isEmpty()) {
                qout << "ip address: <not set>\n";
            } else {
                QHostAddress addr;
                addr.setAddress(dev->ipV4Config().addresses().at(0).address());
                qout << "ip address: " << addr.toString() << "\n";
                qout << "ip address (raw): " << dev->ipV4Address() << "\n";

                if (dev->ipV4Config().routes().isEmpty()) {
                    qout << "default gateway: <not set>\n";
                } else {
                    addr.setAddress(dev->ipV4Config().routes().at(0).route());
                    qout << "default gateway: " << addr.toString() << "\n";
                }
            }
            // DHCPv4 configuration.
            NetworkManager::Dhcp4Config *dhcp4Config = dev->dhcp4Config();
            if (dhcp4Config == 0) {
                qout << "(dhcp) ip address: <not set>\n";
            } else {
                qout << "(dhcp) ip address: " << dhcp4Config->optionValue("ip_address") << "\n";
                qout << "(dhcp) network: " << dhcp4Config->optionValue("network_number") << '/' << dhcp4Config->optionValue("subnet_cidr") << " (" << dhcp4Config->optionValue("subnet_mask") << ")\n";

                if (dhcp4Config->optionValue("routers").isEmpty()) {
                    qout << "(dhcp) gateway(s): <not set>\n";
                } else {
                    qout << "(dhcp) gateway(s): " << dhcp4Config->optionValue("routers") << "\n";
                }

                if (dhcp4Config->optionValue("domain_name_servers").isEmpty()) {
                    qout << "(dhcp) domain name server(s): <not set>\n";
                } else {
                    qout << "(dhcp) domain name server(s): " << dhcp4Config->optionValue("domain_name_servers") << "\n";
                }
            }
        }
    }
    qout << "\n";

    QList<NetworkManager::ActiveConnection*> lst = NetworkManager::activeConnections();

    foreach (const NetworkManager::ActiveConnection * con, lst) {
	NetworkManager::Settings::ConnectionSettings * settings = new NetworkManager::Settings::ConnectionSettings();
	settings->fromMap(con->connection()->settings());
	qDebug() << "\n\n";
    }
}
