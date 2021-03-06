#ifndef POBJECTLISTPROVIDER_H
#define POBJECTLISTPROVIDER_H

#include <repository/repositoryproperty.h>
#include <persistence/pobject.h>
#include <mapping/abstractmapper.h>

#include <QString>
#include <QDebug>
#include <list>

class PObjectListProvider
{
public:
    virtual list<PObject*> *objectList()=0;
    virtual void addObject(PObject *o)=0;
    virtual QString getClassName()=0;
    PObject* addNewObject();
    virtual void deleteObject(PObject *o)=0;
    virtual void setParentObject(PObject *o){qDebug()<<"PObjectListProvider: setParentObject empty";}
};

class PoLListProvider : public PObjectListProvider
{
public:
    PoLListProvider(list<PObject*> *olist, QString className=QString());
    list<PObject*> *objectList();
    void addObject(PObject *o);
    QString getClassName();
    void deleteObject(PObject *o);

private:
    list<PObject*> *olist;
    QString className;
};

class RpListProvider : public PObjectListProvider
{
public:
    RpListProvider(RepositoryProperty *rp, PObject *parentObject=0);
    list<PObject*> *objectList();
    void addObject(PObject *o);
    QString getClassName();
    void deleteObject(PObject *o);
    void setParentObject(PObject *po);

private:
    PObject *parentObject;
    RepositoryProperty *rp;

};

class MapperListProvider : public PObjectListProvider
{
public:
    MapperListProvider(AbstractMapper *mapper);
    MapperListProvider(QString className);
    list<PObject*> *objectList();
    void addObject(PObject *o);
    QString getClassName();
    void deleteObject(PObject *o);

private:
    AbstractMapper *mapper;


};


#endif // POBJECTLISTPROVIDER_H
