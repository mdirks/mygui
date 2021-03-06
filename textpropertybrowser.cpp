#include "textpropertybrowser.h"
#include <transactions/transactions.h>
#include "guirepository.h"
#include <QVBoxLayout>

TextPropertyBrowser::TextPropertyBrowser(PObject *po, RepositoryProperty *cP, RepositoryProperty *dispProp, QWidget *parent) :
    QWidget(parent)
{
    this->colProp=cP;
    parentObject=0;

    viewer = new TextPropertyViewer(0,dispProp,this);
    toolBar = new QToolBar(this);

    combo = new PObjectComboBox(colProp,po,this);
    connect(combo,SIGNAL(currentIndexChanged(int)),this,SLOT(indexChanged(int)));
    connect(combo,SIGNAL(editTextChanged(QString)),this,SLOT(nameChanged(QString)));

    /*
    QString String = "border: 0px solid black;";
    combo->setStyleSheet(String );
    combo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    combo->setEditable(true);
    combo->setInsertPolicy(QComboBox::InsertAtCurrent);
    */

    spinBox = new QSpinBox(this);
    spinBox->setMaximum(999);
    connect(spinBox,SIGNAL(valueChanged(int)),this,SLOT(numberChanged(int)));
    //toolBar->addAction("New",this,SLOT(newObject()));
    toolBar->addWidget(combo);
    toolBar->addWidget(spinBox);




    QVBoxLayout *l= new QVBoxLayout(this);
    l->setContentsMargins(0,0,0,0);
    l->addWidget(toolBar);
    l->addWidget(viewer);

    if(po){
        setParentObject(po);
    }

}

void TextPropertyBrowser::numberChanged(int i)
{
    qDebug() << "TODO: implement TextPropertyBrowser::numberChanged";
}

void TextPropertyBrowser::nameChanged(QString newName)
{
    PObject *o=combo->getObject(combo->currentIndex());
    if(o){
        Transactions::getCurrentTransaction()->add(o);
        o->setName(newName.toStdString());
    }

}

void TextPropertyBrowser::newObject()
{
    PObject *o = GuiRepository::getInstance()->create(colProp->getType());
    Transactions::getCurrentTransaction()->add(parentObject);
    colProp->add(o,parentObject);
    setParentObject(parentObject);
}

void TextPropertyBrowser::setParentObject(PObject *po)
{
    this->parentObject=po;
    combo->setParentObject(po);

    /*
    if(colProp){
        load(colProp->asCollection(parentObject));
    }
    */
}

/*
void TextPropertyBrowser::load(list<PObject*> *ol)
{
    this->olist = ol;

    int ind=combo->currentIndex();
    combo->clear();
    for(list<PObject*>::iterator it=olist->begin(); it!=olist->end(); it++)
    {
        combo->addItem((*it)->getName().c_str());
    }
    //if(ind>-1) combo->setCurrentIndex(ind);
}
*/

void TextPropertyBrowser::indexChanged(int i)
{
        PObject *o = combo->getObject(i);
        viewer->setParentObject(o);
}

/*
PObject* TextPropertyBrowser::getObject(int i)
{
    PObject *o=0;
    if(olist && i<olist->size() && i>-1){
        std::list<PObject*>::iterator it = olist->begin();
        std::advance(it,i);
        o=*it;
    }
    return o;
}
*/
