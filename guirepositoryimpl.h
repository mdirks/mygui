//
// C++ Interface: guirepositoryimpl
//
// Description: 
//
//
// Author: Marcus Dirks <mopp@suse>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUIREPOSITORYIMPL_H
#define GUIREPOSITORYIMPL_H

#include <list>

#include <QMdiArea>
#include <QGraphicsScene>
#include <QWidget>
#include <QString>
#include <QStackedWidget>
#include <QToolBar>
#include <QVBoxLayout>

#include <QApplication>
//#include <kmdichildview.h>
//#include <kmdimainfrm.h>
#include <kmainwindow.h>

#include "guirepository.h"
#include "splashscreen.h"
#include <persistence/pobject.h>
#include <mapping/abstractmapper.h>
#include <mapping/mappingeventlistener.h>
#include <repository/repositoryproperty.h>
#include "pobjecticonview.h"
#include "guicontroler.h"
#include "guimode.h"
#include "guimainwindow.h"
#include "movieplayer.h"

#include <QDialog>
#include <QMediaPlayer>

#include <list>
#include <map>

using namespace std;

/**
@author Marcus Dirks
*/
class GuiRepositoryImpl : public GuiRepository, public MappingEventListener
{
public:
    ~GuiRepositoryImpl();
    static GuiRepositoryImpl* getInstance();
    
    void showForm(QWidget *w);
    void showDialog(QWidget *w);
    void showFormForObject(PObject *o, bool top=false);
    void showFormForObject(PObject *o, list<RepositoryProperty*> *properties, bool top=true);
    void showFormForObject(PObject *o, QWidget *form, bool top=true);
    void showDialogForObject(PObject *o, list<RepositoryProperty*> *properties=0);
    void showEditorForProperty(PObject *o,RepositoryProperty *rp);
    void showCentralWidget(QWidget *w);
    QWidget* getFormForObject(PObject *o,QWidget *parent=0, list<RepositoryProperty*> *properties=0);
    //SitzplanMapView* getMapViewForSitzplan(sitzplan *sp);
    void setSelectedObject(PObject *o);
    void setFormWorkspace(QMdiArea *w);

    void showMovie(QString movieFile);
    void registerMoviePlayer(MoviePlayer *vw);

    void setGraphicsView(QGraphicsView *GraphicsView);
    void initGui();
    void addTool(QWidget *tool, QString label, QString short_label,Qt::DockWidgetArea area=Qt::RightDockWidgetArea);
    void addIconView(PObjectIconView *iv, QString label, QString short_label);
    //KParts::Part *getPdfPart(QWidget *parent);
    //KParts::Part *getPart(QWidget *parent, QString qs1, QString qs2);

    void addMode(GuiMode *mode);
    void addService(GuiService *serv);
    void addGlobalAction(QAction *ac);

    QToolBar* getGlobalToolbar();
    void setActiveMode(GuiMode *mode);

    QStackedWidget* getCentralWidget();
    void setCentralWidget(QStackedWidget *sw);

    /*KXmlGuiWindow*/ GuiMainWindow* getMainFrame();
    void setMainFrame(/*KXmlGuiWindow*/ GuiMainWindow *mf);
    //stundenplan* getActiveStundenplan();
    QWidget* getEditor(PObject *o, RepositoryProperty *rp, QWidget *parent=0);
     void reloadGui();
    void showInfo(QString cap, QString msg);
    void closeGui();

	bool versionChangeRequested(string className);
    void close();
    QString getDisplayString(PObject *);
    //QWidget* getFormForType(string className, QWidget *parent);
    //SitzplanMap*  getMapForSitzplan(sitzplan *sp);

    PObject* createObject();
    PObject* createObject(QStringList classList);
    AbstractMapper* chooseMapper();
    AbstractMapper* chooseMapper(QStringList classList);
    PObject* create(string className);

    PObjectIconViewItemTemplate* getPObjectIconViewItemTemplate(QString clName,QListWidget *lv);
    void registerPObjectIconViewItemTemplate(QString clName, PObjectIconViewItemTemplate *it);

private:
     GuiRepositoryImpl();



     void addTypeView(list<PObject*>* list, QString label, QString short_label);
     void wrappWidgetAndAdd(QWidget *w);
    list<AbstractMapper*>* getMapViews();
    QMenu* getPopup(QString identifier);
    list<AbstractMapper*>* getPersistentClasses();

    
	list<PropertyEditor*> *activeEditors;
    list<GuiMode*> *modelist;

    
private:
     static GuiRepositoryImpl* impl_instance;
     QMdiArea* formWorkspace;
     QStackedWidget *centralWidget;

     const QPixmap std_icon_pixmap;
     GuiMainWindow /*KXmlGuiWindow*/ *mainFrame;
     //stundenplan *activeplan;
     list<PObjectIconView*> *activeIconViews;
     GuiMode *activeMode;
     MoviePlayer *videow;


protected:
    QGraphicsView* GraphicsView;
    //KApplication *app;
    //StundePlanerApp *stundeplaner;
    list<AbstractMapper*> *listMappers, *listMapViews;
    map<QString, QString> mapIcons;
    map<QString, PObjectIconViewItemTemplate*> mapTemplates;

private:
    QToolBar *modeToolBar, *serviceToolBar;


};


#endif
