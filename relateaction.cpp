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
#include "relateaction.h"

RelateAction* RelateAction::instance = 0;

RelateAction::RelateAction()
{
}


RelateAction::~RelateAction()
{
}

RelateAction* RelateAction::getInstance()
{
	if( ! instance){
		instance = new RelateAction();
	}
	return instance;
}


bool RelateAction::relate(PObject *o1, PObject *o2)
{
       qDebug("Relate PObjects: Do nothing !");
       return false;
}
