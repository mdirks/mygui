//
// C++ Implementation: guirepositoryimpl
//
// Description: 
//
//
// Author: Marcus Dirks <mopp@suse>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "guirepositoryimpl.h"
//#include "form_stunde_rt_impl.h"
//#include "form_opo.h"
#include "pobjecteditor3.h"
#include "splashscreenimpl.h"
#include <mapping/mappingcontroler.h>
#include <mapping/abstractmapper.h>
#include "pobjectgraphicsitemmapper.h"
#include "pobjecteditor.h"
//#include "pobjectlistbox.h"
//#include "typelistview.h"
#include "pobjecticonview.h"
#include "pobjecteditor2.h"
#include <transactions/transactions.h>
#include "propertyeditor.h"
#include "pobjecttable.h"
#include "guiconfig.h"
//#include "services/actions/createaction.h"
#include "pobjectdialog.h"
#include "dateeditor.h"
#include "pobjectlabel.h"
#include "stringeditor.h"
#include "textpropertyeditor.h"
#include "formspopupfactory.h"
#include "mapviewspopupfactory.h"
#include "guipopupfactory.h"
#include <repository/repositoryenabled.h>
#include "guiobjectfactoryimpl.h"
#include "pobjecttable.h"
//#include "gui/actions/pobjecttablecontrolerimpl.h"
//#include "gui/forms/lernkarteviewer.h"
//#include "datamodel/lernkarte.h"
#include "genericmapviewcontroler.h"
#include "guiobject.h"
#include "guiobjectmapper.h"
#include "editorbase.h"
#include "guimainwindow.h"
#include "pobjecttablecontrolerimpl.h"
#include "mapperview.h"
/*
#include "gui/actions/modeplanung.h"
#include "gui/actions/modelesen.h"
#include "gui/actions/modelesenmapper.h"
#include "gui/actions/modematerail.h"
#include "gui/actions/servicelatex.h"
#include "gui/actions/modelernen.h"
#include "gui/actions/servicexml.h"
#include "gui/actions/modenotes.h"
#include "gui/actions/servicepdf.h"
*/
#include "standardguimode.h"

#include <QPixmap>
#include <QMessageBox>
#include <QWidget>

#include <KXMLGUIFactory>
#include <KConfig>
#include <KIconLoader>
#include <KIconDialog>
#include <kmessagebox.h>
#include <KParts/Part>



GuiRepositoryImpl* GuiRepositoryImpl::impl_instance=0;
//static GuiRepositoryImpl::QMdiarea* formWorkspace=0;


GuiRepositoryImpl* GuiRepositoryImpl::getInstance()
{
	if(!impl_instance)
	{
		impl_instance=new GuiRepositoryImpl();
	}
	return impl_instance;
}


GuiRepositoryImpl::GuiRepositoryImpl()
 : GuiRepository(),std_icon_pixmap(16,16)
{
	formWorkspace=0;

    //stundeplaner = 0;
	listMappers = 0;
    //activeplan = 0;
    activeMode = 0;
    videow=0;

	
	
	activeEditors = new list<PropertyEditor*>();
	activeIconViews = new list<PObjectIconView*>();
    modelist = new list<GuiMode*>();
	
    /*
	listMappers = new list<AbstractMapper*>();
	listMappers->push_back(ThemaItemmapper::getInstance());
	listMappers->push_back(ThemaMapmapper::getInstance());
	listMappers->push_back(PObjectGraphicsItemmapper::getInstance());
	listMappers->push_back(PlatzGraphicsItemmapper::getInstance());
	listMappers->push_back(SchultagGraphicsItemmapper::getInstance());
	listMappers->push_back(DayMapmapper::getInstance());
        listMappers->push_back(WeekMapmapper::getInstance());
	listMappers->push_back(SitzplanMapmapper::getInstance());
	listMappers->push_back(GuiObjectmapper::getInstance());
    listMappers->push_back(ModeLesenmapper::getInstance());
	
	for(list<AbstractMapper*>::iterator it = listMappers->begin();
		it!=listMappers->end(); ++it)
	{
		AbstractMapper *mapper = *it;
		
		MappingControler::getInstance()->registerPersistentClass(mapper);
		RepositoryEnabled *re = dynamic_cast<RepositoryEnabled*>(*it);
		if(re){
			Repository::getInstance()->addRepositoryEntry(re->getRepositoryEntry());
		}
	}
    */
	
	listMapViews = new list<AbstractMapper*>();


	
	
}


