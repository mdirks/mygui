/***************************************************************************
                          stundeplaner.cpp  -  description
                             -------------------
    begin                : Sam Feb 28 13:37:40 CET 2004
    copyright            : (C) 2004 by Marcus Dirks
    email                : marcus.dirks@web.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// include files for QT
#include <QDockWidget>
#include <QDebug>
#include <QList>
#include <QKeyEvent>
#include <qdir.h>
#include <QPrinter>
#include <qpainter.h>
#include <qprogressdialog.h>
#include <qprocess.h>
//#include <QStackedWidget>
#include <QStatusBar>


// include files for KDE
//#include <QActionCollection>
//#include <kcmdlineargs.h>
#include <kaboutdata.h>
//#include <klocale.h>
//#include <kapp.h>
#include <kiconloader.h>
#include <kmessagebox.h>
//#include <kfiledialog.h>
//#include <kmenubar.h>
//#include <kstatusbar.h>
//#include <klocale.h>
#include <kconfig.h>
//#include <kstdaction.h>
#include <krun.h>
#include <qlayout.h>
//#include <kdockwidget.h>
//#include <kglobal.h>
//#include <kinputdialog.h>
#include <KActionCollection>
#include <QInputDialog>
#include <QFileDialog>


// application specific includes
#include "guimainwindow.h"
//#include "stundeplanerview.h"
//#include "stundeplanerdoc.h"


#include "guirepository.h"
#include "guicontroler.h"
#include "guiconfig.h"
#include "xmlimportdialog.h"
#include "xmlexportdialog.h"
#include "pobjecticonview.h"
#include "pobjectdialog.h"
#include <transactions/transactions.h>
//#include "guicreateaction.h"
#include <mapping/mappingcontroler.h>
#include <persistence/database.h>


#define ID_STATUS_MSG 1



GuiMainWindow::GuiMainWindow(QWidget* , const char* name): KXmlGuiWindow() /*QMainWindow()*/
{
  initStatusBar();
  initActions();
  
  //setupGUI();
  setupGUI(Default,"/home/mopp/dev/stundenplaner/gui/stundenplanerui.rc");
  qDebug() << QString("Build gui from local xml file %1").arg(localXMLFile());
  qDebug() << QString("Build gui from local xml file %1").arg(xmlFile());
  //initDocument();

  globalTb = new QToolBar("global",this);
  QAction *action= actionCollection()->addAction("add_objectview", this, SLOT(slotNewObjectIconView()));
  action->setIcon(GuiConfig::getInstance()->getIcon("objectview"));
  action->setText("Objektbrowser");
  globalTb->addAction(action);

  initView();
  


  readOptions();
  //createTaskBar();

  fileSave->setEnabled(true);
  fileSaveAs->setEnabled(true);
  filePrint->setEnabled(false);
  editCut->setEnabled(false);
  editCopy->setEnabled(false);
  editPaste->setEnabled(false);

  GuiRepository::getInstance()->setMainFrame(this);

}

GuiMainWindow::~GuiMainWindow()
{

}

void GuiMainWindow::initActions()
{
  QAction *action;

  changeDatabaseAction = actionCollection()->addAction("change_database", this, SLOT(slotChangeDatabase()));
  changeDatabaseAction->setIcon(GuiConfig::getInstance()->getIcon("action_database"));
  changeDatabaseAction->setText("Datenbank wechseln");


  action=actionCollection()->addAction("dump_database", this, SLOT(slotDumpDatabase()));
  action->setIcon(GuiConfig::getInstance()->getIcon("DumpDB"));
  action->setText("DB sichern");

  action=actionCollection()->addAction("read_database", this, SLOT(slotReadDatabase()));
  action->setIcon(GuiConfig::getInstance()->getIcon("ReadDB"));
  action->setText("DB lesen");




  fileNewWindow = actionCollection()->addAction("new_window", this, SLOT(slotFileNewWindow()));
  //fileNew = KStandardAction::openNew(GuiCreateAction::getInstance(), SLOT(createObject()), actionCollection());
  fileOpen = KStandardAction::open(this, SLOT(slotFileOpen()), actionCollection());
  //fileOpenRecent = KStandardAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)), actionCollection());
  fileSave = KStandardAction::save(this, SLOT(slotFileSave()), actionCollection());
  fileSaveAs = KStandardAction::saveAs(this, SLOT(slotFileSaveAs()), actionCollection());
  fileClose = KStandardAction::close(this, SLOT(slotFileClose()), actionCollection());
  filePrint = KStandardAction::print(this, SLOT(slotFilePrint()), actionCollection());
  fileQuit = KStandardAction::quit(this, SLOT(slotFileQuit()), actionCollection());
  editCut = KStandardAction::cut(this, SLOT(slotEditCut()), actionCollection());
  editCopy = KStandardAction::copy(this, SLOT(slotEditCopy()), actionCollection());
  editPaste = KStandardAction::paste(this, SLOT(slotEditPaste()), actionCollection());

}

