//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "genericmapview.h"

#include <qpoint.h>
#include <QGraphicsScene>
//#include <qwmatrix.h>
#include <qmessagebox.h>
#include <QMouseEvent>

#include <QFileDialog>
//#include <kaction.h>
#include <krun.h>

//#include "gui/draganddrop/pobjectdrag.h"
#include "relateaction.h"
#include <transactions/transactions.h>
#include "guirepository.h"
#include "pobjectdialog.h"
#include "datenpopup.h"
#include "mapviewspopupfactory.h"

#include <repository/repository.h>
#include <repository/repositoryentry.h>
#include <repository/repositoryproperty.h>

#include "textpropertyviewer.h"
#include "pobjectgraphicsitemnp.h"

#include "reports/latexoutput.h"

#include <qinputdialog.h>
#include <persistence/database.h>


GenericMapView::GenericMapView(QWidget * parent, const char * name) : QGraphicsView(parent)
{
	//popupMenu = new ThemaMapPopup();
	pmenu = 0;
	map=0;
	selectedItem = 0;
	
    setFocusPolicy(Qt::StrongFocus);
    setAcceptDrops(true);
    viewport()->setAcceptDrops(true);

	

}


GenericMapView::~GenericMapView()
{
}



void GenericMapView::resizeEvent ( QResizeEvent * event )
{
    if(scene()){
        fitInView(scene()->sceneRect(),Qt::KeepAspectRatio);
    }
    QGraphicsView::resizeEvent(event);

}

void GenericMapView::mousePressEvent( QMouseEvent* e )
{
     if(selectedItem){
		selectedItem->setSelected(false);
		selectedItem =0;
     }
     QPoint pos = e->pos();

    PObjectGraphicsItemNP *preselectItem =
            dynamic_cast<PObjectGraphicsItemNP*>(
                itemAt(pos));
    if(preselectItem){
        preselectItem->setSelected(true);
    } else {
        movingItem=0;
    }


    /*
    QList<QGraphicsItem*> itemList= items(pos);
    PObjectGraphicsItemNP *preselectItem =0;
    if(! itemList.empty()){
     	preselectItem = dynamic_cast<PObjectGraphicsItemNP*>( *(itemList.begin()) );
        if(preselectItem){
            preselectItem->setSelected(true);
        }
    } else {
    	movingItem =0;
    }
    */
     	
     
    if(e->button()==Qt::RightButton){
    	//QPoint pos = e->globalPos();
	selectedItem = preselectItem;
	//popupMenu->popup(e->globalPos(), GraphicsPos, preselectItem);
	currentPos =e->globalPos();
	//getPopupMenu()->popup(currentPos); //, GraphicsPos, preselectItem)
	emit(popupRequested(currentPos));
	movingItem=0;
    } else if (e->button() == Qt::LeftButton){
        movingItem = preselectItem;
        selectedItem=preselectItem;
        if(selectedItem){
            if(PObjectGraphicsItem *citem = dynamic_cast<PObjectGraphicsItem*>(movingItem)){
                Transactions::getCurrentTransaction()->add(citem);
            }
            GuiRepository::getInstance()->setSelectedObject(selectedItem->getObject());
    	}

     }
     getMap()->update();
}


void GenericMapView::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(selectedItem){
	selectedItem->setSelected(false);
    }

     QPoint pos = e->pos();
     QList<QGraphicsItem*>  itemList=items(pos);
     QGraphicsItem *preselectItem =0;
     if(! itemList.empty()){
     	selectedItem = dynamic_cast<PObjectGraphicsItemNP*>(*(itemList.begin()));
	if(selectedItem){
		selectedItem->setSelected(true);
		emit activateSelectedRequested(selectedItem);
		//activateSelected();
	}
    }
}

void GenericMapView::wheelEvent ( QWheelEvent * event )
{
    /*
    double factor = 1.41*(-event->delta() /240.0);
    scale(factor,factor);
    */
}

void GenericMapView::mouseMoveEvent ( QMouseEvent * e )
{
 	if(movingItem){

        //QPointF p = mapFromGlobal(e->globalPos());
        QPointF p = mapToScene(e->pos());
        movingItem->setX(p.x());
        movingItem->setY(p.y());
        //movingItem->moveBy(p.x()-movingItem->x(),p.y()-movingItem->y());
        /*
        movingItem->moveBy(e->pos().x()-movingItem->x(),e->pos().y()-movingItem->y());
		getMap()->update();
        */
	}
}

