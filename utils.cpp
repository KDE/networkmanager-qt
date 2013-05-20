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

#include "utils.h"

QHostAddress NetworkManager::Utils::ipv6AddressAsHostAddress(const QByteArray& address)
{
    Q_IPV6ADDR tmp;
    for (int i = 0; i < 16; i++) {
        tmp[i] = address[i];
    }
    QHostAddress hostaddress(tmp);

    return hostaddress;
}

QByteArray NetworkManager::Utils::ipv6AddressFromHostAddress(const QHostAddress& address)
{
    Q_IPV6ADDR tmp = address.toIPv6Address();
    QByteArray assembledAddress;
    for (int i = 0; i <16; i++) {
        assembledAddress[i] = tmp[i];
    }

    return assembledAddress;
}

QString NetworkManager::Utils::macAddressAsString(const QByteArray & ba)
{
    QStringList mac;

    for (int i=0; i < ba.size(); i++) {
        mac << QString("%1").arg((quint8)ba[i], 2, 16, QLatin1Char('0')).toUpper();
    }

    return mac.join(":");
}

QByteArray NetworkManager::Utils::macAddressFromString( const QString & s)
{
    QStringList macStringList = s.split(':');
    QByteArray ba;
    if (!s.isEmpty()) {
        ba.resize(6);
        int i = 0;

        foreach (const QString & macPart, macStringList) {
            ba[i++] = macPart.toUInt(0, 16);
        }
    }
    return ba;
}

int NetworkManager::Utils::findChannel(int freq)
{
    int channel;
    if (freq < 2500) {
        channel = 0;
        int i = 0;
        QList<QPair<int, int> > bFreqs = getBFreqs();
        while (i < bFreqs.size()) {
            if (bFreqs.at(i).second <= freq) {
                channel = bFreqs.at(i).first;
            } else {
                break;
            }
            i++;
        }
        return channel;
    }
    channel = 0;
    int i = 0;
    QList<QPair<int, int> > aFreqs = getAFreqs();
    while (i < aFreqs.size()) {
        if (aFreqs.at(i).second <= freq) {
            channel = aFreqs.at(i).first;
        } else {
            break;
        }
        i++;
    }

    return channel;
}

QList<QPair<int, int> > NetworkManager::Utils::getBFreqs()
{
    QList<QPair<int, int> > freqs;

    freqs.append(QPair<int, int>(1, 2412));
    freqs.append(QPair<int, int>(2, 2417));
    freqs.append(QPair<int, int>(3, 2422));
    freqs.append(QPair<int, int>(4, 2427));
    freqs.append(QPair<int, int>(5, 2432));
    freqs.append(QPair<int, int>(6, 2437));
    freqs.append(QPair<int, int>(7, 2442));
    freqs.append(QPair<int, int>(8, 2447));
    freqs.append(QPair<int, int>(9, 2452));
    freqs.append(QPair<int, int>(10, 2457));
    freqs.append(QPair<int, int>(11, 2462));
    freqs.append(QPair<int, int>(12, 2467));
    freqs.append(QPair<int, int>(13, 2472));
    freqs.append(QPair<int, int>(14, 2484));

    return freqs;
}

QList<QPair<int, int> > NetworkManager::Utils::getAFreqs()
{
    QList<QPair<int, int> > freqs;

    freqs.append(QPair<int, int>(183, 4915));
    freqs.append(QPair<int, int>(184, 4920));
    freqs.append(QPair<int, int>(185, 4925));
    freqs.append(QPair<int, int>(187, 4935));
    freqs.append(QPair<int, int>(188, 4940));
    freqs.append(QPair<int, int>(189, 4945));
    freqs.append(QPair<int, int>(192, 4960));
    freqs.append(QPair<int, int>(196, 4980));
    freqs.append(QPair<int, int>(7, 5035));
    freqs.append(QPair<int, int>(8, 5040));
    freqs.append(QPair<int, int>(9, 5045));
    freqs.append(QPair<int, int>(11, 5055));
    freqs.append(QPair<int, int>(12, 5060));
    freqs.append(QPair<int, int>(16, 5080));
    freqs.append(QPair<int, int>(34, 5170));
    freqs.append(QPair<int, int>(36, 5180));
    freqs.append(QPair<int, int>(38, 5190));
    freqs.append(QPair<int, int>(40, 5200));
    freqs.append(QPair<int, int>(42, 5210));
    freqs.append(QPair<int, int>(44, 5220));
    freqs.append(QPair<int, int>(46, 5230));
    freqs.append(QPair<int, int>(48, 5240));
    freqs.append(QPair<int, int>(52, 5260));
    freqs.append(QPair<int, int>(56, 5280));
    freqs.append(QPair<int, int>(60, 5300));
    freqs.append(QPair<int, int>(64, 5320));
    freqs.append(QPair<int, int>(100, 5500));
    freqs.append(QPair<int, int>(104, 5520));
    freqs.append(QPair<int, int>(108, 5540));
    freqs.append(QPair<int, int>(112, 5560));
    freqs.append(QPair<int, int>(116, 5580));
    freqs.append(QPair<int, int>(120, 5600));
    freqs.append(QPair<int, int>(124, 5620));
    freqs.append(QPair<int, int>(128, 5640));
    freqs.append(QPair<int, int>(132, 5660));
    freqs.append(QPair<int, int>(136, 5680));
    freqs.append(QPair<int, int>(140, 5700));
    freqs.append(QPair<int, int>(149, 5745));
    freqs.append(QPair<int, int>(153, 5765));
    freqs.append(QPair<int, int>(157, 5785));
    freqs.append(QPair<int, int>(161, 5805));
    freqs.append(QPair<int, int>(165, 5825));

    return freqs;
}
