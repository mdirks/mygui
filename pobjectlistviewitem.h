//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef POBJECTLISTVIEWITEM_H
#define POBJECTLISTVIEWITEM_H

#include <QListWidget>

#include <repository/repositoryentry.h>
#include <repository/repositoryproperty.h>
#include <persistence/pobject.h>
#include "repositoryitem.h"

/**
@author Marcus Dirks
*/
class PObjectListViewItem : public QListWidgetItem, public RepositoryItem
{
public:
    PObjectListViewItem(QListWidget *parent, RepositoryProperty *rp, PObject *parent_object);
    PObjectListViewItem(QListWidgetItem *parent, RepositoryProperty *rp, PObject *parent_object);
    ~PObjectListViewItem();

    RepositoryProperty* getRepositoryProperty();
    QString text( int column ) const;
    PObject* getParent();
    
protected:
     virtual void activate();
     
private:
	PObject *parent, *property;
	RepositoryProperty *rp;
	RepositoryEntry *entry;
	
};

#endif
