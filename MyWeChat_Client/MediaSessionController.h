#ifndef MEDIASESSIONCONTROLLER_H
#define MEDIASESSIONCONTROLLER_H

#include <QObject>
#include <QUdpSocket>
#include "AudioManager.h"

class MediaSessionController : public QObject {
    Q_OBJECT
public:
    explicit MediaSessionController(QObject *parent = nullptr);

    // 初始化UDP (登录后)
    void initUdp(QString myName, QString serverIp);
    // 设置对方名字
    void setTarget(QString targetName);

    // 通话控制
    void startCall(); // 开启麦克风
    void stopCall();  // 关闭

    // 兼容接口 (UDP模式下该函数为空)
    void playIncomingStream(const QByteArray &data) { Q_UNUSED(data); }

private slots:
    void onAudioData(QByteArray data); // 麦克风 -> UDP
    void onUdpReadyRead();             // UDP -> 扬声器

private:
    AudioManager *m_audio;
    QUdpSocket *m_udpSocket;
    QString m_serverIp;
    QString m_myName;
    QString m_targetName;
    int m_serverPort = 9998;
};

#endif // MEDIASESSIONCONTROLLER_H
