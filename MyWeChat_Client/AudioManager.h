#pragma once
#include <QObject>
#include <QAudioSource>
#include <QAudioSink>
#include <QMediaDevices>
#include <QAudioFormat>
#include <QQmlEngine>

class AudioManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit AudioManager(QObject *parent = nullptr);
    ~AudioManager();

    void startRecording();
    void stopRecording();
    void resetOutput();
    void playAudioChunk(const QByteArray &data);

signals:
    void audioDataReady(QByteArray data);

private slots:
    void onMicReadyRead();

private:
    QAudioSource *m_source = nullptr;
    QAudioSink *m_sink = nullptr;
    QIODevice *m_inputDevice = nullptr;
    QIODevice *m_outputDevice = nullptr;
    QAudioFormat m_format;
};
