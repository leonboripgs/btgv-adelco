#include <QFileInfo>
#include <QWidget>
#include <QQuickItem>
#include <QQmlContext>
#include "uibackend.h"

// !!!tmp #include <systemconfiguration.h>
// !!!tmp #include <batchconfiguration.h>

// TODO:
/*
#include <counterblock.h>
#include <systemstatus.h>
#include <labeledvalue.h>
#include <labeledvaluelist.h>
*/

/*!
 * \brief UIBackend::UIBackend
 * \param st an initialized application settings object
 */
UIBackend::UIBackend(UIController *controller, QSettings *st, QWindow *parent) : QQuickView(parent)
{
    m_controller = controller;
    m_settings = st;

    initializeTypes();
    setVisualProperties();

    // inject controller to the QML context
    rootContext()->setContextProperty("uiController", m_controller);

    // wait for the UI to get ready
    connect(this, SIGNAL(statusChanged(QQuickView::Status)), this, SLOT(onStatusChanged(QQuickView::Status)));
}

/*!
 * \brief UIBackend::~UIBackend
 */
UIBackend::~UIBackend() {
    // TODO:
    // m_imgProvider->deleteLater();
}

/*!
 * \brief UIBackend::onStatusChanged
 * \param status
 */
void UIBackend::onStatusChanged(QQuickView::Status status) {

    if (status != QQuickView::Ready) {
            return;
    }
    m_controller->initialize();

    connect(m_controller, SIGNAL(clearFailedImages()), this, SLOT(onClearFailedImages()) );
}

/*!
 * \brief UIBackend::onClearFailedImages
 */
void UIBackend::onClearFailedImages() {
    // !!!tmp m_bufferImageProvider->clearFailedImages();
}

/**
 * @brief UIBackend::setWinProperties
 */
void UIBackend::setVisualProperties() {
    this->setColor(QColor("#000000"));
    this->setResizeMode(QQuickView::SizeRootObjectToView);
}

/**
 * @brief UIBackend::show
 */
void UIBackend::show() {
    setSource(QUrl(QStringLiteral("qrc:/qml/MainWindow.qml")));
}

/*!
 * \brief UIBackend::getTrainImageProvider
 * \return
 */
// !!!tmp TrainImageProvider *UIBackend::getTrainImageProvider() {
// !!!tmp     return m_trainImageProvider;
// !!!tmp }

/*!
 * \brief UIBackend::getBufferImageProvider
 * \return
 */
// !!!tmp BufferImageProvider *UIBackend::getBufferImageProvider() {
// !!!tmp     return m_bufferImageProvider;
// !!!tmp }

/**
 * @brief UIBackend::initializeTypes
 */
void UIBackend::initializeTypes() {

// !!!tmp     m_bufferImageProvider = new BufferImageProvider();
// !!!tmp     m_trainImageProvider = new TrainImageProvider(this);

// !!!tmp     this->engine()->addImageProvider(QLatin1String("train"), m_trainImageProvider);
// !!!tmp     this->engine()->addImageProvider(QLatin1String("buffer"), m_bufferImageProvider);

// !!!tmp     qmlRegisterType<SystemStatus>("com.darebit.blisterocv",         1, 0, "SystemStatus");
// !!!tmp     qmlRegisterType<SystemConfiguration>("com.darebit.blisterocv",  1, 0, "SystemConfiguration");
// !!!tmp     qmlRegisterType<BatchConfiguration>("com.darebit.blisterocv",   1, 0, "BatchConfiguration");

    // TODO:
    /*
    qmlRegisterType<LabeledValue>("com.darebit.blisterocv",         1, 0, "LabeledValue");
    qmlRegisterType<LabeledValueList>("com.darebit.blisterocv",     1, 0, "LabeledValueList");
    */
}
