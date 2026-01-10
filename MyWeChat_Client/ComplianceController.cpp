#include "ComplianceController.h"

ComplianceController::ComplianceController(QObject *parent) : QObject(parent) {
}

QString ComplianceController::checkAndFilter(const QString &text) {
    QString filtered = text;
    //敏感词库
    QStringList badWords = {"笨蛋", "傻瓜", "坏人", "垃圾", "sb"};

    for (const QString &word : badWords) {
        //将敏感词替换为等长的星号
        filtered.replace(word, QString(word.length(), '*'));
    }
    return filtered;
}
