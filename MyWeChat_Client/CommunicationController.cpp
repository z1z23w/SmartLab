#include "CommunicationController.h"
#include <QJsonDocument>
#include <QDebug>

CommunicationController::CommunicationController(QObject *parent) : QObject(parent) {
    m_socket = new QTcpSocket(this);
    m_currentUser = new Netizen(this);
    m_chatSession = new ChatSession(this);
    m_complianceCtrl = new ComplianceController(this);
    m_mediaCtrl = new MediaSessionController(this);

    connect(m_socket, &QTcpSocket::readyRead, this, &CommunicationController::onReadyRead);
    connect(m_socket, &QTcpSocket::connected, this, &CommunicationController::onConnected);
}

bool CommunicationController::isConnected() const { return m_socket->state() == QTcpSocket::ConnectedState; }
void CommunicationController::onConnected() { emit connectionStatusChanged(); if (!m_pendingData.isEmpty()) { sendJson(m_pendingData); m_pendingData = QJsonObject(); } }
void CommunicationController::connectToServer(QString ip) { if (m_socket->state() == QTcpSocket::ConnectedState && m_socket->peerAddress().toString() == ip) return; m_socket->abort(); m_socket->connectToHost(ip, 9999); }
void CommunicationController::login(QString u, QString p) { QJsonObject o; o["type"]="login"; o["username"]=u; o["password"]=p; sendJson(o); }
void CommunicationController::registerUser(QString u, QString p) { QJsonObject o; o["type"]="register"; o["username"]=u; o["password"]=p; sendJson(o); }
QString CommunicationController::sendMessage(QString content) { if (m_chatSession->currentTarget().isEmpty()) return ""; QString safeText = m_complianceCtrl->checkAndFilter(content); QJsonObject o; o["type"]="msg"; o["to"]=m_chatSession->currentTarget(); o["content"]=safeText; sendJson(o); return safeText; }
void CommunicationController::selectFriend(QString name) { m_chatSession->setCurrentTarget(name); }
void CommunicationController::getHistory(QString friendName) { QJsonObject o; o["type"] = "get_history"; o["friend_name"] = friendName; sendJson(o); }
void CommunicationController::clearUnread(QString friendName) { for (int i = 0; i < m_friendList.size(); ++i) { QJsonObject f = m_friendList[i].toObject(); if (f["username"].toString() == friendName) { if (f.contains("unread")) { f.remove("unread"); m_friendList[i] = f; emit friendListChanged(); } break; } } }
void CommunicationController::searchUser(QString keyword) { QJsonObject o; o["type"]="search_user"; o["keyword"]=keyword; sendJson(o); }
void CommunicationController::addFriend(QString friendName) { QJsonObject o; o["type"]="add_friend"; o["friend_name"]=friendName; sendJson(o); }
void CommunicationController::sendJson(const QJsonObject &json) { if (isConnected()) { m_socket->write(QJsonDocument(json).toJson()); m_socket->flush(); } else { m_pendingData = json; } }

//呼叫控制

void CommunicationController::requestCall(QString targetUser, QString mode) {
    m_currentCallTarget = targetUser;
    m_currentCallMode = mode;
    QJsonObject o;
    o["type"] = "call_request";
    o["from"] = m_currentUser->username();
    o["to"] = targetUser;
    o["mode"] = mode;
    sendJson(o);
    qDebug() << "Calling" << targetUser << "mode:" << mode;
}

void CommunicationController::acceptCall() {
    qDebug() << "Accepting call from" << m_currentCallTarget;
    m_mediaCtrl->setTarget(m_currentCallTarget);

    //开启硬件
    bool isVideo = (m_currentCallMode == "video");
    m_mediaCtrl->startCall(isVideo);

    //立刻通知本地UI切换状态
    emit callAccepted(m_currentCallMode);

    //发送回执
    QJsonObject o;
    o["type"] = "call_response";
    o["to"] = m_currentCallTarget;
    o["from"] = m_currentUser->username();
    o["response"] = "accept";
    sendJson(o);
}

void CommunicationController::rejectCall() {
    qDebug() << "Rejecting call";
    QJsonObject o;
    o["type"] = "call_response";
    o["to"] = m_currentCallTarget;
    o["from"] = m_currentUser->username();
    o["response"] = "reject";
    sendJson(o);
    m_currentCallTarget = "";
}

void CommunicationController::endCall() {
    qDebug() << "Ending call";
    m_mediaCtrl->stopCall();

    if (!m_currentCallTarget.isEmpty()) {
        QJsonObject o;
        o["type"] = "call_end";
        o["to"] = m_currentCallTarget;
        o["from"] = m_currentUser->username();
        sendJson(o);
    }
    m_currentCallTarget = "";
    emit callEnded(); // 确保本地状态更新
}

//数据接收
void CommunicationController::onReadyRead() {
    QByteArray data = m_socket->readAll();
    data.replace("}{", "}|{");
    QByteArrayList packets = data.split('|');
    for (QByteArray packet : packets) {
        if (packet.trimmed().isEmpty()) continue;
        handleData(packet);
    }
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
            m_mediaCtrl->initUdp(m_currentUser->username(), m_socket->peerAddress().toString());
            emit notificationTriggered("系统", "登录成功");
        } else emit notificationTriggered("系统", "登录失败");
    }
    else if (type == "msg") {
        QString from = obj.value("from").toString();
        QString content = obj.value("content").toString();
        bool isCurrent = (m_chatSession->currentTarget() == from);
        if (!isCurrent) {
            for (int i = 0; i < m_friendList.size(); ++i) {
                QJsonObject f = m_friendList[i].toObject();
                if (f["username"].toString() == from) {
                    f["unread"] = true; m_friendList[i] = f;
                    emit friendListChanged(); break;
                }
            }
        }
        emit messageReceived(from, content);
    }
    else if (type == "call_request") {
        QString from = obj.value("from").toString();
        QString mode = obj.value("mode").toString();
        m_currentCallTarget = from;
        m_currentCallMode = mode;
        qDebug() << "Incoming call from" << from << mode;
        emit incomingCall(from, mode);
    }
    else if (type == "call_response") {
        QString resp = obj.value("response").toString();
        if (resp == "accept") {
            qDebug() << "Call accepted by remote";
            m_mediaCtrl->setTarget(m_currentCallTarget);
            bool isVideo = (m_currentCallMode == "video");
            m_mediaCtrl->startCall(isVideo);
            emit callAccepted(m_currentCallMode);
        } else {
            qDebug() << "Call rejected";
            emit callRejected();
            m_currentCallTarget = "";
        }
    }
    else if (type == "call_end") {
        qDebug() << "Call ended by remote";
        m_mediaCtrl->stopCall();
        emit callEnded();
        m_currentCallTarget = "";
    }
    else if (type == "friend_list") {
        m_friendList = obj.value("friends").toArray();
        emit friendListChanged();
    }
    else if (type == "history_resp") {
        QString friendName = obj.value("friend_name").toString();
        QJsonArray history = obj.value("history").toArray();
        if (m_chatSession->currentTarget() == friendName) emit historyLoaded(friendName, history);
    }
    else if (type == "search_user_resp") {
        if (obj.value("found").toBool()) emit notificationTriggered("搜索", "找到用户: "+obj.value("user").toObject().value("username").toString());
        else emit notificationTriggered("搜索", "用户不存在");
    }
}
