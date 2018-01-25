//
// C++ Interface: latexoutput
//
// Description: 
//
//
// Author: Marcus Dirks <marcus.dirks@web.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LATEXOUTPUT_H
#define LATEXOUTPUT_H

#include <qfile.h>
#include <qtextstream.h>
#include <qstring.h>

#include <persistence/pobject.h>

/**
	@author Marcus Dirks <marcus.dirks@web.de>
*/
class LatexOutput : public QFile {
public:
    LatexOutput(QString title, QString& filename);

    ~LatexOutput();
    /*
    void write( stunde *st);
    void write( stundenplaneintrag *se);
    void write( notiz *n);
    void write( WeekMap *wm );
    void write( schultag *scht);
    void write( kursbuch *kb);
    void write( teilleistung *tl );
    void write(list<fehlzeit*> *lf);

    */
    void write( PObject *o);

    void close();
    void edit();


protected:
	void writeHeader();
	void writeFooter();

private:
	QTextStream *stream;
	QString title;
};

#endif
