#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QKeyEvent>

class MoviePlayer : public QVideoWidget
{
public:
    MoviePlayer(QWidget *p);
    void show(QString movieFile);

protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void switchplay();


private:
    QMediaPlayer *player;
    bool ispaused;
};

#endif // MYPLAYER_H
