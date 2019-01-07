#ifndef UIBACKEND_H
#define UIBACKEND_H

#include <QObject>
#include <QQuickView>
#include <QSettings>

#include <uicontroller.h>
#include <trainimageprovider.h>
#include <bufferimageprovider.h>

class UIBackend : public QQuickView
{
    Q_OBJECT

public:
    UIBackend(UIController*, QSettings*, TrainImageProvider*, BufferImageProvider*, QWindow *parent = 0);
    ~UIBackend();

    void show();

    TrainImageProvider *getTrainImageProvider();
    BufferImageProvider *getBufferImageProvider();

private:
    QSettings       *m_settings;
    UIController    *m_controller;

    void setVisualProperties();
    void initializeTypes();

    TrainImageProvider *m_trainImageProvider;
    BufferImageProvider *m_bufferImageProvider;

private slots:
    void onStatusChanged(QQuickView::Status);
    void onClearFailedImages();
};

#endif // UIBACKEND_H
