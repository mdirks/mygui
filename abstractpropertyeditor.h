//
// C++ Interface: abstractpropertyeditor
//
// Description: 
//
//
// Author: Marcus Dirks <marcus.dirks@web.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ABSTRACTPROPERTYEDITOR_H
#define ABSTRACTPROPERTYEDITOR_H

#include "propertyeditor.h"
#include "editrequestor.h"
#include <persistence/pobject.h>
#include <repository/repositoryproperty.h>


/**
	@author Marcus Dirks <marcus.dirks@web.de>
*/
class AbstractPropertyEditor : public PropertyEditor
{


public:
    AbstractPropertyEditor();
    AbstractPropertyEditor(PObject *parentObject, RepositoryProperty *prop);
    ~AbstractPropertyEditor();


    virtual void startEdit();
    virtual void stopEdit();

public:
	EditRequestor *editRequestor;

protected:
	bool editing;

protected:
	
    PObject *parentObject;
        RepositoryProperty *property;
};

#endif
