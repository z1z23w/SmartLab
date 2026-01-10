#include "MediaSessionController.h"
#include <QNetworkDatagram>
#include <QDebug>
#include <QBuffer>

MediaSessionController::MediaSessionController(QObject *parent) : QObject(parent) {
    m_isCallConnected = false;

    //基础设备
    m_audio = new AudioManager(this);
    m_udpSocket = new QUdpSocket(this);
    m_heartbeatTimer = new QTimer(this);

    //绑定端口
    if (m_udpSocket->bind(QHostAddress::AnyIPv4, 0, QUdpSocket::ShareAddress)) { qDebug() << "UDP绑定成功"; }

    //摄像头
    const QCameraDevice &defaultCamera = QMediaDevices::defaultVideoInput();
    m_camera = new QCamera(defaultCamera.isNull() ? QMediaDevices::defaultVideoInput() : defaultCamera, this);
    m_captureSession.setCamera(m_camera);
    m_captureSink = new QVideoSink(this);
    m_captureSession.setVideoOutput(m_captureSink);

    //多线程
    m_encodeThread = new QThread(this);
    m_encoder = new VideoEncoder();
    m_encoder->moveToThread(m_encodeThread);
    connect(m_encodeThread, &QThread::finished, m_encoder, &QObject::deleteLater);
    m_encodeThread->start();

    m_decodeThread = new QThread(this);
    m_decoder = new VideoDecoder();
    m_decoder->moveToThread(m_decodeThread);
    connect(m_decodeThread, &QThread::finished, m_decoder, &QObject::deleteLater);
    m_decodeThread->start();

    //信号连接

    //发送链路
    connect(this, &MediaSessionController::requestEncode, m_encoder, &VideoEncoder::processImage, Qt::QueuedConnection);
    connect(m_encoder, &VideoEncoder::encodedData, this, &MediaSessionController::onEncodedData, Qt::QueuedConnection);

    //接收链路
    connect(this, &MediaSessionController::requestDecode, m_decoder, &VideoDecoder::processBuffer, Qt::QueuedConnection);
    connect(m_decoder, &VideoDecoder::decodedImage, this, &MediaSessionController::onDecodedImage, Qt::QueuedConnection);

    connect(m_audio, &AudioManager::audioDataReady, this, &MediaSessionController::onAudioData);
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &MediaSessionController::onUdpReadyRead);
    connect(m_captureSink, &QVideoSink::videoFrameChanged, this, &MediaSessionController::onCamFrameChanged);
    connect(m_heartbeatTimer, &QTimer::timeout, this, &MediaSessionController::sendHeartbeat);
    m_heartbeatTimer->start(1000);
}

MediaSessionController::~MediaSessionController() {
    stopCall();

    m_encodeThread->quit();
    m_encodeThread->wait();
    m_decodeThread->quit();
    m_decodeThread->wait();
}

void MediaSessionController::initUdp(QString myName, QString serverIp) {
    m_myName = myName;
    m_serverIp = serverIp;
    for(int i=0; i<5; i++) sendHeartbeat();
}

void MediaSessionController::sendHeartbeat() {
    if (m_myName.isEmpty()) return;
    QByteArray packet;
    packet.append((char)0x01);
    packet.append((char)m_myName.toUtf8().length());
    packet.append(m_myName.toUtf8());
    m_udpSocket->writeDatagram(packet, QHostAddress(m_serverIp), m_serverPort);
}

void MediaSessionController::setTarget(QString targetName) { m_targetName = targetName; }
void MediaSessionController::setRemoteVideoItem(VideoItem *item) { m_remoteVideoItem = item; }

void MediaSessionController::startCall(bool videoEnabled) {
    stopCall();

    m_isVideoEnabled = videoEnabled;
    m_isCallConnected = true;

    qDebug() << "通话开始";

    for(int i=0; i<3; i++) sendHeartbeat();

    m_audio->startRecording();
    if (m_isVideoEnabled && m_camera) {
        m_camera->start();
    }
}

void MediaSessionController::stopCall() {
    m_isCallConnected = false;
    m_audio->stopRecording();
    m_audio->resetOutput();
    if (m_camera && m_camera->isActive()) m_camera->stop();

    if (m_remoteVideoItem) {
        QMetaObject::invokeMethod(m_remoteVideoItem, "updateFrame", Qt::QueuedConnection, Q_ARG(QImage, QImage()));
    }
}

// 发送链路
void MediaSessionController::onCamFrameChanged() {
    if (!m_isCallConnected || !m_isVideoEnabled) return;

    //安全获取帧
    QVideoFrame frame = m_captureSink->videoFrame();
    if (!frame.isValid()) return;

    //立即转为QImage
    QImage img = frame.toImage();
    if (img.isNull()) return;

    //格式统一
    if (img.format() != QImage::Format_RGB32) {
        img = img.convertToFormat(QImage::Format_RGB32);
    }

    //深拷贝
    QImage safeImg = img.copy();

    //发送给子线程
    emit requestEncode(safeImg);
}

void MediaSessionController::onEncodedData(QByteArray data) {
    if (!m_isCallConnected || m_targetName.isEmpty()) return;

    QByteArray packet;
    packet.append((char)0x03);
    packet.append((char)m_targetName.toUtf8().length());
    packet.append(m_targetName.toUtf8());
    packet.append(data);

    m_udpSocket->writeDatagram(packet, QHostAddress(m_serverIp), m_serverPort);
}

//接收链路
void MediaSessionController::onUdpReadyRead() {
    while (m_udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
        QByteArray data = datagram.data();

        if (!m_isCallConnected) continue;

        if (data.size() > 2 && (unsigned char)data[0] == 0xFF && (unsigned char)data[1] == 0xD8) {
            //视频：扔给子线程解码
            emit requestDecode(data);
        } else {
            //音频
            if (!data.isEmpty()) m_audio->playAudioChunk(data);
        }
    }
}

void MediaSessionController::onDecodedImage(QImage img) {
    if (m_isCallConnected && m_remoteVideoItem) {
        m_remoteVideoItem->updateFrame(img);
    }
}

void MediaSessionController::onAudioData(QByteArray data) {
    if (!m_isCallConnected || m_targetName.isEmpty()) return;
    QByteArray packet;
    packet.append((char)0x02);
    packet.append((char)m_targetName.toUtf8().length());
    packet.append(m_targetName.toUtf8());
    packet.append(data);
    m_udpSocket->writeDatagram(packet, QHostAddress(m_serverIp), m_serverPort);
}
