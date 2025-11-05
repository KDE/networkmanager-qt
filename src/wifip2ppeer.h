/*
    SPDX-FileCopyrightText: 2025 Martin Rodriguez Reboredo <yakoyoku@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_WIFIP2PPEER_H
#define NETWORKMANAGERQT_WIFIP2PPEER_H

#include "accesspoint.h"
#include <networkmanagerqt/networkmanagerqt_export.h>

#include <nm-version.h>

#include <QObject>
#include <QSharedPointer>
#include <QVariantMap>

namespace NetworkManager
{
class WifiP2PPeerPrivate;

/*!
 * \class NetworkManager::WifiP2PPeer
 * \inheaderfile NetworkManagerQt/WifiP2PPeer
 * \inmodule NetworkManagerQt
 *
 * \brief Represents a wifip2p peer.
 * \since 6.20
 */
class NETWORKMANAGERQT_EXPORT WifiP2PPeer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint flags READ flags NOTIFY flagsChanged)

    Q_PROPERTY(QString hardwareAddress READ hardwareAddress NOTIFY hardwareAddressChanged)

    Q_PROPERTY(int lastSeen READ lastSeen NOTIFY lastSeenChanged)

    Q_PROPERTY(QString manufacturer READ manufacturer NOTIFY manufacturerChanged)

    Q_PROPERTY(QString model READ model NOTIFY modelChanged)

    Q_PROPERTY(QString modelNumber READ modelNumber NOTIFY modelNumberChanged)

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)

    Q_PROPERTY(QString serial READ serial NOTIFY serialChanged)

    Q_PROPERTY(uchar strength READ strength NOTIFY strengthChanged)

    Q_PROPERTY(QByteArray wfdIEs READ wfdIEs NOTIFY wfdIEsChanged)

public:
    using Ptr = QSharedPointer<WifiP2PPeer>;

    explicit WifiP2PPeer(const QString &path, QObject *parent = nullptr);
    ~WifiP2PPeer() override;

    uint flags() const;

    QString hardwareAddress() const;

    int lastSeen() const;

    QString manufacturer() const;

    QString model() const;

    QString modelNumber() const;

    QString name() const;

    QString serial() const;

    uchar strength() const;

    QByteArray wfdIEs() const;

Q_SIGNALS:
    void flagsChanged(uint flags);

    void hardwareAddressChanged(const QString &hardwareAddress);

    void lastSeenChanged(int lastSeen);

    void manufacturerChanged(const QString &manufacturer);

    void modelChanged(const QString &model);

    void modelNumberChanged(const QString &modelNumber);

    void nameChanged(const QString &name);

    void serialChanged(const QString &serial);

    void strengthChanged(uchar strength);

    void wfdIEsChanged(const QByteArray &wfdIEs);

private:
    Q_DECLARE_PRIVATE(WifiP2PPeer)

    std::unique_ptr<WifiP2PPeerPrivate> d_ptr;
};

}

#endif /* NETWORKMANAGERQT_WIFIP2PPEER_H */
