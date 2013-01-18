/*
Copyright 2008,2010 Will Stephenson <wstephenson@kde.org>

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

// class that can extract values out of a properties map into a QObject's properties

#ifndef NMQT_PROPERTY_HELPER_H
#define NMQT_PROPERTY_HELPER_H

#include <QPair>

#include "generic-types.h"

class NMPropertyDeserializer
{
public:
    virtual ~NMPropertyDeserializer();
    virtual void deserialize(const QString& propertyName, const QVariant& value, QObject * recipient) = 0;
};

// deserializer for extracting a plain qvariant
struct GenericDeserializer : public NMPropertyDeserializer
{
public:
    virtual void deserialize(const QString& propertyName, const QVariant& value, QObject * recipient);
};

typedef QPair<char*, char*> PropertySignalPair;

struct NMPropertySpec
{
public:
    char * propertyName;
    char * signalName;
};

class NMPropertyHelper
{
public:
    NMPropertyHelper();
    NMPropertyHelper(QObject * master);
    ~NMPropertyHelper();
    void registerProperty(const QString &nmPropertyName, QPair<char*,char*> spec);
    void deserializeProperties(const QVariantMap &);
private:
    QObject * mMaster;
    QMap<QString, QPair<char*,char*> > mRegisteredProperties;
};
/*
class FrobObject : public QObject
{
Q_OBJECT
Q_PROPERTY(QString frobozz READ frobozz WRITE setFrobozz)
public:
    FrobObject();
    ~FrobObject();
    QString frobozz() const;
    void setFrobozz(const QString& frob);
signals:
    void frobozzChanged();
private:
    QString mFrobozz;
};
*/
#endif

