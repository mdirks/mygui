 //  
 // C++ Interface: stundepersistence 
 //   
 // Description:  
 // 
 // 
 // Author: Marcus Dirks <mopp@suse>, (C) 2005 
 //  
 // Copyright: See COPYING file that comes with this distribution 
 // 
 // Written on So. Jan. 21 13:02:50 2018
// 
 #ifndef PObjectGraphicsItemMAPPER_H 
 #define PObjectGraphicsItemMAPPER_H 
 
 #include <persistence/pobject.h> 
 #include <mapping/mappedobject.h> 
#include <mapping/association.h> 
#include <mapping/murl.h> 
#include <persistence/persistenceclass.h> 
 #include "pobjectgraphicsitem.h" 
#include <persistence/pobject.h>
#include <mapping/abstractmapper.h>
 #include <persistence/variant.h>
 #include <repository/repositoryentryimpl.h>
 #include <repository/stringproperty.h>
 #include <repository/integerproperty.h>
 #include <repository/pobjectproperty.h>
 #include <repository/collectionpropertyimpl.h>
 #include <repository/numericproperty.h>
 #include <repository/dateproperty.h>
 #include <repository/booleanproperty.h>
 #include <repository/datetimeproperty.h>
 #include <repository/repositoryenabled.h>
 
 /** 
 @author Marcus Dirks 
 */ 
 class PObjectGraphicsItemmapper : public AbstractMapper, public RepositoryEnabled
{
 public:
     static PObjectGraphicsItemmapper* getInstance();
     ~PObjectGraphicsItemmapper();
     static PObjectGraphicsItem* create();

     string getTableName();
     string getClassName();
     string* getColumnTypes();
     string* getColumns();
     string* getValues(PObject* realSubject);
     int getColumnCount();
     void save();
     void save(PObject *realSubject);
     PObject* createNewObject();
     list<PObjectGraphicsItem*>* find();
    void init(PObject* o, Variant* res);
    RepositoryEntry *getRepositoryEntry();

PObject * findObject(int pri_id);

protected:
     PObjectGraphicsItemmapper();
 
 private:
    static PObjectGraphicsItemmapper* instance;
 

   string *columnTypes;
    string *columns;
   
 
 };
 
 #endif
 
