#include "ChatServer.h"
#include "DBManager.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QNetworkDatagram>

ChatServer::ChatServer(QObject *parent) : QObject(parent), m_server(new QTcpServer(this)) {
    // 初始化 UDP
    m_udpSocket = new QUdpSocket(this);
    if (m_udpSocket->bind(QHostAddress::Any, 9998)) {
        qDebug() << "✅ UDP 音频服务监听中... 端口: 9998";
        connect(m_udpSocket, &QUdpSocket::readyRead, this, &ChatServer::onUdpReadyRead);
    } else {
        qDebug() << "❌ UDP 绑定失败 (端口可能被占用)";
    }
}

bool ChatServer::start(int port) {
    if (!m_server->listen(QHostAddress::Any, port)) return false;
    qDebug() << "✅ TCP 信令服务监听中... 端口:" << port;
    connect(m_server, &QTcpServer::newConnection, this, &ChatServer::onNewConnection);
    return true;
}

void ChatServer::onNewConnection() {
    QTcpSocket *clientSocket = m_server->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::onClientDisconnected);
    qDebug() << "TCP 新连接:" << clientSocket->peerAddress().toString();
}

// --- UDP 转发逻辑 (核心) ---
void ChatServer::onUdpReadyRead() {
    while (m_udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
        QByteArray data = datagram.data();

        // 协议: [类型 1byte] ...
        if (data.size() < 2) continue;
        char type = data[0];

        // 0x01: 注册包 [0x01][NameLen][Name]
        if (type == 0x01) {
            int nameLen = (unsigned char)data[1];
            if (data.size() < 2 + nameLen) continue;
            QString username = QString::fromUtf8(data.mid(2, nameLen));
            // 记录该用户的公网 IP 和 Port
            m_udpClients[username] = {datagram.senderAddress(), (quint16)datagram.senderPort()};
        }
        // 0x02: 音频包 [0x02][TargetNameLen][TargetName][PCM数据]
        else if (type == 0x02) {
            int targetLen = (unsigned char)data[1];
            if (data.size() < 2 + targetLen) continue;

            QString targetName = QString::fromUtf8(data.mid(2, targetLen));
            QByteArray audioContent = data.mid(2 + targetLen);

            // 查表转发
            if (m_udpClients.contains(targetName)) {
                UserAddress addr = m_udpClients[targetName];
                // 直接转发 PCM 数据给目标 (目标直接播放)
                m_udpSocket->writeDatagram(audioContent, addr.ip, addr.port);
            }
        }
    }
}

void ChatServer::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;
    QByteArray data = socket->readAll();

    // 【核心修复】同客户端，防止切碎包含数组的 JSON
    data.replace("}{", "}|{");
    QByteArrayList packets = data.split('|');

    for (QByteArray packet : packets) {
        if (packet.trimmed().isEmpty()) continue;

        QJsonDocument doc = QJsonDocument::fromJson(packet);
        if (doc.isNull()) continue;

        QJsonObject obj = doc.object();
        QString type = obj.value("type").toString();

        if (type == "login") handleLogin(socket, obj);
        else if (type == "register") handleRegister(socket, obj);
        else if (type == "msg") handleMessage(socket, obj);
        else if (type == "search_user") handleSearchUser(socket, obj);
        else if (type == "add_friend") handleAddFriend(socket, obj);
        else if (type == "get_history") handleGetHistory(socket, obj);

        // 通话信令转发
        else if (type == "call_request" || type == "call_response" || type == "call_end") {
            QString to = obj.value("to").toString();
            if (m_onlineUsers.contains(to)) {
                sendJson(m_onlineUsers[to], obj);
            }
        }
    }
}

// ... 常规业务逻辑保持不变 ...
void ChatServer::handleLogin(QTcpSocket *socket, const QJsonObject &req) {
    QString u = req.value("username").toString();
    QString p = req.value("password").toString();
    QJsonObject resp; resp["type"] = "login_resp";
    int userId = -1;
    if (DBManager::instance().loginUser(u, p, userId)) {
        resp["success"] = true;
        resp["username"] = u;
        m_onlineUsers[u] = socket;
        m_socketToUser[socket] = u;
        sendJson(socket, resp);
        sendFriendList(socket, u);
    } else {
        resp["success"] = false; resp["message"] = "Login Failed";
        sendJson(socket, resp);
    }
}

void ChatServer::handleRegister(QTcpSocket *socket, const QJsonObject &req) {
    QString u = req.value("username").toString();
    QString p = req.value("password").toString();
    bool ok = DBManager::instance().registerUser(u, p);
    QJsonObject resp; resp["type"] = "register_resp"; resp["success"] = ok;
    resp["message"] = ok ? "Register Success" : "User Exists";
    sendJson(socket, resp);
}

void ChatServer::handleMessage(QTcpSocket *socket, const QJsonObject &req) {
    QString to = req.value("to").toString();
    QString from = m_socketToUser.value(socket);
    QString content = req.value("content").toString();
    DBManager::instance().saveMessage(from, to, content, "msg");
    if (m_onlineUsers.contains(to)) {
        QJsonObject fwd = req; fwd["from"] = from;
        sendJson(m_onlineUsers[to], fwd);
    }
}

void ChatServer::handleGetHistory(QTcpSocket *socket, const QJsonObject &req) {
    QString me = m_socketToUser.value(socket);
    QString friendName = req.value("friend_name").toString();
    QJsonArray history = DBManager::instance().getChatHistory(me, friendName);
    QJsonArray finalHistory;
    for(auto item : history) {
        QJsonObject obj = item.toObject();
        obj["sender"] = (obj["real_sender"].toString() == me) ? "me" : "other";
        finalHistory.append(obj);
    }
    QJsonObject resp; resp["type"] = "history_resp";
    resp["friend_name"] = friendName; resp["history"] = finalHistory;
    sendJson(socket, resp);
}

void ChatServer::handleSearchUser(QTcpSocket *socket, const QJsonObject &req) {
    QString keyword = req.value("keyword").toString();
    QJsonObject result;
    bool found = DBManager::instance().searchUser(keyword, result);
    QJsonObject resp; resp["type"] = "search_user_resp"; resp["found"] = found;
    if(found) resp["user"] = result;
    sendJson(socket, resp);
}

void ChatServer::handleAddFriend(QTcpSocket *socket, const QJsonObject &req) {
    QString from = m_socketToUser.value(socket);
    QString to = req.value("friend_name").toString();
    int myId = DBManager::instance().getUserId(from);
    DBManager::instance().addFriend(myId, to);
    sendFriendList(socket, from);
    if(m_onlineUsers.contains(to)) sendFriendList(m_onlineUsers[to], to);
}

void ChatServer::sendFriendList(QTcpSocket *socket, const QString &username) {
    int id = DBManager::instance().getUserId(username);
    QJsonArray list = DBManager::instance().getFriendList(id);
    QJsonObject pkg; pkg["type"] = "friend_list"; pkg["friends"] = list;
    sendJson(socket, pkg);
}

void ChatServer::onClientDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket && m_socketToUser.contains(socket)) {
        QString u = m_socketToUser[socket];
        m_onlineUsers.remove(u);
        m_socketToUser.remove(socket);
        m_udpClients.remove(u); // 清理 UDP 记录
    }
    socket->deleteLater();
}

void ChatServer::sendJson(QTcpSocket *socket, const QJsonObject &obj) {
    QJsonDocument doc(obj);
    socket->write(doc.toJson());
    socket->flush();
}
