/*
    Copyright 2014 Jan Grulich <jgrulich@redhat.com>

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

#include "connection.h"

#include <QtDBus/QtDBus>

Connection::Connection(QObject *parent, const NMVariantMapMap &settings)
    : QObject(parent)
    , m_unsaved(false)
    , m_settings(settings)
{
    qDBusRegisterMetaType<NMVariantMapMap>();
}

Connection::~Connection()
{
}

bool Connection::unsaved() const
{
    return m_unsaved;
}

void Connection::Delete()
{
    // From some reason signal Removed is not send over DBus when using only Q_EMIT
    QDBusMessage message = QDBusMessage::createSignal(m_connectionPath, QLatin1String("org.kde.fakenetwork.Settings.Connection"), QLatin1Literal("Removed"));
    QDBusConnection::sessionBus().send(message);
    // Q_EMIT Removed();

    // Send it for FakeNetwork as well
    Q_EMIT connectionRemoved(QDBusObjectPath(m_connectionPath));
}

NMVariantMapMap Connection::GetSecrets(const QString &setting_name)
{
    // TODO
    return NMVariantMapMap();
}

NMVariantMapMap Connection::GetSettings()
{
    // TODO: return settings without secrets
    return m_settings;
}

void Connection::Save()
{
    // TODO
}

void Connection::Update(const NMVariantMapMap &properties)
{
    m_settings = properties;

    Q_EMIT Updated();
}

void Connection::UpdateUnsaved(const NMVariantMapMap &properties)
{
    // TODO
}

QString Connection::connectionPath() const
{
    return m_connectionPath;
}

void Connection::setConnectionPath(const QString &path)
{
    m_connectionPath = path;
}
