#pragma once
#include <QString>
#include <QSqlDatabase>
#include <QJsonArray>
#include <QJsonObject>

class DBManager
{
public:
    static DBManager& instance();
    bool connectToDb();

    //用户账户相关
    bool isUserExists(const QString &username);
    bool registerUser(const QString &username, const QString &password);
    bool loginUser(const QString &username, const QString &password, int &userId);
    int getUserId(const QString &username);
    QString getUsername(int userId);

    //好友相关
    QJsonArray getFriendList(int userId);
    bool addFriend(int userId, const QString &friendName);
    bool deleteFriend(int userId, const QString &friendName);
    bool updateFriendRemark(int userId, const QString &friendName, const QString &remark);
    bool searchUser(const QString &keyword, QJsonObject &result);

    //消息记录相关
    bool saveMessage(const QString &sender, const QString &receiver, const QString &content, const QString &type);
    QJsonArray getChatHistory(const QString &userA, const QString &userB);

    void updateUserStatus(int userId, bool isOnline, QString ip, int port);

private:
    DBManager();
    QSqlDatabase m_db;
};
