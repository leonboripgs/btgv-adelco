#ifndef BUFFERIMAGEPROVIDER_H
#define BUFFERIMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QQueue>
#include <QDateTime>
#include <QDebug>
#include <QPainter>
#include <QFile>

class BufferImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit BufferImageProvider(QObject *parent = 0);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
    void clearFailedImages();

signals:
    void newImage(bool);

public slots:
    void appendPixmap(QPixmap*, bool);

private:
    QQueue<QPixmap> failedPixmaps;
    QPixmap pixmap;
    QPixmap imageBackup;
    void saveToFile(QPixmap pixmap, QString fname);
    void initializeFailedImages();
};

#endif // BUFFERIMAGEPROVIDER_H
