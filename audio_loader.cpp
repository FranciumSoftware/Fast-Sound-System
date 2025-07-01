#include "audio_loader.h"
#include <QUrl>

AudioLoader::AudioLoader(QObject *parent) : QObject(parent)
{
    player.setAudioOutput(&audioOutput);
}

void AudioLoader::loadAudio(const QString &filePath)
{
    QUrl fileUrl = QUrl::fromLocalFile(filePath);
    player.setSource(fileUrl);
    emit audioLoaded(fileUrl);
}
