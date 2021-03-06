//
// C++ Interface: filter
//
// Description: 
//
//
// Author: Marcus Dirks <marcus.dirks@web.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FILTER_H
#define FILTER_H

#include <persistence/pobject.h>

#include <list>

using namespace std;

/**
	@author Marcus Dirks <marcus.dirks@web.de>
*/
class Filter{
public:
    
    virtual void apply(list<PObject*> *input)=0;
    virtual void apply(list<PObject*> *input, list<PObject*> *output) = 0;
    virtual bool apply(PObject *o) = 0;
    virtual void addAnd(Filter *f) = 0;
    virtual void addOr(Filter *f) = 0;
};

#endif
