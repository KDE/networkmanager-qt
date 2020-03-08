/*
    SPDX-FileCopyrightText: 2011 Ilia Kats <ilia-kats@gmx.net>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef NETWORKMANAGERQT_SECRETAGENT_P_H
#define NETWORKMANAGERQT_SECRETAGENT_P_H

#include "agentmanagerinterface.h"
#include "secretagentadaptor.h"


namespace NetworkManager
{
class SecretAgent;
class SecretAgentPrivate
{
    Q_DECLARE_PUBLIC(SecretAgent)
public:
    explicit SecretAgentPrivate(const QString &, SecretAgent *parent = nullptr);
    virtual ~SecretAgentPrivate();
private Q_SLOTS:
    void dbusInterfacesAdded(const QDBusObjectPath &path, const QVariantMap &interfaces);
    void registerAgent();
private:
    SecretAgent *q_ptr;
    SecretAgentAdaptor agent;
    OrgFreedesktopNetworkManagerAgentManagerInterface agentManager;
    QString agentId;
};
}

#endif // NETWORKMANAGERQT_SECRETAGENT_P_H
