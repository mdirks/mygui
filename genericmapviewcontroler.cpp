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
#include "genericmapviewcontroler.h"
//#include "guicreateaction.h"
#include "pobjectgraphicsitemnp.h"
#include "pobjectgraphicsitem.h"
#include "textpropertyviewer.h"
#include "guirepository.h"
#include <repository/repository.h>
#include <repository/repositoryentry.h>
#include "qpoint.h"
#include <transactions/transactions.h>
#include "reports/latexoutput.h"
#include "datenpopup.h"
//#include "showhistoryaction.h"

#include <krun.h>
#include <qfiledialog.h>

#include <QDebug>

GenericMapViewControler::GenericMapViewControler(GenericMapView *mapView)
{
	this->mapView = mapView;
	connect(mapView,SIGNAL(popupRequested(QPoint)), this, SLOT(showPopup(QPoint)));
	connect(mapView,SIGNAL(activateSelectedRequested(PObjectGraphicsItemNP*)), this, SLOT(activateSelected(PObjectGraphicsItemNP*)));
	
	isPlanning = false;
}


GenericMapViewControler::~GenericMapViewControler()
{
}

/*
void GenericMapViewControler::createNewItem()
{
	PObject* o = GuiCreateAction::getInstance()->createObject();
	mapView->getMap()->addItem(o,pos);

}
*/

void GenericMapViewControler::createNewItem()
{
    PObject* o = GuiRepository::getInstance()->createObject();
	mapView->getMap()->addItem(o,pos);
    Transactions::getCurrentTransaction()->add(mapView->getMap());
    //mapView->getMap()->save();
}


void GenericMapViewControler::deleteSelected()
{
    if(PObjectGraphicsItemNP *pitemnp = dynamic_cast<PObjectGraphicsItemNP*>(mapView->getSelected()))
    {
    pitemnp->setVisible(false);
	mapView->clearSelection();
	if(PObjectGraphicsItem *pitem = dynamic_cast<PObjectGraphicsItem*>(pitemnp)){
		Transactions::getCurrentTransaction()->add(mapView->getMap());
		mapView->getMap()->deleteFromGraphicsItems(pitem);
	}	
	mapView->getMap()->update();
	/** \todo How to properly delete an item ? */
	//delete pitem;

    }
}

/*!
    \fn GenericMapView::itemDown()
 */
void GenericMapViewControler::itemDown()
{
	if(PObjectGraphicsItemNP* npitem = dynamic_cast<PObjectGraphicsItemNP*>(mapView->getSelected())){
            npitem->setZValue(npitem->z() -2);
		if(PObjectGraphicsItem *pitem = dynamic_cast<PObjectGraphicsItem*>(npitem)){
			Transactions::getCurrentTransaction()->add(pitem);
		}
	}
	mapView->getMap()->update();
}


/*!
    \fn GenericMapView::itemUp()
 */
void GenericMapViewControler::itemUp()
{
    if(PObjectGraphicsItemNP* npitem = dynamic_cast<PObjectGraphicsItemNP*>(mapView->getSelected())){
            npitem->setZValue(npitem->zValue() +2);
		if(PObjectGraphicsItem *pitem = dynamic_cast<PObjectGraphicsItem*>(npitem)){
			Transactions::getCurrentTransaction()->add(pitem);
		}
	}
	mapView->getMap()->update();
}
/*!
    \fn GenericMapView::activateSelected()
 */
void GenericMapViewControler::activateSelected(PObjectGraphicsItemNP *selectedItem)
{
	
	if(selectedItem){
		if(PObjectGraphicsItemNP* pitem = dynamic_cast<PObjectGraphicsItemNP*>(selectedItem)){
			if(PObject *o = pitem->getObject()){
					RepositoryEntry *re = Repository::getInstance()->getRepositoryEntry(o);
					RepositoryProperty *rp = 0;
					if(re && (rp=re->getMainProperty())){
						TextPropertyEditorDialog::edit(rp,o);
					} else {
						GuiRepository::getInstance()->showFormForObject(o);
					}
			}
		}
	}

}

void GenericMapViewControler::showFormForSelected()
{
 PObjectGraphicsItemNP *selectedItem = mapView->getSelected();
 if(selectedItem){
	if(PObjectGraphicsItemNP* pitem = dynamic_cast<PObjectGraphicsItemNP*>(selectedItem)){
		if(PObject *o = pitem->getObject()){
                GuiRepository::getInstance()->showFormForObject(o,false);
		}
	}
    }
}



void GenericMapViewControler::writeLatex()
{
    QString fileName = QFileDialog::getSaveFileName();

	LatexOutput *lout = new LatexOutput(QString("Testoutput"),fileName);
	if(lout){
		if(PObjectGraphicsItemNP *selectedItem = mapView->getSelected()){
			if(PObjectGraphicsItemNP* pitem = dynamic_cast<PObjectGraphicsItemNP*>(selectedItem)){
				if(PObject *o = pitem->getObject()){
					lout->write(o);
				} else {
				qDebug("GenericMapView::writeLatex : could not get object from item");
				} 
			} else {
				qDebug("GenericMapView::writeLatex : could not recover selected item");
				lout -> write(mapView->getMap());	
			}
    		} else {	
			lout -> write(mapView->getMap());
		}
		lout -> close();
		lout -> edit();
	}	

	delete lout;
}

void GenericMapViewControler::showPopup(QPoint pos)
{
	this->pos = pos;
    QMenu *pmenu = getPopupMenu();
	

	pmenu->popup(pos);
}

void GenericMapViewControler::fit()
{
    mapView->fitInView(mapView->scene()->sceneRect(),Qt::KeepAspectRatio);
}

/*!
    \fn GenericMapViewControler::getPopupMenu()
 */
QMenu* GenericMapViewControler::getPopupMenu()
{
        QMenu *pmenu = new QMenu(mapView);
	PObjectGraphicsItemNP *selectedItem =mapView->getSelected();
	if(selectedItem){
        pmenu->addAction("Rauf", this,SLOT(itemUp()));
        pmenu->addAction("Runter", this,SLOT(itemDown()));
        pmenu->addAction("Anzeigen", this,SLOT(activateSelected()));
        pmenu->addAction("Formular", this,SLOT(showFormForSelected()));
        pmenu->addAction("Löschen",this,SLOT(deleteSelected()));
        pmenu->addAction("Latex", this,SLOT(writeLatex()));

        if(PObject *o = selectedItem->getObject()){
            pmenu->addMenu(new DatenPopup(o,mapView));
		}

	} else {
        pmenu->addAction("Fit",this,SLOT(fit()));
        pmenu->addAction("Vergrößern",  this,SLOT(zoomIn()));
        pmenu->addAction("Verkleinern", this,SLOT(zoomOut()));
        pmenu->addAction("Neues Element", this,SLOT(createNewItem()));

	}

	return pmenu;
}
