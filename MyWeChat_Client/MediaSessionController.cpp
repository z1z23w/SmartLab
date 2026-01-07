#include "MediaSessionController.h"
#include <QNetworkDatagram>
#include <QDebug>

MediaSessionController::MediaSessionController(QObject *parent) : QObject(parent) {
    m_audio = new AudioManager(this);
    m_udpSocket = new QUdpSocket(this);

    // 连接信号
    connect(m_audio, &AudioManager::audioDataReady, this, &MediaSessionController::onAudioData);
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &MediaSessionController::onUdpReadyRead);
}

void MediaSessionController::initUdp(QString myName, QString serverIp) {
    m_myName = myName;
    m_serverIp = serverIp;

    // 发送注册包 [0x01][Len][Name]
    QByteArray packet;
    packet.append((char)0x01);
    packet.append((char)myName.toUtf8().length());
    packet.append(myName.toUtf8());

    m_udpSocket->writeDatagram(packet, QHostAddress(serverIp), m_serverPort);
    qDebug() << "UDP 已初始化，发送注册包";
}

void MediaSessionController::setTarget(QString targetName) {
    m_targetName = targetName;
}

void MediaSessionController::startCall() {
    qDebug() << "通话开始";
    m_audio->startRecording();
}

void MediaSessionController::stopCall() {
    qDebug() << "通话结束";
    m_audio->stopRecording();
}

void MediaSessionController::onAudioData(QByteArray data) {
    if (m_targetName.isEmpty()) return;

    // 发送音频包 [0x02][TargetLen][TargetName][PCM]
    QByteArray packet;
    packet.append((char)0x02);
    packet.append((char)m_targetName.toUtf8().length());
    packet.append(m_targetName.toUtf8());
    packet.append(data);

    m_udpSocket->writeDatagram(packet, QHostAddress(m_serverIp), m_serverPort);
}

void MediaSessionController::onUdpReadyRead() {
    while (m_udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
        QByteArray audioData = datagram.data();

        // 收到就是纯音频数据，直接播放
        if (!audioData.isEmpty()) {
            m_audio->playAudioChunk(audioData);
        }
    }
}
