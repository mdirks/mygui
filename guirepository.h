//
// C++ Interface: guirepository
//
// Description: 
//
//
// Author: Marcus Dirks <mopp@suse>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUIREPOSITORY_H
#define GUIREPOSITORY_H

#include <qstring.h>
#include <qwidget.h>
//#include <kiconloader.h>
//#include <kmainwindow.h>

#include <persistence/pobject.h>
#include <mapping/abstractmapper.h>
#include <repository/repositoryproperty.h>
#include "propertyeditor.h"
#include "pobjecticonview.h"
//#include "gui/mapviews/sitzplanmapview.h"
//#include "datamodel/stundenplan.h"
//#include "splashscreen.h"
#include "guicontroler.h"
#include "guimode.h"
#include "guiservice.h"
#include "splashscreen.h"
#include "guimainwindow.h"
#include "movieplayer.h"

//#include <kglobal.h>
//#include <KConfig>
#include <KConfig>
#include <KIconLoader>
#include <KIconDialog>
//#include <ktrader.h>
//#include <klibloader.h>
#include <KParts/Part>
#include <kmessagebox.h>
#include <KXmlGuiWindow>

#include <QStackedWidget>
#include <QMainWindow>
#include <QtMultimediaWidgets/QVideoWidget>

/** The main GUI Controller Singleton (Startup, global services, editing controll)
@author Marcus Dirks
*/
class GuiRepository :public GuiControler {

Q_OBJECT


public slots:
    virtual void showForm(QWidget *w)=0;
    virtual void showDialog(QWidget *form)=0;
    virtual void showFormForObject(PObject *o, bool top=true)=0;
    virtual void showFormForObject(PObject *o, list<RepositoryProperty*> *properties, bool top=true)=0;
    virtual void showFormForObject(PObject *o, QWidget *form, bool top=true)=0;
	virtual void showDialogForObject(PObject *o,list<RepositoryProperty*> *properties=0)=0;
    virtual void showCentralWidget(QWidget *w)=0;




public:
    virtual QWidget* getFormForObject(PObject *o,QWidget *parent=0, list<RepositoryProperty*> *properties=0)=0;
    //virtual SitzplanMapView* getMapViewForSitzplan(sitzplan *sp)=0;
    virtual void setSelectedObject(PObject *o)=0;
    virtual QStackedWidget* getCentralWidget()=0;
    virtual void setCentralWidget(QStackedWidget *sw)=0;
    virtual void setActiveMode(GuiMode *mode)=0;
   virtual void showEditorForProperty(PObject *o,RepositoryProperty *rp)=0;
   virtual void addTool(QWidget *tool,
                         QString label,
                         QString short_label,
                         Qt::DockWidgetArea area = Qt::RightDockWidgetArea)=0;
   virtual void addIconView(PObjectIconView *iv, QString label, QString short_label)=0;
    virtual void addMode(GuiMode *mode)=0;
    virtual void addService(GuiService *serv)=0;
   virtual void addGlobalAction(QAction *ac)=0;
   virtual QToolBar* getGlobalToolbar()=0;

    virtual void showMovie(QString movieFile)=0;
    virtual void registerMoviePlayer(MoviePlayer *vw)=0;

    virtual PObjectIconViewItemTemplate* getPObjectIconViewItemTemplate(QString clName,QListWidget *lv)=0;
    virtual void registerPObjectIconViewItemTemplate(QString clName, PObjectIconViewItemTemplate *it)=0;


   virtual QMenu* getPopup(QString identifier) = 0;
   
   static GuiRepository* getInstance();
   static SplashScreen* showSplashScreen();
   static  void hideSplashScreen();

   virtual void initGui()=0;
   virtual list<AbstractMapper*>* getMapViews() = 0;


    


    virtual QMainWindow* getMainFrame() = 0;
    virtual void setMainFrame(GuiMainWindow *mf) = 0;

    virtual QWidget* getEditor(PObject *o, RepositoryProperty *rp, QWidget *parent=0)=0;


    virtual list<AbstractMapper*>* getPersistentClasses() = 0;
    //virtual stundenplan* getActiveStundenplan()=0;

    virtual void reloadGui() = 0;
    virtual void showInfo(QString cap, QString mes) = 0;
    virtual void closeGui() = 0;
 
    KParts::Part* getPdfPart(QWidget *parent);
    KParts::Part* getPart(QWidget *parent, QString qs1, QString qs2);
    virtual void close() = 0;
    virtual QString getDisplayString(PObject *po)=0;
    //virtual SitzplanMap*  getMapForSitzplan(sitzplan *sp)=0;

    virtual PObject* createObject()=0;
    virtual PObject* createObject(QStringList classList)=0;
    virtual AbstractMapper* chooseMapper()=0;
    virtual AbstractMapper* chooseMapper(QStringList classList)=0;
    virtual PObject* create(string className)=0;


private:
    /** */
    static GuiRepository* instance;
    static SplashScreen *splash;
};

#endif
