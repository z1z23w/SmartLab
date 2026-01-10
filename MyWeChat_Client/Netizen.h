#pragma once
#include <QObject>
#include <QQmlEngine>

class Netizen : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY dataChanged)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY dataChanged)
public:
    explicit Netizen(QObject *parent = nullptr) : QObject(parent) {}
    QString username() const { return m_username; }
    void setUsername(const QString &u) { if(m_username!=u){m_username=u; emit dataChanged();} }
    QString status() const { return m_status; }
    void setStatus(const QString &s) { if(m_status!=s){m_status=s; emit dataChanged();} }
signals:
    void dataChanged();
private:
    QString m_username;
    QString m_status = "Offline";
};
