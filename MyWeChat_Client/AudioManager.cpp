#include "AudioManager.h"
#include <QDebug>

AudioManager::AudioManager(QObject *parent) : QObject(parent) {
    // 8000Hz, 单声道, 16位 (VoIP标准)
    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleFormat(QAudioFormat::Int16);

    //初始化扬声器
    QAudioDevice outputDev = QMediaDevices::defaultAudioOutput();
    if (!outputDev.isNull()) {
        m_sink = new QAudioSink(outputDev, m_format, this);
        m_sink->setBufferSize(8192);
        m_outputDevice = m_sink->start();
    }

    //初始化麦克风
    QAudioDevice inputDev = QMediaDevices::defaultAudioInput();
    if (!inputDev.isNull()) {
        m_source = new QAudioSource(inputDev, m_format, this);
    }
}

AudioManager::~AudioManager() {
    if (m_source) m_source->stop();
    if (m_sink) m_sink->stop();
}

void AudioManager::startRecording() {
    if (!m_source) return;
    if (m_source->state() == QAudio::ActiveState) m_source->stop();
    m_inputDevice = m_source->start();
    connect(m_inputDevice, &QIODevice::readyRead, this, &AudioManager::onMicReadyRead);
}

void AudioManager::stopRecording() {
    if (!m_source) return;
    m_source->stop();
    if (m_inputDevice) {
        disconnect(m_inputDevice, &QIODevice::readyRead, this, &AudioManager::onMicReadyRead);
        m_inputDevice = nullptr;
    }
}

void AudioManager::resetOutput() {
    if (m_sink) {
        m_sink->stop();
        m_sink->reset();                  //清空缓冲区
        m_outputDevice = m_sink->start(); //重启等待
    }
}

void AudioManager::onMicReadyRead() {
    if (!m_inputDevice) return;
    QByteArray data = m_inputDevice->readAll();
    if (!data.isEmpty()) emit audioDataReady(data);
}

void AudioManager::playAudioChunk(const QByteArray &data) {
    if (m_outputDevice && m_sink) {
        m_outputDevice->write(data);
        if (m_sink->state() == QAudio::StoppedState) {
            m_sink->start();
        }
    }
}
