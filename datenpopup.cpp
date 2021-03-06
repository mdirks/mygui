/***************************************************************************
 *   Copyright (C) 2008 by Marcus Dirks   *
 *   m-dirks@web.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "datenpopup.h"
#include <repository/repository.h>
#include <mapping/mappingcontroler.h>
#include "pobjectdialog.h"
#include "guirepository.h"
#include <transactions/transactions.h>

#include <qmessagebox.h>

DatenPopup::DatenPopup(PObject *o, QWidget *parent)
 : QMenu(parent)
{
    setTitle("Daten");
	RepositoryEntry *re = Repository::getInstance() ->getRepositoryEntry( o );
	if(re){
		list<RepositoryProperty*> *list_rp = re->getAllProperties( true );
		for(list<RepositoryProperty*>::iterator it = list_rp->begin(); it != list_rp->end(); it++){
			RepositoryProperty *rp = *it;
			if(rp->isCollection()){
                QAction *ac = new ShowCollectionAction(o,rp,this);
                addAction(ac);
			} else if(rp->isPObject()){
				QAction *ac = new ShowObjectAction(o,rp,this);
                addAction(ac);
			} else {
				QAction *ac = new ShowPropertyAction(o,rp,this);
                addAction(ac);
			}
		}
	}
}



ShowCollectionAction::ShowCollectionAction(PObject *o, RepositoryProperty *rp, QObject *parent) 
    : QAction(rp->getName().c_str(), parent)
{
	this->o=o;
	this->rp = rp;
        connect(this, SIGNAL(triggered(bool)) , this, SLOT( showCollection() ) );
}

ShowCollectionAction::~ShowCollectionAction(){}

void ShowCollectionAction::showCollection()
{
	//QMessageBox::about(GuiRepository::getInstance()->getMainFrame(), QString("ShowCollection"),QString("ShowCollectionAction::showCollection()"));
	list<PObject*> *list_objects = 0;
	if(list_objects =  rp->asCollection(o)){
		PObjectDialog::choosePObject(list_objects);
	}
}


ShowObjectAction::ShowObjectAction(PObject *o, RepositoryProperty *rp, QObject *parent) 
    : QAction(rp->getName().c_str(),parent)
{
	this->o=o;
	this->rp = rp;
        connect(this, SIGNAL(triggered(bool)) , this, SLOT( showObject() ) );
}

ShowObjectAction::~ShowObjectAction(){}

void ShowObjectAction::showObject()
{
	//QMessageBox::about(GuiRepository::getInstance()->getMainFrame(), QString("ShowObject"),QString("ShowObjectAction::showCollection()"));
	PObject *po;
	if(po =  rp->asPObject(o)){
		if(po){
            GuiRepository::getInstance()->showFormForObject(po);
            //GuiRepository::getInstance()->showDialogForObject(po);
		} else {
			QMessageBox::information(GuiRepository::getInstance()->getMainFrame(),"Kein Objekt","Kein Objekt","OK");
		}
	} else {
		string className = rp->getType();
		po = PObjectDialog::choosePObject(MappingControler::getInstance()->getMapperByName( className ) );
		Transactions::getCurrentTransaction()->add(o);
		rp->set(po,o);
	}
}

ShowPropertyAction::ShowPropertyAction(PObject *o, RepositoryProperty *rp, QObject *parent) 
    : QAction(rp->getName().c_str(), parent)
{
	this->o=o;
	this->rp = rp;
        connect(this, SIGNAL(triggered(bool)), this, SLOT( showProperty() ) );
}

ShowPropertyAction::~ShowPropertyAction(){}

void ShowPropertyAction::showProperty()
{
	//QMessageBox::about(GuiRepository::getInstance()->getMainFrame(), QString("ShowObject"),QString("ShowObjectAction::showCollection()"));
		GuiRepository::getInstance()->showEditorForProperty(o,rp);
}





DatenPopup::~DatenPopup()
{
}


