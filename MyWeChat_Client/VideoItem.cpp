#include "VideoItem.h"
#include <QPainter>
#include <QMutexLocker>

VideoItem::VideoItem(QQuickItem *parent) : QQuickPaintedItem(parent) {
}

void VideoItem::paint(QPainter *painter) {
    QImage imgToDraw;
    {
        //加锁拷贝，快速释放锁，避免阻塞渲染线程
        QMutexLocker locker(&m_mutex);
        if (m_currentImage.isNull()) {
            painter->fillRect(boundingRect(), Qt::black);
            return;
        }
        imgToDraw = m_currentImage;
    }

    QRectF target = boundingRect();
    //使用FastTransformation，优先流畅度
    QImage scaled = imgToDraw.scaled(target.width(), target.height(),
                                     Qt::KeepAspectRatio, Qt::FastTransformation);

    double x = (target.width() - scaled.width()) / 2;
    double y = (target.height() - scaled.height()) / 2;

    painter->drawImage(QPointF(x, y), scaled);
}

void VideoItem::updateFrame(const QImage &frame) {
    {
        QMutexLocker locker(&m_mutex);
        //深拷贝,切断与源数据的内存关联，防止源数据在另一线程被修改
        m_currentImage = frame.copy();
    }
    update();
}
