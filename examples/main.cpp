/*
Copyright 2011-2013 Lamarque V. Souza <lamarque@kde.org>

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

#include <QtCore/QTextStream>

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Device>
#include <NetworkManagerQt/ActiveConnection>
#include <NetworkManagerQt/Connection>

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

    NetworkManager::Device::List list = NetworkManager::networkInterfaces();

    // List device configuration, not including vpn connections, which do not
    // have a real device tied to them.
    foreach (NetworkManager::Device::Ptr dev, list) {
        qout << "\n=====\n";
        qout << dev->uni() << "\n";
        qout << "type: " << typeAsString(dev->type()) << "\n";
        qout << "managed: " << dev->managed() << "\n";
        qout << "interface name: " << dev->interfaceName() << "\n";

        NetworkManager::IpConfig ipConfig = dev->ipV4Config();
        if (ipConfig.isValid()) {
            // static IPv4 configuration.
            if (ipConfig.addresses().isEmpty()) {
                qout << "ip address: <not set>\n";
            } else {
                NetworkManager::IpAddress address = ipConfig.addresses().at(0);
                qout << "ip address: " << address.ip().toString() << "\n";
                qout << "gateway: " << address.gateway().toString() << "\n";
                qout << "ip address (raw): " << dev->ipV4Address().toString() << "\n";

                // Static routes.
                if (ipConfig.routes().isEmpty()) {
                    qout << "routers: <not set>\n";
                } else {
                    qout << "routers: " << ipConfig.routes().at(0).ip().toString() << "\n";
                }

                if (ipConfig.nameservers().isEmpty()) {
                    qout << "nameserver: <not set>\n";
                } else {
                    qout << "nameserver: " << ipConfig.nameservers().at(0).toString() << "\n";
                }
            }
            // DHCPv4 configuration.
            NetworkManager::Dhcp4Config::Ptr dhcp4Config = dev->dhcp4Config();
            if (!dhcp4Config) {
                qout << "dhcp info unavailable\n";
            } else {
                qout << "Dhcp4 options (" << dhcp4Config->path() << "): ";
                QVariantMap options = dhcp4Config->options();
                QVariantMap::ConstIterator it = options.constBegin();
                QVariantMap::ConstIterator end = options.constEnd();
                for (; it != end; ++it) {
                    qout << it.key() << "=" << it.value().toString() << " ";
                }
                qout << "\n";

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

        NetworkManager::Connection::List connections = dev->availableConnections();

        qout << "available connections: ";

        foreach (NetworkManager::Connection::Ptr con, connections) {
            qout << "con";
            NetworkManager::ConnectionSettings::Ptr settings = con->settings();
            qout << "\"" << settings->id() << "\" ";
        }
    }
    qout << "\n";
}