/*!
    \fn GenericMapView::getGenericMap()
 */
GenericMap* GenericMapView::getMap()
{
    return map;
}


/*!
    \fn GenericMapView::setGenericMap(GenericMap *map)
 */
void GenericMapView::setMap(GenericMap *map)
{
    this->map = map;
    //popupMenu->setMap(map);
    
    QGraphicsView::setScene(map);
    map->displayMap();
}


/*!
    \fn GenericMapView::keyPressEvent(QKeyEvent *e)
 */
void GenericMapView::keyPressEvent(QKeyEvent *e)
{
    qDebug("GenericMapView : Key Pressed");
}





/*!
    \fn GenericMapView::contentsDragEnterEvent(QDragEnterEvent *e)
 */
void GenericMapView::dragEnterEvent(QDragEnterEvent *e)
{
     qDebug() << "GenericMapView : DragEnter";
     //if(e->mimeData()->hasFormat("application/pobject")){
       e->acceptProposedAction();
    //}
	
}

void GenericMapView::dragMoveEvent(QDragMoveEvent* e)
{
	qDebug("GenericMapView : DragMove");
	/*
	if(PObjectDrag::canDecode(e)){
		
	}
	*/
}
/*!
    \fn GenericMapView::contentsDropEvent(QDropEvent *e)
 */
 void GenericMapView::dropEvent(QDropEvent *e)
{
     qDebug("GenericMapView : Drop");

    QPoint eventPos = e->pos();
    QPointF graphicsPos = mapToScene(eventPos);
	PObject *o=0;
    const QMimeData *mdata = e->mimeData();
    if(mdata->hasFormat("application/pobject") ){ // recieved a valid object
        QByteArray data=e->mimeData()->data("application/pobject");
        QDataStream stream(&data,QIODevice::ReadOnly);
        QString className;
        int id;
        stream >> className >> id;
        o = Database::getInstance()->load(className.toStdString(), id);
        /*
             o= PObjectData::decode(e->mimeData()->data("application/pobject"));
         */
        if(o){
            e->accept();
        }else{
            e->ignore();
            qDebug() << "GenericMapView::contentsDropEvent: Strange, how to decode this data?";
        }
    } else if(mdata->hasUrls()){
        QList<QUrl> lurls=mdata->urls();
        if(lurls.size()>0 && lurls.at(0).isLocalFile()){
                QString file = lurls.at(0).toLocalFile();
                qDebug() << "GenericMapView::dropEvent: Sorry dont know what to do for local file !???";
		}
        //e->accept();
        e->ignore();
    } else {
        qDebug("GenericMapView::contenstDropEvent : Decode of event failed");
    }
	

	if(o){
        QList<QGraphicsItem*> col  = items(eventPos); //getMap()->items(eventPos);
		if(! col.isEmpty() ){
			bool done = false;
            QList<QGraphicsItem*>::iterator it;
			for(it = col.begin(); it != col.end(); it++){
				if(PObjectGraphicsItemNP *oci = dynamic_cast<PObjectGraphicsItemNP*>( (*it) ))
				{
					bool done_here = RelateAction::getInstance()->relate(o,oci->getObject());
					if(done_here){
						qDebug("Adding via relation");
						Transactions::getCurrentTransaction()->add(o);
						Transactions::getCurrentTransaction()->add(oci->getObject());
						oci->update();
					} 
					done = (done | done_here);
				}
				
			}
			if(!done){
				qDebug("Adding to map without relation");
                map->addItem(o,graphicsPos);
			}
		}  else {
			qDebug("Collisionlist was empty");
            map->addItem(o,graphicsPos);
		}
		//qDebug("GenericMapView::contentsDropEvent : Adding to map from PObjectDrag");
		//map->addItem(o,GraphicsPos);
		map->save();
	   } else {
		qWarning("GenericMapView::contenstDropEvent : decoded Object invalid");
	   }
      
      
      
}




/*!  Deletes the selected item from the map

	

    \fn GenericMapView::deleteSelected()
 */
/*
void GenericMapView::deleteSelected()
{
    if(PObjectGraphicsItemNP *pitemnp = dynamic_cast<PObjectGraphicsItemNP*>(selectedItem)){
	pitemnp->setVisible(FALSE);
	selectedItem=0;
	if(PObjectGraphicsItem *pitem = dynamic_cast<PObjectGraphicsItem*>(pitemnp)){
		Transactions::getCurrentTransaction()->add(getMap());
		getMap()->deleteFromGraphicsItems(pitem);
	}	
	getMap()->update();
	/** \todo How to properly delete an item ? */
	//delete pitem;
	/*
    }
}
*/
/*!
    \fn GenericMapView::itemDown()
 */
