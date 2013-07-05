#include <QtCore/QObject>
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

#include <QtCore/QString>
#include "nmpropertyhelper.h"

FrobObject::FrobObject()
    : QObject(0)
{
}
FrobObject::~FrobObject()
{
}

QString FrobObject::frobozz() const
{
    return mFrobozz;
}

void FrobObject::setFrobozz(const QString &frob)
{
    mFrobozz = frob;
}

int main(int argc, char **argv)
{
    FrobObject obj;
    NMPropertyHelper hlp(&obj);

    QPair<char *, char *> spec;
    spec.first = "frobozz";
    spec.second = "frobozzChanged";
    hlp.registerProperty("frobozz", spec);
    hlp.registerProperty("othername", QPair<char *, char *>("myname", 0));

    QVariantMap map;
    map.insert(QString("frobozz"), QVariant(42.0));
    map.insert(QString("othername"), QVariant("pas"));
    hlp.deserializeProperties(map);
    nmDebug("Hello mom");
    nmDebug() << "map keys: " <<  map.keys();
    nmDebug() << "qobj frobozz property (names match): " << obj.property("frobozz");
    nmDebug() << "qobj myname property (mapped from othername): " << obj.property("myname");
    return 0;
}
