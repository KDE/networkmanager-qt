/*
    SPDX-FileCopyrightText: 2025 Martin Rodriguez Reboredo <yakoyoku@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_WIFIP2PDEVICE_H
#define NETWORKMANAGERQT_WIFIP2PDEVICE_H

#include <networkmanagerqt/networkmanagerqt_export.h>

#include "device.h"
#include "wifip2ppeer.h"

#include <QVariantMap>

namespace NetworkManager
{
class WifiP2PDevicePrivate;

class NETWORKMANAGERQT_EXPORT WifiP2PDevice : public Device
{
    Q_OBJECT

    Q_PROPERTY(QString hardwareAddress READ hardwareAddress NOTIFY hardwareAddressChanged)

    Q_PROPERTY(QStringList peers READ peers NOTIFY peersChanged)

public:
    using Ptr = QSharedPointer<WifiP2PDevice>;

    explicit WifiP2PDevice(const QString &path, QObject *parent = nullptr);
    ~WifiP2PDevice() override;

    Type type() const override;

    QString hardwareAddress() const;
    QStringList peers() const;

    Q_INVOKABLE WifiP2PPeer::Ptr findPeer(const QString &uni);

public Q_SLOTS:
    QDBusPendingReply<> startFind(const QVariantMap &options = {});

    QDBusPendingReply<> stopFind();

Q_SIGNALS:
    void hardwareAddressChanged(const QString &hardwareAddress);
    void peersChanged(const QStringList &peers);
    void peerAppeared(const QString &peer);
    void peerDisappeared(const QString &peer);

private:
    Q_DECLARE_PRIVATE(WifiP2PDevice)
};

}

#endif /* NETWORKMANAGERQT_WIFIP2PDEVICE_H */