GuiRepositoryImpl::~GuiRepositoryImpl()
{
}




/*!
    \fn GuiRepositoryImpl::showFormForObject(PObject *o)

     Given the object we get its class to create the appropriate widget. Appropriate widgets are hard coded.
	The generic case (PObject) is handeld by PObjectEditor2. The widget is then turned into a KMdiChildView

			stundeplaner->createWrapper(form, className, className);

	and added to the Mainframe.

			stundeplaner->addWindow(childView);
    
 */
QWidget* GuiRepositoryImpl::getFormForObject(PObject *o, QWidget *parent, list<RepositoryProperty*> *properties)
{
	string className;
	if(o->getPersistenceObject()){
		className = o->getPersistenceObject()->getClassName();
	} else {
		qWarning("GuiRepositoryImp::getFormForObject: Invalid objet given given");
		return 0;
	}


    PObjectEditor3 *editor = new PObjectEditor3(o,parent, properties);
    editor->setToolTip(QString("%1: %2").arg(o->getClassName().c_str()).arg(o->getName().c_str()));
    Transactions::getCurrentTransaction() -> add(o);

    editor->setWindowIcon(GuiConfig::getInstance()->getIcon(className.c_str()));
    return editor;
}



void GuiRepositoryImpl::showForm(QWidget *w){
    if(activeMode){
        activeMode->showForm(w);
    }
}

void GuiRepositoryImpl::showFormForObject(PObject *o, bool top){
    QWidget *form = getFormForObject(o, getMainFrame());
    showFormForObject(o,form,top);
}

void GuiRepositoryImpl::showFormForObject(PObject *o, list<RepositoryProperty*> *properties, bool top){

	QWidget *form = getFormForObject(o, getMainFrame());
    showFormForObject(o,form,top);
}

void GuiRepositoryImpl::showFormForObject(PObject *o, QWidget *form, bool top){
    if(form !=0){

        string objectName = o-> getName();
		string className = o->getPersistenceObject()->getClassName();
		
        if(activeMode){
            activeMode->showForm(form);
        }
	} else { qDebug("GuiRepositoryImp: could not get a valid form");}
	
}

PObjectIconViewItemTemplate* GuiRepositoryImpl::getPObjectIconViewItemTemplate(QString clName, QListWidget *lv)
{
    PObjectIconViewItemTemplate *tem=0;
    map<QString,PObjectIconViewItemTemplate*>::iterator it=mapTemplates.find(clName);
    if(it!=mapTemplates.end()){
            tem=it->second;
            if(tem){
                tem=tem->getInstanceFor(lv);
            }
    }
    return tem;
}


void GuiRepositoryImpl::registerPObjectIconViewItemTemplate(QString clName, PObjectIconViewItemTemplate *it)
{
    mapTemplates[clName]=it;

}


void GuiRepositoryImpl::showDialogForObject(PObject *o, list<RepositoryProperty*> *properties)
{
	
    EditorBase *dialog = new EditorBase(getMainFrame());
	QWidget *form=getFormForObject(o,dialog,properties);
	

	if(form){
        showDialog(form);
    } else {
		qWarning("GuiRepositoryImpl::showDialogForObject: Could not get form for object");
	}
}

void GuiRepositoryImpl::showDialog(QWidget *form)
{
    if(form){
        EditorBase *dialog = new EditorBase(getMainFrame());
        dialog->setMainWidget(form);
        dialog->exec();
    } else {
        qWarning("GuiRepositoryImpl::showDialog: Zero form");
    }
}

void GuiRepositoryImpl::showCentralWidget(QWidget *w)
{


}

void GuiRepositoryImpl::registerVideoWidget(QVideoWidget *vw)
{
    videow=vw;
}



void GuiRepositoryImpl::showMovie(QString movieFile)
{
    if(videow){
        if(!player){
            player = new QMediaPlayer();
        }
        player->setMedia(QUrl::fromLocalFile(movieFile));
        player->setVideoOutput(videow);

        player->play();
    }
}