/*
void GenericMapView::itemDown()
{
	if(PObjectGraphicsItemNP* npitem = dynamic_cast<PObjectGraphicsItemNP*>(selectedItem)){
    		npitem->setZ(npitem->z() -2);
		if(PObjectGraphicsItem *pitem = dynamic_cast<PObjectGraphicsItem*>(npitem)){
			Transactions::getCurrentTransaction()->add(pitem);
		}
	}
	getMap()->update();
}


/*!
    \fn GenericMapView::itemUp()
 */
/*
void GenericMapView::itemUp()
{
    if(PObjectGraphicsItemNP* npitem = dynamic_cast<PObjectGraphicsItemNP*>(selectedItem)){
    		npitem->setZ(npitem->z() +2);
		if(PObjectGraphicsItem *pitem = dynamic_cast<PObjectGraphicsItem*>(npitem)){
			Transactions::getCurrentTransaction()->add(pitem);
		}
	}
	getMap()->update();
}
*/
/*!
    \fn GenericMapView::activateSelected()
 */

void GenericMapView::activateSelected()
{
    	qDebug("GenericMapView::activateSelected : do nothing implementation called");
	/*
	if(selectedItem){
	if(PObjectGraphicsItemNP* pitem = dynamic_cast<PObjectGraphicsItemNP*>(selectedItem)){
		if(PObject *o = pitem->getObject()){
			if(material *m = dynamic_cast<material*>(o)){
				QString fileName = m->getFile()->name();
				if(fileName.contains(".tex")){ // hack to handel tex-files
					KRun::runCommand(QString("kile %1").arg(fileName),fileName,"");
				} else {	
					new KRun(fileName);
				}
			} else  if (stundenplaneintrag *se = dynamic_cast<stundenplaneintrag*>(o)){
				krusbucheintrag *ke = se->getKursbuchEintrag();
				RepositoryEntry *re = Repository::getInstance()->getRepositoryEntry(ke);
				if(re){
					RepositoryProperty *rp = re->getProperty("Eintrag");
					TextPropertyEditorDialog::edit(rp,ke);
				} else {
					qDebug("GenericMapViewControler::activateSelected : strange : could not get kursbucheintrag");
				}
			} else { 
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
	*/
}

/*
void GenericMapView::showFormForSelected()
{
 if(selectedItem){
	if(PObjectGraphicsItemNP* pitem = dynamic_cast<PObjectGraphicsItemNP*>(selectedItem)){
		if(PObject *o = pitem->getObject()){
				GuiRepository::getInstance()->showFormForObject(o);
		}
	}
    }
}
*/

/*
QPopupMenu*  GenericMapView::getDatenPopupForSelected()
{
	if(selectedItem){
		if(PObjectGraphicsItemNP* pitem = dynamic_cast<PObjectGraphicsItemNP*>(selectedItem)){
			if(PObject *o = pitem->getObject()){
				QPopupMenu *pup = new DatenPopup(o,this);
				///*
				QPopupMenu *pup = new QPopupMenu(this);
				RepositoryEntry *re = Repository::getInstance() ->getRepositoryEntry( o );
				list<RepositoryProperty*> *list_rp = re->getAllProperties( true );
				for(list<RepositoryProperty*>::iterator it = list_rp->begin(); it != list_rp->end(); it++){
					RepositoryProperty *rp = *it;
					if(rp->isCollection()){
						QAction *ac = new ShowCollectionAction(o,rp,pup);
						ac->addTo(pup);
					} else if(rp->isPObject()){
						QAction *ac = new ShowObjectAction(o,rp,pup);
						ac->addTo(pup);
					} else {
						QAction *ac = new ShowPropertyAction(o,rp,pup);
						ac->addTo(pup);
					}
				}
				//*//*
			return pup;
			}
		}
	}
	return 0;
}
*/



/*
void GenericMapView::writeLatex()
{
	QString fileName = KFileDialog::getSaveFileName();

	LatexOutput *lout = new LatexOutput(QString("Testoutput"),fileName);
	if(lout){
		if(selectedItem){
			if(PObjectGraphicsItemNP* pitem = dynamic_cast<PObjectGraphicsItemNP*>(selectedItem)){
				if(PObject *o = pitem->getObject()){
					lout->write(o);
				} else {
				qDebug("GenericMapView::writeLatex : could not get object from item");
				} 
			} else {
				qDebug("GenericMapView::writeLatex : could not recover selected item");
				lout -> write(getMap());	
			}
    		} else {	
			lout -> write(getMap());
		}
		lout -> close();
		lout -> edit();
	}	

	delete lout;
}
*/


