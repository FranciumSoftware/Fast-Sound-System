#ifndef AUDIOLOADER_H
#define AUDIOLOADER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>

class AudioLoader : public QObject
{
    Q_OBJECT

public:
    explicit AudioLoader(QObject *parent = nullptr);
    void loadAudio(const QString &filePath);

signals:
    void audioLoaded(const QUrl &fileUrl);

private:
    QMediaPlayer player;
    QAudioOutput audioOutput;
};

#endif // AUDIOLOADER_H