void GuiRepositoryImpl::showEditorForProperty(PObject *o,RepositoryProperty *rp)
{

    //Transactions::getCurrentTransaction()->add(o); // hack because some editors do not commit properly

    if(rp){
        if(rp->isPObject()){
            showFormForObject(rp->asPObject(o));
        } else {
            EditorBase *dialog = new EditorBase(getMainFrame());
            QWidget *form=0;

            if(rp->isString()){
                StringEditor *se= new StringEditor(o,rp,dialog);
                connect(dialog,SIGNAL(okClicked()),se,SLOT(stopEdit()));
                form=se;
            } else if(rp->isText()){
                TextPropertyEditor *pe = new TextPropertyEditor(o,rp,dialog);
                connect(dialog,SIGNAL(okClicked()),pe,SLOT(stopEdit()));
                form=pe;
            } else if(rp->isNumeric()){
                form = new StringEditor(o,rp,dialog);
            } else if(rp){
                form = new QLabel(QString("%1 : unknown editor type").arg(rp->getName().c_str()),dialog);
            } else {
                form = new QLabel(QString("empty property"),dialog);
            }

            dialog->setMainWidget(form);
            dialog->exec();
        }
    }

}
/*!
    \fn GuiRepositoryImpl::setFormWorkspace(QMdiarea *w)
 */
void GuiRepositoryImpl::setFormWorkspace(QMdiArea *w)
{
    formWorkspace=w;
}





/*!
    \fn GuiRepositoryImpl::setGraphicsView(QGraphicsView *GraphicsView)
 */
void GuiRepositoryImpl::setGraphicsView(QGraphicsView *GraphicsView)
{
    this->GraphicsView  = GraphicsView;
}


/*!
    \fn GuiRepositoryImpl::initGui()
 */
void GuiRepositoryImpl::initGui()
{
    mainFrame = new GuiMainWindow();

    //rightToolBar = new QToolBar(stundeplaner);
    //stundeplaner->addToolBar(Qt::RightToolBarArea,rightToolBar);
    modeToolBar = new QToolBar(mainFrame);
    mainFrame->addToolBar(Qt::LeftToolBarArea,modeToolBar);
    serviceToolBar = new QToolBar(mainFrame);
    mainFrame->addToolBar(Qt::LeftToolBarArea,serviceToolBar);


    //setMainFrame(stundeplaner);
    setCentralWidget(mainFrame->getCentralWidget());
    mainFrame->show();
     

	GuiControler::setInstance(this);
	GuiObjectFactory::setInstance(new GuiObjectFactoryImpl());
	FormsPopupFactory::setInstance(GuiPopupFactory::getInstance());
	MapviewsPopupFactory::setInstance(GuiPopupFactory::getInstance());
	PObjectTableControlerFactory::setInstance(new PObjectTableControlerFactoryImpl());

    /*
    addMode(ModePlanung::getInstance());
    addService(ServiceLatex::getInstance());
    */
}

void GuiRepositoryImpl::setSelectedObject(PObject *o)
{
    if(activeMode){
        activeMode->activateObject(o);
    }
}

void GuiRepositoryImpl::setActiveMode(GuiMode *mode)
{
    if(activeMode != mode){
        if(activeMode) activeMode->tearDownMode();
        activeMode=mode;
        mode->setupMode();
    }
}

void GuiRepositoryImpl::addMode(GuiMode *mode)
{
    modelist->push_back(mode);
    modeToolBar->addAction(mode);

}

void GuiRepositoryImpl::addService(GuiService *serv)
{
    //stundeplaner->addMode(mode);
    GuiServiceToolButton *tb = new GuiServiceToolButton(serv,serviceToolBar);
    tb->setIcon(serv->icon());
    serviceToolBar->addWidget(tb);

}


void GuiRepositoryImpl::addGlobalAction(QAction *ac)
{
    getMainFrame()->getGlobalToolbar()->addAction(ac);

}

QToolBar* GuiRepositoryImpl::getGlobalToolbar()
{
    return getMainFrame()->getGlobalToolbar();
}

/*
KParts::Part* GuiRepositoryImpl::getPdfPart(QWidget* parent)
{	
	return getPart(parent, "application/pdf", "'KParts/ReadOnlyPart' in ServiceTypes");
}
*/

/*
KParts::Part* GuiRepositoryImpl::getPart(QWidget* parent, QString mimetype, QString qs2)
{
    KParts::Part* part=0;

    part = KParts::ComponentFactory::createPartInstanceFromQuery<KParts::ReadOnlyPart>(
                mimetype, QString(), parent);

    QString message("Found %1");
    if(part){
        message=message.arg("part");
    } else {
        message=message.arg("nothing");
    }


	KMessageBox::error(getMainFrame(),message);
	



    return part;
}
*/

