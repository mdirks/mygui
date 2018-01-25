//
// C++ Interface: stringpropertyfilter
//
// Description: 
//
//
// Author: Marcus Dirks <marcus.dirks@web.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef STRINGPROPERTYFILTER_H
#define STRINGPROPERTYFILTER_H

#include <repository/repositoryproperty.h>
#include "propertyfilter.h"

#include <string>

using namespace std;

/**
	@author Marcus Dirks <marcus.dirks@web.de>
*/
class StringPropertyFilter : PropertyFilter {
public:
    StringPropertyFilter(RepositoryProperty *rp, string testItem);
    ~StringPropertyFilter();

    bool apply(PObject *o);
    void apply(list<PObject*> *input, list<PObject*> *output){AbstractFilter::apply(input,output);}
    void apply(list<PObject*> *input){AbstractFilter::apply(input);}
    string getValueString();
    void setValueString(string vs);

protected:
	//RepositoryProperty *rp;
	string testItem;
};

#endif
