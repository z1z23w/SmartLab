#pragma once
#include <QObject>
#include <QQmlEngine>

class ChatSession : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString currentTarget READ currentTarget WRITE setCurrentTarget NOTIFY targetChanged)

public:
    explicit ChatSession(QObject *parent = nullptr);
    QString currentTarget() const;
    void setCurrentTarget(const QString &t);

signals:
    void targetChanged();

private:
    QString m_target;
};
