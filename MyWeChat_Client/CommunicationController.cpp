#include "CommunicationController.h"
#include <QJsonDocument>

CommunicationController::CommunicationController(QObject *parent) : QObject(parent) {
    m_socket = new QTcpSocket(this);
    m_currentUser = new Netizen(this);
    m_chatSession = new ChatSession(this);
    m_complianceCtrl = new ComplianceController(this);
    m_mediaCtrl = new MediaSessionController(this);

    connect(m_mediaCtrl, &MediaSessionController::mediaStreamGenerated, this, &CommunicationController::onMediaData);
    connect(m_socket, &QTcpSocket::readyRead, this, &CommunicationController::onReadyRead);
    connect(m_socket, &QTcpSocket::connected, this, &CommunicationController::onConnected);
}

bool CommunicationController::isConnected() const {
    return m_socket->state() == QTcpSocket::ConnectedState;
}

void CommunicationController::onConnected() {
    emit connectionStatusChanged();
    if (!m_pendingData.isEmpty()) {
        sendJson(m_pendingData);
        m_pendingData = QJsonObject();
    }
}

void CommunicationController::connectToServer(QString ip) {
    if (m_socket->state() == QTcpSocket::ConnectedState && m_socket->peerAddress().toString() == ip) return;
    m_socket->abort();
    m_socket->connectToHost(ip, 9999);
}

void CommunicationController::login(QString u, QString p) {
    QJsonObject o; o["type"]="login"; o["username"]=u; o["password"]=p;
    sendJson(o);
}

void CommunicationController::registerUser(QString u, QString p) {
    QJsonObject o; o["type"]="register"; o["username"]=u; o["password"]=p;
    sendJson(o);
}

// 【修改】返回过滤文本
QString CommunicationController::sendMessage(QString content) {
    if (m_chatSession->currentTarget().isEmpty()) {
        emit notificationTriggered("错误", "未选择好友");
        return "";
    }

    // 1. 过滤
    QString safeText = m_complianceCtrl->checkAndFilter(content);

    // 2. 发送
    QJsonObject o; o["type"]="msg"; o["to"]=m_chatSession->currentTarget(); o["content"]=safeText;
    sendJson(o);

    // 3. 返回给 UI 本地显示
    return safeText;
}

// 【新增】请求历史记录
void CommunicationController::getHistory(QString friendName) {
    QJsonObject o;
    o["type"] = "get_history";
    o["friend_name"] = friendName;
    sendJson(o);
}

// 【新增】清除红点
void CommunicationController::clearUnread(QString friendName) {
    for (int i = 0; i < m_friendList.size(); ++i) {
        QJsonObject f = m_friendList[i].toObject();
        if (f["username"].toString() == friendName) {
            if (f.contains("unread")) {
                f.remove("unread");
                m_friendList[i] = f;
                emit friendListChanged();
            }
            break;
        }
    }
}

void CommunicationController::startMediaSession() {
    if (m_chatSession->currentTarget().isEmpty()) return;
    m_mediaCtrl->startSession();
}

void CommunicationController::endMediaSession() {
    m_mediaCtrl->endSession();
}

void CommunicationController::onMediaData(QByteArray data) {
    if (m_chatSession->currentTarget().isEmpty()) return;
    QJsonObject o; o["type"]="voice"; o["to"]=m_chatSession->currentTarget();
    o["content"]=QString::fromLatin1(data.toBase64());
    sendJson(o);
}

void CommunicationController::selectFriend(QString name) {
    m_chatSession->setCurrentTarget(name);
}

void CommunicationController::searchUser(QString keyword) {
    QJsonObject o; o["type"]="search_user"; o["keyword"]=keyword;
    sendJson(o);
}

void CommunicationController::addFriend(QString friendName) {
    QJsonObject o; o["type"]="add_friend"; o["friend_name"]=friendName;
    sendJson(o);
}

void CommunicationController::sendJson(const QJsonObject &json) {
    if (isConnected()) {
        m_socket->write(QJsonDocument(json).toJson());
        m_socket->flush();
    } else {
        m_pendingData = json;
        if (m_socket->state() == QTcpSocket::UnconnectedState)
            emit notificationTriggered("提示", "正在连接服务器...");
    }
}

void CommunicationController::onReadyRead() {
    QByteArray data = m_socket->readAll();
    handleData(data);
}

void CommunicationController::handleData(const QByteArray &data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) return;
    QJsonObject obj = doc.object();
    QString type = obj.value("type").toString();

    if (type == "login_resp") {
        if (obj.value("success").toBool()) {
            m_currentUser->setUsername(obj.value("username").toString());
            m_currentUser->setStatus("Online");
            emit notificationTriggered("系统", "登录成功");
        } else emit notificationTriggered("系统", "登录失败");
    }
    else if (type == "msg") {
        QString from = obj.value("from").toString();
        QString content = obj.value("content").toString();

        bool isCurrentChat = (m_chatSession->currentTarget() == from);

        // 1. 如果不是当前正在聊的人，就标记红点
        if (!isCurrentChat) {
            for (int i = 0; i < m_friendList.size(); ++i) {
                QJsonObject f = m_friendList[i].toObject();
                if (f["username"].toString() == from) {
                    f["unread"] = true;
                    m_friendList[i] = f;
                    emit friendListChanged();
                    break;
                }
            }
        }

        // 2. 【关键修改】无论是不是当前聊天，都要通知 UI
        // 这样 UI 才能决定是 "上屏" 还是 "弹窗"
        emit messageReceived(from, content);
    }
    else if (type == "history_resp") {
        // 收到历史记录
        QString friendName = obj.value("friend_name").toString();
        QJsonArray history = obj.value("history").toArray();
        if (m_chatSession->currentTarget() == friendName) {
            emit historyLoaded(friendName, history);
        }
    }
    else if (type == "voice") {
        QByteArray d = QByteArray::fromBase64(obj.value("content").toString().toLatin1());
        m_mediaCtrl->playIncomingStream(d);
    }
    else if (type == "friend_list") {
        m_friendList = obj.value("friends").toArray();
        emit friendListChanged();
    }
    else if (type == "search_user_resp") {
        if (obj.value("found").toBool()) emit notificationTriggered("搜索", "找到用户: "+obj.value("user").toObject().value("username").toString());
        else emit notificationTriggered("搜索", "用户不存在");
    }
}
