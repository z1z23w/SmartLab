#include "Netizen.h"

Netizen::Netizen(QObject *parent)
    : QObject(parent)  // 调用父类构造函数
{

}

QString Netizen::username() const {
    return m_username;
}

void Netizen::setUsername(const QString &u) {
    if (m_username != u) {
        m_username = u;
        emit dataChanged();
    }
}

QString Netizen::status() const {
    return m_status;
}

void Netizen::setStatus(const QString &s) {
    if (m_status != s) {
        m_status = s;
        emit dataChanged();
    }
}