QToolBar* GuiMainWindow::getGlobalToolbar()
{
    return globalTb;
}

void GuiMainWindow::initStatusBar()
{
  //statusBar()->insertItem(i18n("Ready."), ID_STATUS_MSG);
}


QStackedWidget* GuiMainWindow::getCentralWidget()
{
    return centralWidget;
}

void GuiMainWindow::initView()
{ 
  centralWidget = new QStackedWidget(this);
  setCentralWidget(centralWidget);
}





void GuiMainWindow::addDockWindow(QWidget *w, QString name, Qt::DockWidgetArea area)
{

    QDockWidget *dock = new QDockWidget(name,this);
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(w);
    addDockWidget(area, dock);

}

void GuiMainWindow::saveOptions()
{	
  qDebug() << "Warning: GuiMainWindow::saveOptions() not implemented" ;

}


void GuiMainWindow::readOptions()
{
  qDebug()<< "Warning: GuiMainWindow::readOptions() not implemented" ;

}




bool GuiMainWindow::queryClose()
{
    return true;
  //return doc->saveModified();
}

bool GuiMainWindow::queryExit()
{
  saveOptions();
  return true;
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////
void GuiMainWindow::slotChangeDatabase()
{
    QString db_name = QInputDialog::getText(this,"Datenbank wechseln","Neue Datenbank",QLineEdit::Normal,Database::getDatabaseName());

	GuiRepository::getInstance()->closeGui();
    if(MappingControler::getInstance()->initNewDatabase(db_name.toStdString())){
        GuiConfig::getInstance()->setDatabaseName(db_name);
		
	} else {
		GuiRepository::getInstance()->showInfo("Ungueltige Datenbank",QString("Konnte Datenbank %1 nicht oeffnen").arg(db_name));
		
	}
	GuiRepository::getInstance()->reloadGui();
}


void GuiMainWindow::slotDumpDatabase()
{
    QString fileName = QFileDialog::getSaveFileName();
	dumpDatabase(fileName);
}

void GuiMainWindow::dumpDatabase(QString fileName)
{
    QString databasename = GuiConfig::getInstance()->getDatabaseName();
    QStringList args = databasename.split(":");
    QString dbName=args.at(0);
    QString com;
    if(args.size()>1){
        com = QString("mysqldump -h%1 -u%2 -p%3 %4 > %5").arg(args.at(1),args.at(2),args.at(3),dbName,fileName);
    } else {
        com = QString("mysqldump -user=root %1 > %2").arg(dbName).arg(fileName);
    }
    qDebug() << com;
    KRun::runCommand(com,this);
}

void GuiMainWindow::slotReadDatabase()
{
    QString fileName = QFileDialog::getOpenFileName();
    QString dbName = QInputDialog::getText(this,"Datenbank einlesen","Datenbankname",QLineEdit::Normal,Database::getDatabaseName());

	QString pstring = QString("mysql");
    QString astring = QString("--user=root  %1 < %2").arg(dbName).arg(fileName);
    qDebug() << QString("Creating process %1 with %2").arg(pstring).arg(astring);
	p = new QProcess(this);
	//p->addArgument(astring);
	
    QString program = "mysql";
    QStringList arguments;
    arguments << "--user=root"  << dbName << fileName;
    p->start(program,arguments);
    connect(p,SIGNAL(processExited()),this,SLOT(slotDatabaseReadFinished()));
    GuiConfig::getInstance()->setDatabaseName(dbName);
}

void GuiMainWindow::slotDatabaseReadFinished(){
    if(p && p->exitStatus() == QProcess::NormalExit){
		QString dbName = GuiConfig::getInstance()->getDatabaseName();
		GuiRepository::getInstance()->closeGui();
        if(MappingControler::getInstance()->initNewDatabase(dbName.toStdString())){
			GuiRepository::getInstance()->showInfo("Datenbank gelesen",QString("Datenbank %1 wurde geoeffnet").arg(dbName));
		} else {
			GuiRepository::getInstance()->showInfo("Ungueltige Datenbank",QString("Konnte Datenbank %1 nicht oeffnen").arg(dbName));
			
		}
		GuiRepository::getInstance()->reloadGui();
		p=0;
	} else {
		GuiRepository::getInstance()->showInfo("Einlesen fehlgeschlagen",QString("Konnte Datenbankfile  nicht oeffnen"));
	}
}


void GuiMainWindow::slotNewObjectIconView()
{
    PObjectIconView *iconView = new PObjectIconView(GuiRepository::getInstance()->chooseMapper());
	GuiRepository::getInstance()->addIconView(iconView, QString("Leer %1").arg(getNewIconViewNumber()),"Leer");
}


int GuiMainWindow::getNewIconViewNumber()
{
	return iv_nr++;
}



void GuiMainWindow::slotFileNewWindow()
{

  GuiMainWindow *new_window= new GuiMainWindow();
  new_window->show();


}

void GuiMainWindow::slotFileNew()
{


}

void GuiMainWindow::slotFileOpen()
{
	
  XmlImportDialog *dialog = new XmlImportDialog(this);
  dialog->exec();
}

void GuiMainWindow::slotFileOpenRecent(const QUrl& url)
{
	


}

void GuiMainWindow::slotFileSave()
{
	
  GuiControler::getInstance()->stopEdit();
  Transactions::getCurrentTransaction()->commit();

}

void GuiMainWindow::slotFileSaveAs()
{
  XmlExportDialog *dialog = new XmlExportDialog(this);
  dialog->show();

}

void GuiMainWindow::slotFileClose()
{
	
  close();

}

void GuiMainWindow::slotFilePrint()
{
    qDebug()<< "Warning: GuiMainWindow::slotFilePrint() not implemented";

}

void GuiMainWindow::slotFileQuit()
{
  dumpDatabase(QString("/home/mopp/schule/stundenplaner/stundenplaner-%1.sql").arg(QDate::currentDate().toString("yy-MM-dd")));

  saveOptions();

}

void GuiMainWindow::slotEditCut()
{
}

void GuiMainWindow::slotEditCopy()
{

}

void GuiMainWindow::slotEditPaste()
{
}

void GuiMainWindow::slotViewToolBar()
{
    qDebug() << "Warning: GuiMainWindow::slotViewToolBar() not implemented";

}

void GuiMainWindow::slotViewStatusBar()
{
  //turn Statusbar on or off
  if(!viewStatusBar->isChecked())
  {
    statusBar()->hide();
  }
  else
  {
    statusBar()->show();
  }

}


void GuiMainWindow::slotStatusMsg(const QString &text)
{
   qDebug() << "Warning: GuiMainWindow::slotStatusMsg() not implemented";

}


void GuiMainWindow::keyPressEvent( QKeyEvent *k )
{
    qDebug() << QString("GuiMainWindow: ") + k->text();
}



/*!
    \fn GuiMainWindow::getTree
 */
/*
doctree_stunden* GuiMainWindow::getTree()
{
    return view->getTree(); 
}
*/



MySideBar::MySideBar(QWidget *parent)
    : QListWidget(parent)
{

    setViewMode(QListView::IconMode);
    setSelectionRectVisible(false);
    setStyleSheet("QListWidget { background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(100, 100, 100, 255), stop:1 rgba(150, 150, 150, 255)); border-right: 2px groove gray; }"
           "QListWidget::item:hover { background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(150, 150, 150, 255), stop:1 rgba(200, 200, 200, 255)); }"
                  "QListWidget::item:selected { background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(200, 200, 200, 200), stop:1 rgba(220, 220, 220, 255));}"
                  "QListWidget::item {border-bottom: 2px groove gray; padding: 10px; color: white;}");

    setIconSize(QSize(48,48));
    connect(this,SIGNAL(itemActivated(QListWidgetItem*)),this,SLOT(activateItem(QListWidgetItem*)));
}

void MySideBar::activateItem(QListWidgetItem *item)
{
    GuiMode *mode = dynamic_cast<GuiMode*>(item);
    if(mode){
        GuiRepository::getInstance()->setActiveMode(mode);
    }
}
