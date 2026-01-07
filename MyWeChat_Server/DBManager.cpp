#include "DBManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

DBManager::DBManager() {}

DBManager& DBManager::instance() {
    static DBManager instance;
    return instance;
}

bool DBManager::connectToDb() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/../../../wechat.db");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qDebug() << "❌ 数据库连接失败:" << m_db.lastError().text();
        return false;
    }
    qDebug() << "✅ 数据库连接成功! 文件路径:" << dbPath;

    QSqlQuery query;
    // 1. 用户表
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            is_online INTEGER DEFAULT 0,
            ip TEXT, port INTEGER
        )
    )");

    // 2. 好友关系表
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS friends (
            user_id INTEGER,
            friend_id INTEGER,
            remark TEXT,
            PRIMARY KEY(user_id, friend_id)
        )
    )");

    // 3. 【新增】消息记录表
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS messages (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            sender TEXT,
            receiver TEXT,
            content TEXT,
            msg_type TEXT,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )");

    return true;
}

// ... (registerUser, loginUser, getUserId 等保持不变，为节省篇幅略过，请保留原代码) ...
// ... 请保留原有的 用户/好友相关 函数实现 ...

bool DBManager::isUserExists(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = :u");
    query.bindValue(":u", username);
    return query.exec() && query.next();
}

bool DBManager::registerUser(const QString &username, const QString &password) {
    if (isUserExists(username)) return false;
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:u, :p)");
    query.bindValue(":u", username);
    query.bindValue(":p", password);
    return query.exec();
}

bool DBManager::loginUser(const QString &username, const QString &password, int &userId) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = :u AND password = :p");
    query.bindValue(":u", username);
    query.bindValue(":p", password);
    if (query.exec() && query.next()) {
        userId = query.value("id").toInt();
        return true;
    }
    return false;
}

int DBManager::getUserId(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = :u");
    query.bindValue(":u", username);
    if (query.exec() && query.next()) return query.value("id").toInt();
    return -1;
}

QString DBManager::getUsername(int userId) {
    QSqlQuery query;
    query.prepare("SELECT username FROM users WHERE id = :i");
    query.bindValue(":i", userId);
    if (query.exec() && query.next()) return query.value("username").toString();
    return "";
}

// ... (getFriendList, addFriend 等保持不变) ...
QJsonArray DBManager::getFriendList(int userId) {
    QJsonArray list;
    QSqlQuery query;
    query.prepare(R"(
        SELECT u.username, f.remark
        FROM friends f
        JOIN users u ON f.friend_id = u.id
        WHERE f.user_id = :uid
    )");
    query.bindValue(":uid", userId);
    query.exec();
    while (query.next()) {
        QJsonObject obj;
        obj["username"] = query.value("username").toString();
        obj["remark"] = query.value("remark").toString();
        list.append(obj);
    }
    return list;
}

bool DBManager::addFriend(int userId, const QString &friendName) {
    int friendId = getUserId(friendName);
    if (friendId == -1 || friendId == userId) return false;

    m_db.transaction();
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO friends (user_id, friend_id) VALUES (:u, :f)");
    query.bindValue(":u", userId); query.bindValue(":f", friendId);
    bool ok1 = query.exec();

    query.bindValue(":u", friendId); query.bindValue(":f", userId);
    bool ok2 = query.exec();

    if (ok1 && ok2) { m_db.commit(); return true; }
    m_db.rollback(); return false;
}

bool DBManager::searchUser(const QString &keyword, QJsonObject &result) {
    if (isUserExists(keyword)) {
        result["username"] = keyword;
        return true;
    }
    return false;
}

// --- 【新增】消息记录实现 ---

bool DBManager::saveMessage(const QString &sender, const QString &receiver, const QString &content, const QString &type) {
    QSqlQuery query;
    query.prepare("INSERT INTO messages (sender, receiver, content, msg_type) VALUES (:s, :r, :c, :t)");
    query.bindValue(":s", sender);
    query.bindValue(":r", receiver);
    query.bindValue(":c", content);
    query.bindValue(":t", type);
    if (!query.exec()) {
        qDebug() << "Save msg error:" << query.lastError().text();
        return false;
    }
    return true;
}

QJsonArray DBManager::getChatHistory(const QString &userA, const QString &userB) {
    QSqlQuery query;
    // 获取 A和B 之间的所有消息
    query.prepare(R"(
        SELECT sender, content, msg_type FROM messages
        WHERE (sender=:u1 AND receiver=:u2) OR (sender=:u2 AND receiver=:u1)
        ORDER BY id ASC
    )");
    query.bindValue(":u1", userA);
    query.bindValue(":u2", userB);
    query.exec();

    QJsonArray history;
    while (query.next()) {
        QJsonObject msg;
        // 标记消息是谁发的 (me / other)，方便客户端显示
        msg["sender"] = (query.value("sender").toString() == userA) ? "me" : "other";
        msg["real_sender"] = query.value("sender").toString();
        msg["text"] = query.value("content").toString();
        msg["type"] = query.value("msg_type").toString();
        history.append(msg);
    }
    return history;
}

// ... 其他未列出的函数请保持原样 ...
bool DBManager::deleteFriend(int, const QString&) { return false; } // 占位
bool DBManager::updateFriendRemark(int, const QString&, const QString&) { return false; } // 占位
void DBManager::updateUserStatus(int, bool, QString, int) {} // 占位
