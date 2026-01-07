#include "ChatServer.h"
#include "DBManager.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

ChatServer::ChatServer(QObject *parent) : QObject(parent), m_server(new QTcpServer(this)) {}

bool ChatServer::start(int port) {
    if (!m_server->listen(QHostAddress::Any, port)) return false;
    qDebug() << "服务器监听中... 端口:" << port;
    connect(m_server, &QTcpServer::newConnection, this, &ChatServer::onNewConnection);
    return true;
}

void ChatServer::onNewConnection() {
    QTcpSocket *clientSocket = m_server->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::onClientDisconnected);
    qDebug() << "新连接:" << clientSocket->peerAddress().toString();
}

void ChatServer::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;
    QByteArray data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) return;
    QJsonObject obj = doc.object();
    QString type = obj.value("type").toString();

    if (type == "login") handleLogin(socket, obj);
    else if (type == "register") handleRegister(socket, obj);
    else if (type == "msg") handleMessage(socket, obj);
    else if (type == "voice") handleVoice(socket, obj);
    else if (type == "search_user") handleSearchUser(socket, obj);
    else if (type == "add_friend") handleAddFriend(socket, obj);
    else if (type == "get_history") handleGetHistory(socket, obj); // 【新增】
}

// ... handleLogin, handleRegister 保持不变 ...
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

    // 【新增】保存到数据库
    DBManager::instance().saveMessage(from, to, content, "msg");

    if (m_onlineUsers.contains(to)) {
        QJsonObject fwd = req;
        fwd["from"] = from;
        sendJson(m_onlineUsers[to], fwd);
    }
}

void ChatServer::handleVoice(QTcpSocket *socket, const QJsonObject &req) {
    QString to = req.value("to").toString();
    QString from = m_socketToUser.value(socket);
    QString content = req.value("content").toString(); // Base64

    // 【新增】保存语音
    DBManager::instance().saveMessage(from, to, content, "voice");

    if (m_onlineUsers.contains(to)) {
        QJsonObject fwd = req;
        fwd["from"] = from;
        sendJson(m_onlineUsers[to], fwd);
    }
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

// 【新增】处理获取历史记录请求
void ChatServer::handleGetHistory(QTcpSocket *socket, const QJsonObject &req) {
    QString me = m_socketToUser.value(socket);
    QString friendName = req.value("friend_name").toString();

    QJsonArray history = DBManager::instance().getChatHistory(me, friendName);

    // 修正 sender，让客户端知道是 "me" 还是 "other"
    QJsonArray finalHistory;
    for(auto item : history) {
        QJsonObject obj = item.toObject();
        obj["sender"] = (obj["real_sender"].toString() == me) ? "me" : "other";
        finalHistory.append(obj);
    }

    QJsonObject resp;
    resp["type"] = "history_resp";
    resp["friend_name"] = friendName;
    resp["history"] = finalHistory;
    sendJson(socket, resp);
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
    }
    socket->deleteLater();
}

void ChatServer::sendJson(QTcpSocket *socket, const QJsonObject &obj) {
    QJsonDocument doc(obj);
    socket->write(doc.toJson());
    socket->flush();
}
