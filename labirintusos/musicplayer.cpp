#include "musicplayer.h"

#include <QMediaPlaylist>
#include <QtDebug>

MusicPlayer::MusicPlayer()
{
    player = new QMediaPlayer(this);
    playMusic();
    volume = 5;
    player->setVolume(volume);
}


void MusicPlayer::playMusic()
{
    QMediaPlaylist *playlist = new QMediaPlaylist();
    //playlist->addMedia(QUrl::fromLocalFile("C:\\Users\\naray\\Documents\\Qt Projects\\A_IDE_MINDENT\\songs\\theSong.wav"));
    //playlist->addMedia(QUrl("https://open.spotify.com/track/7DPdGTjKlnV3Q3IfFNYYRn?si=hJgB-3fDT1Kd-PYYN06bqA"));
    playlist->addMedia(QUrl::fromLocalFile("songs/song1.mp3"));
    playlist->addMedia(QUrl::fromLocalFile("songs/song2.mp3"));
    playlist->addMedia(QUrl::fromLocalFile("songs/song3.mp3"));
    playlist->addMedia(QUrl::fromLocalFile("songs/song4.mp3"));
    playlist->shuffle();
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    player->setPlaylist(playlist);
    player->play();
}

int MusicPlayer::getVolume() const
{
    return volume;
}

void MusicPlayer::tooLoud()
{
    if(volume > 0)volume -= 5;
    player->setVolume(volume);
}

void MusicPlayer::tooQuiet()
{
    if(volume < 100)volume += 5;
    player->setVolume(volume);
}
