#include "standardguimode.h"
#include "guirepository.h"
#include "formarea.h"
#include "guiconfig.h"

#include <QPixmap>


StandardGuiMode::StandardGuiMode()
    : GuiMode("StandardMode")
{
    stack=0;

    QPixmap pm = GuiConfig::getInstance()->getIcon("StandardGuiMode");
    if(pm.isNull()){
        GuiConfig::getInstance()->selectIcon("StandardGuiMode");
        pm = GuiConfig::getInstance()->getIcon("StandardGuiMode");
    }
    setIcon(pm);
}


void StandardGuiMode::setupMode()
{
    GuiRepository *guirep=GuiRepository::getInstance();
    QStackedWidget *sw=guirep->getCentralWidget();

    guirep->setActiveMode(this);

    if(stack==0){
        stack= new QStackedWidget(sw);

        formArea = new FormArea();
        stack->addWidget(formArea);
        //formArea->hide();

        sw->addWidget(stack);
        stack->setCurrentWidget(formArea);
    }

    sw->setCurrentWidget(stack);


}

void StandardGuiMode::showForm(QWidget *w)
{
    formArea->showForm(w);
}
