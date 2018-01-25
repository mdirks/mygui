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
#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H

#include <repository/repositoryproperty.h>
#include <persistence/pobject.h>

/**
@author Marcus Dirks
*/
class PropertyEditor{
public:
    
    virtual void stopEdit() = 0;
    virtual void startEdit(RepositoryProperty *rp, PObject *parent) = 0;
    virtual void startEdit() = 0;
};

#endif
