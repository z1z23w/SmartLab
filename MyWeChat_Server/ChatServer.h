#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QObject>
#include <QMap>
#include <QJsonObject>

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);
    bool start(int port);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

    //UDP读取槽函数
    void onUdpReadyRead();

private:
    QTcpServer *m_server;

    //UDP Socket及客户端地址映射
    QUdpSocket *m_udpSocket;
    struct UserAddress {
        QHostAddress ip;
        quint16 port;
    };
    QMap<QString, UserAddress> m_udpClients;

    //TCP相关
    QMap<QString, QTcpSocket*> m_onlineUsers;
    QMap<QTcpSocket*, QString> m_socketToUser;

    void sendJson(QTcpSocket *socket, const QJsonObject &obj);

    //业务处理
    void handleRegister(QTcpSocket *socket, const QJsonObject &req);
    void handleLogin(QTcpSocket *socket, const QJsonObject &req);
    void handleMessage(QTcpSocket *socket, const QJsonObject &req);
    void handleSearchUser(QTcpSocket *socket, const QJsonObject &req);
    void handleAddFriend(QTcpSocket *socket, const QJsonObject &req);
    void handleGetHistory(QTcpSocket *socket, const QJsonObject &req);

    void sendFriendList(QTcpSocket *socket, const QString &username);
};
