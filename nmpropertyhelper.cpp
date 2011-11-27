/*
Copyright 2008,2010 Will Stephenson <wstephenson@kde.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of
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

#include "nmpropertyhelper.h"

NMPropertyHelper::NMPropertyHelper(QObject *master) : mMaster(master)
{
}

NMPropertyHelper::~NMPropertyHelper()
{
}

void NMPropertyHelper::registerProperty(const QString & nmPropertyName, QPair<char*, char*> propSpec)
{
    mRegisteredProperties.insert(nmPropertyName, propSpec);
}

void NMPropertyHelper::deserializeProperties(const QVariantMap& propertiesMap)
{
    QMapIterator<QString,QVariant> i(propertiesMap);
    while (i.hasNext()) {
        i.next();
        if (mRegisteredProperties.contains(i.key())) {
            QPair<char*,char*> spec = mRegisteredProperties.value(i.key());
            mMaster->setProperty(spec.first, i.value());
            //spec.deserializer->deserialize( spec.propertyName, i.value(), mMaster);
            // emit signal if one was registered
            if ( spec.second != 0 )
                QMetaObject::invokeMethod( mMaster, spec.second );
        }
    }
}

NMPropertyDeserializer::~NMPropertyDeserializer()
{
}

void GenericDeserializer::deserialize(const QString & propertyName, const QVariant & value, QObject * recipient)
{
    /*
    recipient->setProperty(propertyName, value);
        QMetaObject::invokeMethod(recipient, signalName);
    emit recipient->propertyChanged(propertyName);*/
}
