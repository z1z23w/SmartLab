#ifndef COMMUNICATIONCONTROLLER_H
#define COMMUNICATIONCONTROLLER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonObject>
#include "Netizen.h"
#include "ChatSession.h"
#include "ComplianceController.h"
#include "MediaSessionController.h"

class CommunicationController : public QObject {
    Q_OBJECT
    Q_PROPERTY(Netizen* currentUser READ currentUser CONSTANT)
    Q_PROPERTY(ChatSession* currentSession READ currentSession CONSTANT)
    Q_PROPERTY(QJsonArray friendList READ friendList NOTIFY friendListChanged)
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionStatusChanged)

public:
    explicit CommunicationController(QObject *parent = nullptr);

    Q_INVOKABLE void connectToServer(QString ip);
    Q_INVOKABLE void login(QString u, QString p);
    Q_INVOKABLE void registerUser(QString u, QString p);
    Q_INVOKABLE QString sendMessage(QString content);

    // 【新增】呼叫控制接口
    Q_INVOKABLE void requestCall(QString targetUser);
    Q_INVOKABLE void acceptCall();
    Q_INVOKABLE void rejectCall();
    Q_INVOKABLE void endCall();

    Q_INVOKABLE void selectFriend(QString name);
    Q_INVOKABLE void searchUser(QString keyword);
    Q_INVOKABLE void addFriend(QString friendName);
    Q_INVOKABLE void getHistory(QString friendName);
    Q_INVOKABLE void clearUnread(QString friendName);

    Netizen* currentUser() const { return m_currentUser; }
    ChatSession* currentSession() const { return m_chatSession; }
    QJsonArray friendList() const { return m_friendList; }
    bool isConnected() const;

signals:
    void friendListChanged();
    void connectionStatusChanged();
    void messageReceived(QString from, QString content);
    void notificationTriggered(QString title, QString msg);
    void historyLoaded(QString friendName, QJsonArray history);

    // 【新增】呼叫相关信号
    void incomingCall(QString fromUser);
    void callAccepted();
    void callRejected();
    void callEnded();

private slots:
    void onReadyRead();
    void onConnected();

private:
    QTcpSocket *m_socket;
    Netizen *m_currentUser;
    ChatSession *m_chatSession;
    ComplianceController *m_complianceCtrl;
    MediaSessionController *m_mediaCtrl;
    QJsonArray m_friendList;
    QJsonObject m_pendingData;

    // 记录通话对象
    QString m_currentCallTarget;

    void sendJson(const QJsonObject &json);
    void handleData(const QByteArray &data);
};

#endif // COMMUNICATIONCONTROLLER_H
