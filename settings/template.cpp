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

#include "template.h"
#include "template_p.h"

// TODO: Uncomment and replace the template with the setting that you want to implement
//#include <nm-setting-template.h>

#include <QtCore/QDebug>

NetworkManager::Settings::TemplateSettingPrivate::TemplateSettingPrivate():
    name(QString("template"))
{ }

NetworkManager::Settings::TemplateSetting::TemplateSetting():
    /* TODO: Uncomment and replace the template with the setting that you want to implement
     *       This setting must also be added into the enum in base class
     */
    //Setting(Setting::Template),
    d_ptr(new TemplateSettingPrivate())
{ }

NetworkManager::Settings::TemplateSetting::TemplateSetting(TemplateSetting * setting):
    Setting(setting),
    d_ptr(new TemplateSettingPrivate())
{
    /*
     * setFoo(setting->foo());
     *
     */
}

NetworkManager::Settings::TemplateSetting::~TemplateSetting()
{
    delete d_ptr;
}

QString NetworkManager::Settings::TemplateSetting::name() const
{
    Q_D(const TemplateSetting);

    return d->name;
}

void NetworkManager::Settings::TemplateSetting::fromMap(const QVariantMap& setting)
{
    /*
     * if (setting.contains(QLatin1String(NM_SETTING_TEMPLATE_FOO))) {
     *     setFoo(setting.value(QLatin1String(NM_SETTING_TEMPLATE_FOO)));
     * }
     *
     */
}

QVariantMap NetworkManager::Settings::TemplateSetting::toMap() const
{
    QVariantMap setting;

    /*
     * if (!foo.isEmpty()) {
     *     setting.insert(QLatin1String(NM_SETTING_TEMPLATE_FOO), foo());
     * }
     *
     */

    return setting;
}

void NetworkManager::Settings::TemplateSetting::printSetting()
{
    NetworkManager::Settings::Setting::printSetting();

    /*
     * qDebug() << "FOO - " << foo();
     *
     */
}
