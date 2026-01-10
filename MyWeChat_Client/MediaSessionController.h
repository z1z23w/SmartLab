#pragma once
#include <QObject>
#include <QUdpSocket>
#include <QMediaCaptureSession>
#include <QCamera>
#include <QVideoSink>
#include <QMediaDevices>
#include <QThread>
#include <QPointer>
#include <QImage>
#include <QTimer>
#include <QBuffer>
#include "AudioManager.h"
#include "VideoItem.h"

//视频编码器
class VideoEncoder : public QObject {
    Q_OBJECT
    QML_ELEMENT
public slots:
    void processImage(const QImage &img) {
        if (img.isNull()) return;

        //缩放+压缩
        QImage small = img.scaled(320, 240, Qt::KeepAspectRatio, Qt::FastTransformation);
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);

        if (small.save(&buffer, "JPEG", 30)) {
            if (bytes.size() < 60000) {
                emit encodedData(bytes);
            }
        }
    }
signals:
    void encodedData(QByteArray data);
};

//视频解码器
class VideoDecoder : public QObject {
    Q_OBJECT
    QML_ELEMENT
public slots:
    void processBuffer(const QByteArray &data) {
        QImage img;
        if (img.loadFromData(data, "JPEG")) {
            emit decodedImage(img);
        }
    }
signals:
    void decodedImage(QImage img);
};

//主控制器
class MediaSessionController : public QObject {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit MediaSessionController(QObject *parent = nullptr);
    ~MediaSessionController();

    void initUdp(QString myName, QString serverIp);
    void setTarget(QString targetName);
    void setRemoteVideoItem(VideoItem *item);

    void startCall(bool videoEnabled);
    void stopCall();

private slots:
    void onAudioData(QByteArray data);
    void onUdpReadyRead();
    void onCamFrameChanged(); //保持void，内部处理
    void sendHeartbeat();

    void onEncodedData(QByteArray data);
    void onDecodedImage(QImage img);

signals:
    void requestEncode(const QImage &img);
    void requestDecode(const QByteArray &data);

private:
    AudioManager *m_audio;
    QUdpSocket *m_udpSocket;
    QTimer *m_heartbeatTimer;

    QMediaCaptureSession m_captureSession;
    QCamera *m_camera = nullptr;
    QVideoSink *m_captureSink;
    QPointer<VideoItem> m_remoteVideoItem;

    QThread *m_encodeThread;
    VideoEncoder *m_encoder;

    QThread *m_decodeThread;
    VideoDecoder *m_decoder;

    QString m_serverIp;
    QString m_myName;
    QString m_targetName;
    int m_serverPort = 9998;

    bool m_isVideoEnabled = false;
    bool m_isCallConnected = false;
};
