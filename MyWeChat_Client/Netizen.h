#pragma once
#include <QObject>
#include <QQmlEngine>

class Netizen : public QObject {
    Q_OBJECT
    QML_ELEMENT  // 供QML访问的标记
    // 声明属性，指定读写函数和通知信号
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY dataChanged)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY dataChanged)

public:
    // 构造函数声明
    explicit Netizen(QObject *parent = nullptr);

    // 读函数声明
    QString username() const;
    QString status() const;

    // 写函数声明
    void setUsername(const QString &u);
    void setStatus(const QString &s);

signals:
    // 数据变更通知信号
    void dataChanged();

private:
    // 私有成员变量
    QString m_username;
    QString m_status = "Offline";  // 初始化为离线状态
};
