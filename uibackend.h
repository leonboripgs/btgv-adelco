#ifndef UIBACKEND_H
#define UIBACKEND_H

#include <QObject>
#include <QQuickView>
#include <QSettings>

#include <uicontroller.h>
// #include <trainimageprovider.h> // !!!tmp
// #include <bufferimageprovider.h> // !!!tmp

class UIBackend : public QQuickView
{
    Q_OBJECT

public:
    UIBackend(UIController*, QSettings*, QWindow *parent = 0);
    ~UIBackend();

    void show();

    // !!!tmp TrainImageProvider *getTrainImageProvider();
    // !!!tmp BufferImageProvider *getBufferImageProvider();

private:
    QSettings       *m_settings;
    UIController    *m_controller;

    void setVisualProperties();
    void initializeTypes();

    // !!!tmp TrainImageProvider *m_trainImageProvider;
    // !!!tmp BufferImageProvider *m_bufferImageProvider;

private slots:
    void onStatusChanged(QQuickView::Status);
    void onClearFailedImages();
};

#endif // UIBACKEND_H
