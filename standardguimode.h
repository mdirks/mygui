#ifndef STANDARDGUIMODE_H
#define STANDARDGUIMODE_H

#include "guimode.h"
#include "formarea.h"
#include <QStackedWidget>

class StandardGuiMode : public GuiMode
{
public:
    StandardGuiMode();

    void setupMode();
    void showForm(QWidget *w);

private:
    QStackedWidget *stack;
    FormArea *formArea;
};

#endif // STANDARDGUIMODE_H
