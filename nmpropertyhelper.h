// class that can extract values out of a properties map into a QObject's properties

#ifndef NM_PROPERTY_HELPER_H
#define NM_PROPERTY_HELPER_H

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

