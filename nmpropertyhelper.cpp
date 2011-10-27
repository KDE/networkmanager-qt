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
