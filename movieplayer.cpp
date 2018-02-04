#include "movieplayer.h"

MoviePlayer::MoviePlayer(QWidget *p)
    : QVideoWidget(p)
{
    player = new QMediaPlayer();
    setFocusPolicy(Qt::StrongFocus);
}


void MoviePlayer::show(QString movieFile)
{
    QUrl url= QUrl::fromLocalFile(movieFile);
    if(url.isLocalFile()){
        player->setMedia(url);
        player->setVideoOutput(this);
        player->play();
        ispaused=false;
        isfs=false;
    } else {
        player->stop();
    }
}

void MoviePlayer::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key()==Qt::Key_Right){
        player->setPosition(player->position()+50000);
    } else if(ev->key()==Qt::Key_Left){
        player->setPosition(player->position()-50000);
    } else if(ev->key()==Qt::Key_Up){
        player->setPosition(player->position()+500000);
    } else if(ev->key()==Qt::Key_Down){
        player->setPosition(player->position()-500000);
    }else if(ev->key()==Qt::Key_Space){
        switchplay();
        //player->setPosition(player->position()-500000);
    }else if(ev->key()==Qt::Key_F){
        switchfullscreen();
        qDebug()<<"switched fs";
        //player->setPosition(player->position()-500000);
    } else {
        QVideoWidget::keyPressEvent(ev);
    }
}


void MoviePlayer::wheelEvent(QWheelEvent *event)
{
    int adv = event->delta()*100;
    player->setPosition(player->position()+adv);
    event->accept();
}

void MoviePlayer::switchplay()
{
    if(ispaused){
        player->play();
        ispaused=false;
    } else {
        player->pause();
        ispaused=true;
    }
}

void MoviePlayer::stop()
{
    player->stop();
}

void MoviePlayer::switchfullscreen()
{
    if(!isfs){
        setFullScreen(true);
        isfs=true;
    } else {
        setFullScreen(false);
        isfs=false;
    }
}
