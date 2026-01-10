#pragma once
#include <QObject>
#include <qqmlintegration.h>

class ComplianceController : public QObject {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ComplianceController(QObject *parent = nullptr);

    QString checkAndFilter(const QString &text);
};
