//
// C++ Implementation: latexoutput
//
// Description: 
//
//
// Author: Marcus Dirks <marcus.dirks@web.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "latexoutput.h"

#include <krun.h>
#include <list>
#include <map>
#include <string>
#include <QDebug>

#include <repository/repository.h>
#include <repository/repositoryentry.h>
#include <repository/repositoryproperty.h>
#include "datecompare.h"

LatexOutput::LatexOutput(QString t, QString& filename)
	: QFile(filename), title(t)
{   
    if ( this->open(QIODevice::WriteOnly) ) {
        this-> stream = new QTextStream( this );
    //stream->setEncoding(QTextStream::Latin1);
	writeHeader();
    } else {
	stream = 0;
    }



}


LatexOutput::~LatexOutput()
{
        //close();

}

void LatexOutput::close()
{
	writeFooter();
	QFile::close();
}

void LatexOutput::edit()
{
    qDebug() << QString("Editing %1").arg(this->fileName());
	//new KRun(this->name());
    KRun::runCommand(QString("texmaker %1").arg(this->fileName()),0);

}



void LatexOutput::writeHeader()
{
	if(stream){
		*stream << "\\documentclass[a4paper,10pt]{article} \n\
\\usepackage{mdstd} \n\
\n\
\\begin{document} \n\n";
	}
}

void LatexOutput::writeFooter()
{
	if(stream)
	{
		*stream << "\\end{document} \n";
	}
}

void LatexOutput::write( PObject *o)
{
        qDebug() << QString("LatexOutput::write : Warning : write not implemented for %1").arg(o->getPersistenceObject()->getClassName().c_str());

}

