#include "ChatSession.h"

ChatSession::ChatSession(QObject *parent)
    : QObject(parent)  // 初始化父类
{
}

QString ChatSession::currentTarget() const {
    return m_target;
}

void ChatSession::setCurrentTarget(const QString &t) {
    if (m_target != t) {
        m_target = t;
        emit targetChanged();
    }
}
