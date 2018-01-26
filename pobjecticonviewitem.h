//
// C++ Interface: pobjecticonviewitem
//
// Description: 
//
//
// Author: Marcus Dirks <marcus.dirks@web.de>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef POBJECTICONVIEWITEM_H
#define POBJECTICONVIEWITEM_H

#include <QListWidgetItem>
#include <QLabel>
#include <persistence/pobject.h>
#include "textpropertyviewer.h"
#include <repository/repositoryproperty.h>
#include "stringeditor.h"

#include <QToolButton>
#include <list>

class ActiveLabel;
class PropertyButton;

/**
	@author Marcus Dirks <marcus.dirks@web.de>
*/


class PObjectIconViewItemBase : public QListWidgetItem
{
public:
    PObjectIconViewItemBase(PObject *o, QListWidget *iv);
    ~PObjectIconViewItemBase();
    PObject* getObject();
    virtual void activateItem()=0;
    virtual void showFull(bool full){};

protected:
    QListWidget *lv;

private:
    PObject *object;

};

class PObjectIconViewItemTemplate : public QObject
{
public:
    PObjectIconViewItemTemplate(){};

    virtual PObjectIconViewItemTemplate* getInstanceFor(QListWidget *lv)=0;
    virtual PObjectIconViewItemBase *createItem(PObject *o)=0;

};


class PObjectIconViewItem : public PObjectIconViewItemBase
{

public:
    PObjectIconViewItem(PObject *o, QListWidget *iv, QPixmap &icon, RepositoryProperty *displayProp=0);
    ~PObjectIconViewItem();

    virtual void activateItem();
};

class PObjectIconViewItemE : public QObject, public PObjectIconViewItemBase
{
    Q_OBJECT
public:
    PObjectIconViewItemE(PObject *o, list<RepositoryProperty*> *listRp, QListWidget *iv, QPixmap &icon);
    ~PObjectIconViewItemE();

    virtual void activateItem();

    //void setDisplayProperty(RepositoryProperty *p);
    void showFull(bool full);

public slots:
     void editRequested();
     void setNewSize();

private:
    QWidget *widget;
    QLabel *label1a;
    ActiveLabel *label1b;
    //TextPropertyViewer *label2;
    QStackedWidget *sw;



};

class ActiveLabel : public QStackedWidget
{
Q_OBJECT
public:
    ActiveLabel(PObject *po, QWidget *parent=0);
protected:
    void mouseDoubleClickEvent( QMouseEvent * e );

public slots:
    void stopEdit();

signals:
    void clicked();

private:
    PObject *po;
    StringEditor *nameEditor;
    QLabel *nameLabel;
};

class PropertyButton : public QToolButton
{
Q_OBJECT
public:
    PropertyButton(RepositoryProperty *rp, int i,QWidget *editor, QStackedWidget *editorStack, QWidget *parent);
public slots:
    void switchVisible();

signals:
    void resized();

private:
    int ind;
    QWidget *editor;
    QStackedWidget *editorStack;
};
#endif