void GuiRepositoryImpl::addIconView(PObjectIconView *iv, QString label, QString short_label)
{
	activeIconViews->push_back(iv);
	addTool(iv,label,short_label);
	
}

void GuiRepositoryImpl::addTool(QWidget *tool,
                                QString label,
                                QString short_label,
                                Qt::DockWidgetArea area)
{
    //KMdiChildView *childView = stundeplaner->createWrapper(tool, label, short_label);
    getMainFrame()->addDockWindow( tool, short_label,area);
}

void GuiRepositoryImpl::addTypeView(list<PObject*>* list, QString label, QString short_label){
    qDebug() << "Warning: GuiRepositoryImpl::addTypeView not implemented";
    /*
    PObjectListBox *listBox = new PObjectListBox();
	listBox->addObjects(list);
	addTool(listBox,label,short_label);
    */
}

/*!
    \fn GuiRepositoryImpl::wrappWidget(QWidget *w)
 */
void GuiRepositoryImpl::wrappWidgetAndAdd(QWidget *pNewView)
{
    /*
    KMdiChildView* pMDICover = new KMdiChildView( pNewView->caption());
          QBoxLayout* pLayout = new QHBoxLayout( pMDICover, 0, -1, "layout");
          pNewView->reparent( pMDICover, QPoint(0,0));
          pLayout->addWidget( pNewView);
          pMDICover->setName( pNewView->name());
   	QString shortName = pNewView->caption();
   	int length = shortName.length();
  	 shortName = shortName.right(length - (shortName.findRev('/') +1));
   	pMDICover->setTabCaption( shortName);
   	pMDICover->setCaption(pNewView->caption());

   	int flags = KMdi::StandardAdd;
    */

   if(activeMode){
       activeMode->showForm( pNewView);
   }
}


/*!
    \fn GuiRepositoryImpl::getMapViews()
 */
list<AbstractMapper*>* GuiRepositoryImpl::getMapViews()
{
    return listMapViews;
}

/*

QPixmap GuiRepositoryImpl::loadIcon(QString iconName, KIcon::Group group)
{
	KIconLoader *loader = KGlobal::iconLoader();
	return loader->loadIcon(iconName, group);
}


QPixmap GuiRepositoryImpl::getIcon(QString name, KIcon::Group group)
{
	KIconLoader *loader = KGlobal::iconLoader();
	map<QString,QString>::iterator it = mapIcons.find(name);
	if(it != mapIcons.end()){
		QString iconName = it->second;
		return loader->loadIcon(iconName, group);
		qDebug(QString("Map: Found icon %1 for %2").arg(iconName).arg(name));
	} else {
		//KConfig *config = KGlobal::config();
		//config->setGroup("Gui");
		//QString iconName = config->readEntry(name.append("_icon"),"unknown");
		QString iconName = GuiConfig::getInstance()->readEntry("Gui",name.append("_icon"));
		mapIcons[name]=iconName;
		qDebug(QString("Config: Found icon %1 for %2").arg(iconName).arg(name));
		
		return loader->loadIcon(iconName,group);
	}
}

QPixmap GuiRepositoryImpl::getIcon(PObject *o, KIcon::Group group)
{
    KIconLoader *loader = KGlobal::iconLoader();
    if(o && o->getPersistenceObject()){
	QString className(o->getPersistenceObject()->getClassName());
	return getIcon(className,group);
    } else {
        qDebug("Warning: getPersistenceObject failed for object");
    	return loader->loadIcon("kfind",group);
    }
    
   
}

QPixmap GuiRepositoryImpl::getIcon(RepositoryProperty *rp, KIcon::Group group)
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


void GuiRepositoryImpl::selectIcon(PObject *o)
{
	QString className(o->getPersistenceObject()->getClassName());
	selectIcon(className);
}

void GuiRepositoryImpl::selectIcon(QString name)
{
	QString iconName = KIconDialog::getIcon();
	mapIcons[name] = iconName;
	
	KConfig *config = KGlobal::config();
	config->setGroup("Gui");
	config->writeEntry(name.append("_icon"), iconName);
}


void GuiRepositoryImpl::selectIcon(RepositoryProperty *rp)
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


void GuiRepositoryImpl::addActiveEditor(PropertyEditor *editor)
{
     	
	activeEditors->push_back(editor);
}



void GuiRepositoryImpl::stopEdit()
{
    if(activeEditors){
	for(list<PropertyEditor*>::iterator it = activeEditors->begin(); it != activeEditors->end(); it++){
		if((*it)){
			(*it)->stopEdit();
		} else {
			qDebug(QString("GuiRepositoryImpl::stopEdit : !!! Invalid editor in editor-list"));
		}
	}
   }
}
*/

