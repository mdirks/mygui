#ifndef EDITORBASE_H
#define EDITORBASE_H

#include <QDialog>
#include <QVBoxLayout>

class EditorBase : public QDialog
{
public:
    EditorBase(QWidget *parent);
    void setMainWidget(QWidget *mw);

protected slots:
    void slotOk();

private:
    QWidget *mainWidget;
    QVBoxLayout *contentsLayout;

};

#endif // EDITORBASE_H
