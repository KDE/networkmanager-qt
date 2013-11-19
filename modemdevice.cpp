/*
    Copyright 2008,2011 Will Stephenson <wstephenson@kde.org>
    Copyright 2011-2013 Lamarque Souza <lamarque@kde.org>
    Copyright 2013 Daniel Nicoletti <dantti12@gmail.com>
    Copyright 2013 Jan Grulich <jgrulich@redhat.com>

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

#if WITH_MODEMMANAGERQT
#include <ModemManagerQt/manager.h>
#include <ModemManagerQt/modemdevice.h>
#include <ModemManagerQt/modem.h>
#include <ModemManagerQt/sim.h>
#endif

#include "manager_p.h"

NetworkManager::ModemDevice::Capabilities convertModemCapabilities(uint theirCaps)
{
    NetworkManager::ModemDevice::Capabilities ourCaps
        = (NetworkManager::ModemDevice::Capabilities) theirCaps;
    return ourCaps;
}

NetworkManager::ModemDevicePrivate::ModemDevicePrivate(const QString &path, ModemDevice *q)
    : DevicePrivate(path, q)
    , modemIface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
{
}

void NetworkManager::ModemDevicePrivate::initModemProperties()
{
    Q_Q(ModemDevice);
    modemCapabilities = convertModemCapabilities(modemIface.modemCapabilities());
    currentCapabilities = convertModemCapabilities(modemIface.currentCapabilities());
}

NetworkManager::ModemDevice::ModemDevice(const QString &path, QObject *parent)
    : Device(*new ModemDevicePrivate(path, this), parent)
#if WITH_MODEMMANAGERQT
    , modemSimCardIface(0)
    , modemNetworkIface(0)
#endif
{
    Q_D(ModemDevice);
    d->initModemProperties();
#if WITH_MODEMMANAGERQT
    d->m_modemUdi = getUdiForModemManager();
#endif
    connect(&d->modemIface, SIGNAL(PropertiesChanged(QVariantMap)),
            this, SLOT(propertiesChanged(QVariantMap)));
}

NetworkManager::ModemDevice::ModemDevice(NetworkManager::ModemDevicePrivate &dd, QObject *parent)
    : Device(dd, parent)
#if WITH_MODEMMANAGERQT
    , modemSimCardIface(0)
    , modemNetworkIface(0)
#endif
{
    Q_D(ModemDevice);
    d->initModemProperties();
#if WITH_MODEMMANAGERQT
    d->m_modemUdi = getUdiForModemManager();
#endif
    connect(&d->modemIface, SIGNAL(PropertiesChanged(QVariantMap)),
            this, SLOT(propertiesChanged(QVariantMap)));
}

NetworkManager::ModemDevice::~ModemDevice()
{
}

NetworkManager::Device::Type NetworkManager::ModemDevice::type() const
{
    return NetworkManager::Device::Modem;
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

void NetworkManager::ModemDevice::propertyChanged(const QString &property, const QVariant &value)
{
    Q_D(ModemDevice);

    if (property == QLatin1String("CurrentCapabilities")) {
        d->currentCapabilities = convertModemCapabilities(value.toUInt());
        emit currentCapabilitiesChanged(d->currentCapabilities);
    } else {
        Device::propertyChanged(property, value);
    }
}

#if WITH_MODEMMANAGERQT
QString NetworkManager::ModemDevice::getUdiForModemManager()
{
    if (driver() != QLatin1String("bluez")) {
        return udi();
    }

    /* BlueZ knows about the rfcommX string that we could use to find the device in ModemManager
     * but does not export this info, so let's use the first bluetooth device we find in ModemManager.
     * Modem will be registered in ModemManager only after someone execute its org.bluez.Serial.Connect method. */
    foreach (const ModemManager::ModemDevice::Ptr &modem, ModemManager::modemDevices()) {
        if (modem->hasInterface(ModemManager::ModemDevice::ModemInterface)) {
            ModemManager::Modem::Ptr modemInterface = modem->interface(ModemManager::ModemDevice::ModemInterface).objectCast<ModemManager::Modem>();
            if (modemInterface) {
                if (modemInterface->drivers().contains(QLatin1String("bluetooth"))) {
                    return modem->uni();
                }
            }
        }

    }

    modemRemoved(udi());
    return QString();
}

ModemManager::Sim::Ptr NetworkManager::ModemDevice::getModemCardIface()
{
    Q_D(ModemDevice);

    d->m_modemUdi = getUdiForModemManager();
    if (d->m_modemUdi.isEmpty()) {
        return ModemManager::Sim::Ptr();
    }
    if (modemSimCardIface == 0) {
        ModemManager::ModemDevice::Ptr modem = ModemManager::findModemDevice(d->m_modemUdi);
        if (modem) {
            if (!modem->sims().isEmpty()) {
                modemSimCardIface = modem->sims().first();
            }
            connect(ModemManager::notifier(), SIGNAL(modemRemoved(QString)), this, SLOT(modemRemoved(QString)));
        }
    }

    return modemSimCardIface;
}

ModemManager::Modem::Ptr NetworkManager::ModemDevice::getModemNetworkIface()
{
    Q_D(ModemDevice);
    d->m_modemUdi = getUdiForModemManager();
    if (d->m_modemUdi.isEmpty()) {
        return ModemManager::Modem::Ptr();
    }
    if (modemNetworkIface.isNull()) {
        ModemManager::ModemDevice::Ptr modem = ModemManager::findModemDevice(d->m_modemUdi);
        if (modem) {
            if (modem->hasInterface(ModemManager::ModemDevice::ModemInterface)) {
                modemNetworkIface = modem->interface(ModemManager::ModemDevice::ModemInterface).objectCast<ModemManager::Modem>();
            }
            if (modemNetworkIface) {
                connect(ModemManager::notifier(), SIGNAL(modemRemoved(QString)), this, SLOT(modemRemoved(QString)));
            }
        }
    }

    return modemNetworkIface;
}

void NetworkManager::ModemDevice::modemRemoved(const QString &modemUdi)
{
    Q_D(ModemDevice);
    if (modemUdi == d->m_modemUdi) {
        modemNetworkIface.clear();
        modemSimCardIface.clear();
    }
}

void NetworkManager::ModemDevice::setModemCardIface(const ModemManager::Sim::Ptr &iface)
{
    modemSimCardIface = iface;
}

void NetworkManager::ModemDevice::clearModemCardIface()
{
    modemSimCardIface.clear();
}

void NetworkManager::ModemDevice::setModemNetworkIface(const ModemManager::Modem::Ptr& iface)
{
    modemNetworkIface = iface;
}

void NetworkManager::ModemDevice::clearModemNetworkIface()
{
    modemNetworkIface.clear();
}
#endif

// vim: sw=4 sts=4 et tw=100

#include "modemdevice.moc"
