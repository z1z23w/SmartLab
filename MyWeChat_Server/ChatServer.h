#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
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

private:
    QTcpServer *m_server;
    QMap<QString, QTcpSocket*> m_onlineUsers;
    QMap<QTcpSocket*, QString> m_socketToUser;

    void sendJson(QTcpSocket *socket, const QJsonObject &obj);

    void handleRegister(QTcpSocket *socket, const QJsonObject &req);
    void handleLogin(QTcpSocket *socket, const QJsonObject &req);
    void handleMessage(QTcpSocket *socket, const QJsonObject &req);
    void handleVoice(QTcpSocket *socket, const QJsonObject &req); // 语音
    void handleSearchUser(QTcpSocket *socket, const QJsonObject &req);
    void handleAddFriend(QTcpSocket *socket, const QJsonObject &req);

    // 【新增】获取历史记录
    void handleGetHistory(QTcpSocket *socket, const QJsonObject &req);

    void sendFriendList(QTcpSocket *socket, const QString &username);
};

#endif // CHATSERVER_H
