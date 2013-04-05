/*
Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
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

#include "modemdevice.h"
#include "modemdevice_p.h"

#include <QtModemManager/manager.h>

#include "manager_p.h"

NetworkManager::ModemDevicePrivate::ModemDevicePrivate(const QString & path, QObject * owner)
    : DevicePrivate(path, owner), modemIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

NetworkManager::ModemDevice::ModemDevice(const QString & path, QObject * parent)
    : Device(*new ModemDevicePrivate(path, this), parent),
      modemGsmCardIface(0), modemGsmNetworkIface(0)
{
    initModemProperties();
}

void NetworkManager::ModemDevice::initModemProperties()
{
    Q_D(ModemDevice);
    //TODO: get properties and store them
    d->modemCapabilities = convertModemCapabilities(d->modemIface.modemCapabilities());
    d->currentCapabilities = convertModemCapabilities(d->modemIface.currentCapabilities());
    d->m_modemUdi = getUdiForModemManager();
    connect(&d->modemIface, SIGNAL(PropertiesChanged(QVariantMap)),
                this, SLOT(modemPropertiesChanged(QVariantMap)));
}

NetworkManager::ModemDevice::ModemDevice(NetworkManager::ModemDevicePrivate & dd, QObject * parent) : Device(dd, parent),
      modemGsmCardIface(0), modemGsmNetworkIface(0)
{
    initModemProperties();
}

NetworkManager::ModemDevice::~ModemDevice()
{

}

NetworkManager::Device::Type NetworkManager::ModemDevice::type() const
{
    return NetworkManager::Device::Modem;
}

NetworkManager::ModemDevice::Capabilities NetworkManager::ModemDevice::convertModemCapabilities(uint theirCaps)
{
    NetworkManager::ModemDevice::Capabilities ourCaps
        = (NetworkManager::ModemDevice::Capabilities) theirCaps;
    return ourCaps;
}

NetworkManager::ModemDevice::Capabilities NetworkManager::ModemDevice::currentCapabilities() const
{
    Q_D(const ModemDevice);
    return d->currentCapabilities;
}

NetworkManager::ModemDevice::Capabilities NetworkManager::ModemDevice::modemCapabilities() const
{
    Q_D(const ModemDevice);
    return d->modemCapabilities;
}

void NetworkManager::ModemDevice::modemPropertiesChanged(const QVariantMap &properties)
{
    Q_D(ModemDevice);

    QVariantMap::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        QString property = it.key();
        if (property == QLatin1String("CurrentCapabilities")) {
            d->currentCapabilities = convertModemCapabilities((*it).toUInt());
            emit currentCapabilitiesChanged(d->currentCapabilities);
        } else {
            qWarning() << Q_FUNC_INFO << "Unhandled property" << property;
        }
        ++it;
    }
}

QString NetworkManager::ModemDevice::getUdiForModemManager()
{
    if (driver() != QLatin1String("bluez")) {
        return udi();
    }

    /* BlueZ knows about the rfcommX string that we could use to find the device in ModemManager
     * but does not export this info, so let's use the first bluetooth device we find in ModemManager.
     * Modem will be registered in ModemManager only after someone execute its org.bluez.Serial.Connect method. */
    foreach(const ModemManager::ModemInterface *modem, ModemManager::modemInterfaces()) {
        if (modem->driver() == QLatin1String("bluetooth")) {
            return modem->udi();
        }
    }

    modemRemoved(udi());
    return QString();
}

ModemManager::ModemGsmCardInterface * NetworkManager::ModemDevice::getModemCardIface()
{
    Q_D(ModemDevice);
    d->m_modemUdi = getUdiForModemManager();
    if (d->m_modemUdi.isEmpty()) {
        return 0;
    }
    if (modemGsmCardIface == 0) {
        modemGsmCardIface = qobject_cast<ModemManager::ModemGsmCardInterface*> (ModemManager::findModemInterface(d->m_modemUdi, ModemManager::ModemInterface::GsmCard));
        connect(ModemManager::notifier(), SIGNAL(modemRemoved(QString)), this, SLOT(modemRemoved(QString)));
    }

    return modemGsmCardIface;
}

ModemManager::ModemGsmNetworkInterface * NetworkManager::ModemDevice::getModemNetworkIface()
{
    Q_D(ModemDevice);
    d->m_modemUdi = getUdiForModemManager();
    if (d->m_modemUdi.isEmpty()) {
        return 0;
    }
    if (modemGsmNetworkIface == 0) {
        modemGsmNetworkIface = qobject_cast<ModemManager::ModemGsmNetworkInterface*> (ModemManager::findModemInterface(d->m_modemUdi, ModemManager::ModemInterface::GsmNetwork));
        if (modemGsmNetworkIface) {
            connect(ModemManager::notifier(), SIGNAL(modemRemoved(QString)), this, SLOT(modemRemoved(QString)));
        }
    }

    return modemGsmNetworkIface;
}

void NetworkManager::ModemDevice::modemRemoved(const QString & modemUdi)
{
    Q_D(ModemDevice);
    if (modemUdi == d->m_modemUdi) {
        modemGsmNetworkIface = 0;
        modemGsmCardIface = 0;
    }
}

void NetworkManager::ModemDevice::setModemCardIface(ModemManager::ModemGsmCardInterface * iface)
{
    modemGsmCardIface = iface;
}

void NetworkManager::ModemDevice::setModemNetworkIface(ModemManager::ModemGsmNetworkInterface * iface)
{
    modemGsmNetworkIface = iface;
}

// vim: sw=4 sts=4 et tw=100

#include "modemdevice.moc"
