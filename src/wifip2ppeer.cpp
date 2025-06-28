/*
    SPDX-FileCopyrightText: 2025 Martin Rodriguez Reboredo <yakoyoku@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "wifip2ppeer.h"
#include "accesspoint.h"
#include "manager_p.h"
#include "wifip2ppeer_p.h"

#include "nmdebug.h"

using namespace Qt::Literals;

NetworkManager::WifiP2PPeerPrivate::WifiP2PPeerPrivate(const QString &path, WifiP2PPeer *q)
#ifdef NMQT_STATIC
    : iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::sessionBus())
#else
    : iface(NetworkManagerPrivate::DBUS_SERVICE, path, QDBusConnection::systemBus())
#endif
    , uni(path)
    , flags(AccessPoint::None)
    , lastSeen(-1)
    , strength(0)
    , q_ptr(q)
{
}

NetworkManager::AccessPoint::Capabilities NetworkManager::WifiP2PPeerPrivate::convertCapabilities(int caps)
{
    NetworkManager::AccessPoint::Capabilities capFlags;
    if (caps & NM_802_11_AP_FLAGS_PRIVACY) {
        capFlags |= AccessPoint::Privacy;
    }
    return capFlags;
}

NetworkManager::WifiP2PPeer::WifiP2PPeer(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(std::make_unique<WifiP2PPeerPrivate>(path, this))
{
    Q_D(WifiP2PPeer);

    QDBusConnection::systemBus().connect(NetworkManagerPrivate::DBUS_SERVICE,
                                         d->uni,
                                         NetworkManagerPrivate::FDO_DBUS_PROPERTIES,
                                         QLatin1String(),
                                         d,
                                         SLOT(dbusPropertiesChanged(QString, QVariantMap, QStringList)));

    // Retrieve all WifiP2PPeer's properties at once
    const QVariantMap initialProperties = NetworkManagerPrivate::retrieveInitialProperties(d->iface.staticInterfaceName(), path);
    if (!initialProperties.isEmpty()) {
        d->propertiesChanged(initialProperties);
    }
}

NetworkManager::WifiP2PPeer::~WifiP2PPeer() = default;

uint NetworkManager::WifiP2PPeer::flags() const
{
    Q_D(const WifiP2PPeer);
    return d->flags;
}

QString NetworkManager::WifiP2PPeer::hardwareAddress() const
{
    Q_D(const WifiP2PPeer);
    return d->hardwareAddress;
}

int NetworkManager::WifiP2PPeer::lastSeen() const
{
    Q_D(const WifiP2PPeer);
    return d->lastSeen;
}

QString NetworkManager::WifiP2PPeer::manufacturer() const
{
    Q_D(const WifiP2PPeer);
    return d->manufacturer;
}

QString NetworkManager::WifiP2PPeer::model() const
{
    Q_D(const WifiP2PPeer);
    return d->model;
}

QString NetworkManager::WifiP2PPeer::modelNumber() const
{
    Q_D(const WifiP2PPeer);
    return d->modelNumber;
}

QString NetworkManager::WifiP2PPeer::name() const
{
    Q_D(const WifiP2PPeer);
    return d->name;
}

QString NetworkManager::WifiP2PPeer::serial() const
{
    Q_D(const WifiP2PPeer);
    return d->serial;
}

uchar NetworkManager::WifiP2PPeer::strength() const
{
    Q_D(const WifiP2PPeer);
    return d->strength;
}

QByteArray NetworkManager::WifiP2PPeer::wfdIEs() const
{
    Q_D(const WifiP2PPeer);
    return d->wfdIEs;
}

void NetworkManager::WifiP2PPeerPrivate::dbusPropertiesChanged(const QString &interfaceName,
                                                               const QVariantMap &properties,
                                                               const QStringList &invalidatedProperties)
{
    Q_UNUSED(invalidatedProperties);
    if (interfaceName == "org.freedekstop.NetworkManager.WifiP2PPeer"_L1) {
        propertiesChanged(properties);
    }
}

void NetworkManager::WifiP2PPeerPrivate::propertiesChanged(const QVariantMap &properties)
{
    Q_Q(WifiP2PPeer);

    QVariantMap::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        const QString property = it.key();
        if (property == "Flags"_L1) {
            flags = convertCapabilities(it->toUInt());
            Q_EMIT q->flagsChanged(flags);
        } else if (property == "HwAddress"_L1) {
            hardwareAddress = it->toString();
            Q_EMIT q->hardwareAddressChanged(hardwareAddress);
        } else if (property == "LastSeen"_L1) {
            lastSeen = it->toInt();
            Q_EMIT q->lastSeenChanged(lastSeen);
        } else if (property == "Manufacturer"_L1) {
            manufacturer = it->toString();
            Q_EMIT q->manufacturerChanged(manufacturer);
        } else if (property == "Model"_L1) {
            model = it->toString();
            Q_EMIT q->modelChanged(model);
        } else if (property == "ModelNumber"_L1) {
            modelNumber = it->toString();
            Q_EMIT q->modelNumberChanged(modelNumber);
        } else if (property == "Name"_L1) {
            name = it->toString();
            Q_EMIT q->nameChanged(name);
        } else if (property == "Serial"_L1) {
            serial = it->toString();
            Q_EMIT q->serialChanged(serial);
        } else if (property == "Strength"_L1) {
            strength = it->value<uchar>();
            Q_EMIT q->strengthChanged(strength);
        } else if (property == "WfdIEs"_L1) {
            wfdIEs = it->toByteArray();
            Q_EMIT q->wfdIEsChanged(wfdIEs);
        } else {
            qCDebug(NMQT) << Q_FUNC_INFO << "Unhandled property" << property;
        }
        ++it;
    }
}

#include "moc_wifip2ppeer.cpp"
#include "moc_wifip2ppeer_p.cpp"