/*!
    \fn GenericMapView::zoomIn()
 */
/*
void GenericMapView::zoomOut()
{
    	QWMatrix wm = worldMatrix();
	wm.scale(.9,.9);
	setWorldMatrix(wm);
}
*/

/*!
    \fn GenericMapView::zoomOut()
 */
/*
void GenericMapView::zoomIn()
{
    QWMatrix wm = worldMatrix();
	wm.scale(1.1,1.1);
	setWorldMatrix(wm);
}
*/

/*
void GenericMapView::createNewItem()
{
	 PObject* o = CreateAction::getInstance()->createObject();
	getMap()->addItem(o,currentPos);

}
*/

/*
list<QAction*>* GenericMapView::initActionsForSelection(QActionCollection *actionCollection)
{
	list<QAction*> *actionList = new list<QAction*>();
	if(selectedItem){
		if(PObjectGraphicsItemNP* pitem = dynamic_cast<PObjectGraphicsItemNP*>(selectedItem)){
			if(PObject *o = pitem->getObject()){
				if(stundenplaneintrag *se = dynamic_cast<stundenplaneintrag*>(o)){
					actionList->push_back(new ShowSitzplanAction(se,this));
				}
			}
		}
	}
	return actionList;
}
*/

/*!
    \fn GenericMapView::initActions()
 */
/*
list<QAction*>* GenericMapView::initActions(QActionCollection *actionCollection)
{
	list<QAction*> *actionList = new list<QAction*>();

	if(selectedItem){
		list<QAction*> *actionsForSelected = initActionsForSelection(actionCollection);
		for(list<QAction*>::iterator it = actionsForSelected->begin(); it != actionsForSelected->end(); it++){
			actionList->push_back((*it));
		}
		actionList-> push_back( new QAction("Rauf", KStdAccel::shortcut(KStdAccel::New), this,SLOT(itemUp()), actionCollection, "activate") );
		
		actionList-> push_back( new QAction("Runter", KStdAccel::shortcut(KStdAccel::New), this,SLOT(itemDown()), actionCollection, "activate") );

		actionList-> push_back( new QAction("Anzeigen", KStdAccel::shortcut(KStdAccel::New), this,SLOT(activateSelected()), actionCollection, "activate") );

		
		actionList-> push_back( new QAction("Formular", KStdAccel::shortcut(KStdAccel::New), this,SLOT(showFormForSelected()), actionCollection, "activate") );

		actionList -> push_back( new QAction("Löschen", KStdAccel::shortcut(KStdAccel::New), this,SLOT(deleteSelected()), actionCollection, "activate") );
	
		actionList -> push_back( new QAction("Latex", KStdAccel::shortcut(KStdAccel::New), this,SLOT(writeLatex()), actionCollection, "activate") );

	} else {
		actionList->push_back( new QAction("Vergrößern", KStdAccel::shortcut(KStdAccel::New), this,SLOT(zoomIn()), actionCollection, "zoomin") );
	
		
		actionList->push_back( new QAction("Verkleinern", KStdAccel::shortcut(KStdAccel::New), this,SLOT(zoomOut()), actionCollection, "zoomout") );

	
		actionList -> push_back( new QAction("Neues Element", KStdAccel::shortcut(KStdAccel::New), this,SLOT(createNewItem()), actionCollection, "create_new") );
		
	}

	return actionList;
}
*/



/*!
    \fn GenericMapView::getPopupMenu()
 */
QMenu* GenericMapView::getPopupMenu()
{
  	MapviewsPopupFactory *factory = MapviewsPopupFactory::getInstance();
	if(factory){
		return factory->getPopupForGenericMapView(this,currentPos);
	} else {
        return new QMenu();
	}
}


/*!
    \fn GenericMapView::getSelected()
 */
PObjectGraphicsItemNP* GenericMapView::getSelected()
{
    return selectedItem;
}

void GenericMapView::clearSelection()
{
    selectedItem = 0;
}


/*!
    \fn GenericMapView::getSelectedObject()
 */
PObject* GenericMapView::getSelectedObject()
{
    	PObject *o=0;
    	PObjectGraphicsItemNP* ci = getSelected();
	if(ci){
		o = ci->getObject();
	}
	return o;
}
