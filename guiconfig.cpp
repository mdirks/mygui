//
// C++ Implementation: config
//
// Description: 
//
//
// Author: Marcus Dirks <marcus.dirks@web.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "guiconfig.h"

#include <persistence/database.h>
//#include "services/sstundenplan.h"
//#include "services/skalender.h"
//#include "datamodel/tutoriummapper.h"

//#include <kglobal.h>
#include <KConfig>
#include <KConfigGroup>
#include <kiconloader.h>
#include <kicondialog.h>

GuiConfig* GuiConfig::instance=0;

GuiConfig* GuiConfig::getInstance()
{
	if(! instance){
		instance = new GuiConfig();
	}
	return instance;
}

GuiConfig::GuiConfig()
{

    //KConfig *config = KGlobal::config();
    //config->setGroup("Gui");

    KConfig config("stundenplanerrc");
    config.group("Gui");


}


GuiConfig::~GuiConfig()
{
}

QString GuiConfig::readEntry(QString group, QString name)
{	
    /*
    KConfig *config = KGlobal::config();
	config->setGroup(group);
	return config->readEntry(name,"");
    */
    KConfig config("stundenplanerrc");
    return config.group(group).readEntry(name,"");

}
   
void GuiConfig::writeEntry(QString group, QString name, QString entry)
{
    /*
    KConfig *config = KGlobal::config();
	config->setGroup(group);
	config->writeEntry(name, entry);
    */
    KConfig config("stundenplanerrc");
    config.group(group).writeEntry(name, entry);

}


QPixmap GuiConfig::loadIcon(QString iconName, KIconLoader::Group group)
{
    KIconLoader *loader = KIconLoader::global();
	return loader->loadIcon(iconName, group);
}




QPixmap GuiConfig::getIcon(QString name, KIconLoader::Group group)
{
    QPixmap pm;
    KIconLoader *loader = KIconLoader::global();
	map<QString,QString>::iterator it = mapIcons.find(name);
	if(it != mapIcons.end()){
		QString iconName = it->second;
        pm= loader->loadIcon(iconName, group);
    } else {
        QString iconName = GuiConfig::getInstance()->readEntry("Gui",name + "_icon");
		mapIcons[name]=iconName;
        pm= loader->loadIcon(iconName,group);
	}
    if(pm.isNull()){
        selectIcon(name);
        it = mapIcons.find(name);
        if(it != mapIcons.end()){
            QString iconName = it->second;
            pm= loader->loadIcon(iconName, group);
        }
    }
    return pm;

}
/*!
    \fn GuiRepositoryImpl::getIconForObject(PObject *o)
 */
QPixmap GuiConfig::getIcon(PObject *o, KIconLoader::Group group)
{
    KIconLoader *loader = KIconLoader::global();
    if(o && o->getPersistenceObject()){
    QString className(o->getPersistenceObject()->getClassName().c_str());
	return getIcon(className,group);
    } else {
        qDebug("GuiConfig::getIcon: Warning: getPersistenceObject failed for object");
    	return getIcon("kfind",group);//loader->loadIcon("kfind",group);
    }
}

QPixmap GuiConfig::getIcon(RepositoryProperty *rp, KIconLoader::Group group)
{
	if(rp->isCollection()){
		return getIcon("Collection", group);
	} else if (rp->isString()){
		return getIcon("String", group);
	} else if (rp->isText()){
		return getIcon("Text", group);
	} else {
		return getIcon("Unknown", group);
	}
}



void GuiConfig::selectIcon(PObject *o)
{
    QString className(o->getPersistenceObject()->getClassName().c_str());
	selectIcon(className);
}

void GuiConfig::selectIcon(QString name)
{
    QString iconName = KIconDialog::getIcon(KIconLoader::Desktop,KIconLoader::Application,false,0,false,0,name);
	mapIcons[name] = iconName;
	
    KConfig config("stundenplanerrc");
    config.group("Gui").writeEntry(name+"_icon", iconName);
}


void GuiConfig::selectIcon(RepositoryProperty *rp)
{
	QString type;
	if(rp->isCollection()){
		type = "Collection";
	} else if (rp->isString()){
		type = "String";
	} else if (rp->isText()){
		type = "Text";
	} else {
		type = "Unknown";
	}
	
	selectIcon(type);
}

/*
QWidget* GuiRepositoryImpl::getEditor(PObject *mo, RepositoryProperty *prop, QWidget *mainTab)
{
	QWidget *w = 0;
	if(prop->isString()){
		w= new StringEditor(mo,prop,mainTab);
	} else if(prop->isText()){
					

					w =  new TextPropertyEditor(mo,prop,mainTab);
					//w->setPaletteBackgroundColor(QColor(220,220,250));
			} else if(prop->isCollection()){
					if(prop->getName() == "Eintraege"){
						PObjectTable *table = new PObjectTable(prop,mo,mainTab);
						table->load();
						w=table;
					} else {
						PObjectIconView *iconView = new PObjectIconView(prop,mo,mainTab);
						iconView->load();
						//iconView -> load(prop->asCollection( mo ) );
						w = iconView;
					}
			} else if(prop->isDate()){
					w=new DateEditor(mo,prop,mainTab) ;
			} else if(prop->isPObject()){
					w= new PObjectLabel(prop,mo,mainTab);
			} else if(prop->isNumeric()){
					w= new StringEditor(mo,prop,mainTab);
			}	
			return w;
}
*/





/*!
    \fn GuiConfig::getDatabaseName()
 */
QString GuiConfig::getDatabaseName()
{
    return readEntry("persistence", "databasename");
}


/*!
    \fn GuiConfig::setDatabaseName(QString dbName)
 */
void GuiConfig::setDatabaseName(QString dbName)
{
    writeEntry("persistence","databasename",dbName);
}


/*!
    \fn GuiConfig::setDisplayProperties(QString guiObjectIdentifier, QStringList properties)
 */
void GuiConfig::setDisplayProperties(QString guiObjectIdentifier, QStringList properties)
{
    writeEntry("displayproperties",guiObjectIdentifier, properties.join(","));
}


/*!
    \fn GuiConfig::getDisplayProperties(QString guiObjectIdentifier)
 */
QStringList GuiConfig::getDisplayProperties(QString guiObjectIdentifier)
{
    QString propertiesString = readEntry("displayproperties",guiObjectIdentifier);
    if(propertiesString==""){
        return QStringList();
    } else {
        return propertiesString.split(",");
    }
}


