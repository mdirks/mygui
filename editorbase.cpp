#include "editorbase.h"
#include <QLabel>
#include <QPushButton>
#include "guicontroler.h"

EditorBase::EditorBase(QWidget *parent) : QDialog(parent)
{
    mainWidget=new QLabel("Empty Label",this);

    QPushButton *closeButton = new QPushButton(tr("Close"));
    connect(closeButton, &QAbstractButton::clicked, this, &QWidget::close);

    contentsLayout = new QVBoxLayout;
    contentsLayout->addWidget(mainWidget);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout= new QVBoxLayout;
    mainLayout->addLayout(contentsLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

}


void EditorBase::slotOk()
{
    GuiControler::getInstance()->stopEdit(); /*QDialog::slotOk();*/
}

void EditorBase::setMainWidget(QWidget *mw)
{
    contentsLayout->replaceWidget(mainWidget,mw);
    mainWidget=mw;
}
