#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QDialog>
#include <QMediaPlayer>
#include <QDebug>


class MusicPlayer : public QDialog
{
public:
    MusicPlayer();

    void tooLoud();
    void tooQuiet();

    int getVolume() const;

private:
    void playMusic();

    int volume;
    QMediaPlayer* player;


};

#endif // MUSICPLAYER_H
