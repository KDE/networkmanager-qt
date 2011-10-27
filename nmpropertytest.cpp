#include <QtCore/QObject>
#include <QtCore/QString>
#include "nmpropertyhelper.h"

FrobObject::FrobObject() : QObject(0)
{
}
FrobObject::~FrobObject()
{
}

QString FrobObject::frobozz() const
{
    return mFrobozz;
}

void FrobObject::setFrobozz(const QString& frob)
{
    mFrobozz = frob;
}

int main(int argc, char**argv)
{
    FrobObject obj;
    NMPropertyHelper hlp(&obj);

    QPair<char*,char*> spec;
    spec.first = "frobozz";
    spec.second = "frobozzChanged";
    hlp.registerProperty("frobozz", spec);
    hlp.registerProperty("othername", QPair<char*,char*>("myname", 0));

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