QStackedWidget* GuiRepositoryImpl::getCentralWidget()
{
    return centralWidget;
}

void GuiRepositoryImpl::setCentralWidget(QStackedWidget *cW)
{
    centralWidget = cW;
}


/*!
    \fn GuiRepositoryImpl::getMainFrame()
 */
/*KXmlGuiWindow**/ GuiMainWindow* GuiRepositoryImpl::getMainFrame()
{
    return mainFrame;
}

void  GuiRepositoryImpl::setMainFrame(GuiMainWindow /*KXmlGuiWindow*/ *mf)
{
     this->mainFrame = mf;
}

/*!
    \fn GuiRepositoryImpl::getPopup(QString identifier)
 */

QMenu* GuiRepositoryImpl::getPopup(QString identifier)
{
    QMenu *popup = new QMenu();
    /*
    QWidget *w =0;
    if(getMainFrame()){
        QWidget *w = getMainFrame()->guiFactory()->container(identifier, getMainFrame());
        if(w){
            popup= static_cast<QMenu*>(w);
        }
    }

    if(!popup){
        qDebug() << "Failed to get widget from xml";
	}
    */
	return popup;
}


/*!
    \fn GuiRepositoryImpl::getPersistentClasses()
 */
list<AbstractMapper*>* GuiRepositoryImpl::getPersistentClasses()
{
    return getMapViews();
}




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

void GuiRepositoryImpl::reloadGui()
{
	for(list<PObjectIconView*>::iterator it = activeIconViews->begin(); it!=activeIconViews->end(); it++)
	{
		(*it)->clear();
		(*it)->load();
	}
}



/*!
    \fn GuiRepositoryImpl::showInfo(QString msg)
 */
void GuiRepositoryImpl::showInfo(QString cap, QString mesg)
{
    QMessageBox::warning(getMainFrame(),cap,mesg,QMessageBox::Ok,QMessageBox::Ignore );
}


/*!
    \fn GuiRepositoryImpl::closeGui()
 */
void GuiRepositoryImpl::closeGui()
{
    for(list<GuiMode*>::iterator it = modelist->begin(); it!=modelist->end(); it++)
    {
        (*it)->close();
    }
}

bool GuiRepositoryImpl::versionChangeRequested(string className)
{
    int ans = QMessageBox::question(getMainFrame(),"Version change",
              QString("Really change version of %1 in database - stored data could be lost").arg(className.c_str()),
                                    QMessageBox::No, QMessageBox::Yes, QMessageBox::NoButton);

	if(ans==QMessageBox::Yes)
		return true;
	else return false;
}


/*!
    \fn GuiRepositoryImpl::close()
 */
void GuiRepositoryImpl::close()
{
    /// @todo implement me
}





/*!
    \fn GuiRepositoryImpl::getDisplayString(PObject *)
 */
QString GuiRepositoryImpl::getDisplayString(PObject *po)
{
    QString dS("N t d");

    return dS;
}

PObject*  GuiRepositoryImpl::createObject()
{
    PObject *res=0;
    AbstractMapper *mapper = chooseMapper();

    if(mapper){
        res = create(mapper->getClassName());
    }

    return res;
}


PObject*  GuiRepositoryImpl::createObject(QStringList classList)
{
    PObject *res=0;
    AbstractMapper *mapper = chooseMapper(classList);


    if(mapper){
        res = create(mapper->getClassName());
    }
    return res;
}

AbstractMapper* GuiRepositoryImpl::chooseMapper()
{
    return MapperDialog::chooseMapper();
}


AbstractMapper* GuiRepositoryImpl::chooseMapper(QStringList classList)
{
    return MapperDialog::chooseMapper(classList);
}



/*!
    \fn CreateAction::create(string className)
 */
PObject* GuiRepositoryImpl::create(string className)
{
    PObject *o=0;

    AbstractMapper *mapper = MappingControler::getInstance()->getMapperByName(className);
    if(mapper){
            o=mapper->create();
    } else {
        qDebug() << QString("Could not get mapper for %1").arg(className.c_str());
    }
    return o;
}



