#pragma once
#include <QObject>
#include <QQmlEngine>

class ChatSession : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString currentTarget READ currentTarget WRITE setCurrentTarget NOTIFY targetChanged)
public:
    explicit ChatSession(QObject *parent = nullptr) : QObject(parent) {}
    QString currentTarget() const { return m_target; }
    void setCurrentTarget(const QString &t) {
        if(m_target != t) { m_target = t; emit targetChanged(); }
    }
signals:
    void targetChanged();
private:
    QString m_target;
};
