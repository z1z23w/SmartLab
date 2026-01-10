#pragma once
#include <QQuickPaintedItem>
#include <QImage>
#include <QMutex> //互斥锁

class VideoItem : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit VideoItem(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;

public slots:
    void updateFrame(const QImage &frame);

private:
    QImage m_currentImage;
    QMutex m_mutex; // 保护m_currentImage的读写
};
