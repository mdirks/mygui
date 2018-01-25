//
// C++ Interface: config
//
// Description: 
//
//
// Author: Marcus Dirks <marcus.dirks@web.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUICONFIG_H
#define GUICONFIG_H

#include <repository/repositoryproperty.h>
#include <persistence/pobject.h>

#include <QString>
#include <QPixmap>
#include <QWidget>
#include <kiconloader.h>
//#include <string>
#include <map>

using namespace std;
/**
	@author Marcus Dirks <marcus.dirks@web.de>
*/
class GuiConfig{
public:
    GuiConfig();
    ~GuiConfig();

   static GuiConfig* getInstance();

   QString readEntry(QString group, QString name);
   void writeEntry(QString group, QString name, QString entry);


   QPixmap loadIcon(QString iconName, KIconLoader::Group group=KIconLoader::Desktop);

    QPixmap getIcon(RepositoryProperty *rp, KIconLoader::Group group=KIconLoader::Desktop) ;
    QPixmap getIcon(PObject *o, KIconLoader::Group group=KIconLoader::Desktop);
    QPixmap getIcon(QString className, KIconLoader::Group group=KIconLoader::Desktop) ;

    void selectIcon(QString name); 
    void selectIcon(PObject *o);
    void selectIcon(RepositoryProperty *rp);

    QString getDatabaseName();
    void setDatabaseName(QString dbName);

    QStringList getDisplayProperties(QString guiObjectIdentifier);
    void setDisplayProperties(QString guiObjectIdentifier, QStringList properties);

   //QWidget* getEditor(PObject *o, RepositoryProperty *rp, QWidget *parent=0);

private:
	static GuiConfig *instance;
	map<QString, QString> mapIcons;

};

#endif
