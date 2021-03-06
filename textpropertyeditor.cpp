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
#include "textpropertyeditor.h"
#include "guirepository.h"
#include "guicontroler.h"
#include <transactions/transactions.h>
#include <QTextStream>

TextPropertyEditor::TextPropertyEditor(QWidget *parent)
 : QTextEdit(parent) , property(0)
{
    connect(this,SIGNAL(textChanged()),this,SLOT(startEdit()));
    setLineWrapMode(QTextEdit::NoWrap);
}

TextPropertyEditor::TextPropertyEditor(PObject *o, RepositoryProperty *prop, QWidget *p)
 : QTextEdit(p), property(prop), parent(o)
{
    setLineWrapMode(QTextEdit::NoWrap);
    setText(property->asString( parent ).c_str());
    connect(this,SIGNAL(textChanged()),this,SLOT(startEdit()));
}


TextPropertyEditor::TextPropertyEditor(PObject *o, QString displayString, QWidget *p)
 : QTextEdit(p), property(0), parent(o)
{
    setLineWrapMode(QTextEdit::NoWrap);
    setText(displayString);
    connect(this,SIGNAL(textChanged()),this,SLOT(startEdit()));
}



TextPropertyEditor::~TextPropertyEditor()
{
}




/*!
    \fn TextPropertyEditor::startEdit(RepositoryProperty *prop, PObject *o)
 */
void TextPropertyEditor::startEdit(RepositoryProperty *prop, PObject *parent)
{
    this->property = prop;
    this->parent = parent;
    setLineWrapMode(QTextEdit::NoWrap);
    setText(property->asString( parent ).c_str());
}

void TextPropertyEditor::startEdit()
{
    if(!editing){
        //QString propString  = QString::fromStdString(property->asString(parent));
        //setText(propString);

		GuiControler::getInstance()->addActiveEditor(this);
		editing = true;
		Transactions::getCurrentTransaction()->add(parent);
	}
}


void TextPropertyEditor::stopEdit()
{
    if(property && parent){
        QString text = toPlainText();
        QTextStream str(&text);
        property->fromString(str.readAll().toStdString(), parent);
		parent->save();
	}
	editing = false;
	
	//clear();
}


/*!
    \fn TextPropertyEditor::focusOutEvent(QFocusEvent *e)
 */
void TextPropertyEditor::focusOutEvent(QFocusEvent *e)
{
	//qDebug("TextPropertyEditor: focusOutEvent");
       //stopEdit();
}

void TextPropertyEditor::setProperty(RepositoryProperty *rp)
{
    this->property = rp;
}

void TextPropertyEditor::setParentObject(PObject *po)
{
    this->parent=po;
    if(property && po){
        setText(property->asString( parent ).c_str());
    } else {
        clear();
    }
}
