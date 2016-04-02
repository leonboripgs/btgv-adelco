#include "mainwindow.h"
#include <QLayout>

/*!
 * \brief MainWindow::MainWindow The MainWindow class of the application
 * \param settings application wide settings
 * \param parent
 */
MainWindow::MainWindow(QSettings *settings, QWidget *parent)
    : QMainWindow(parent)
{
    // Initialize COM and declare this thread to be an STA thread.
    CoInitialize(NULL);

    // set initial window size
    setFixedWidth(800);
    setFixedHeight(600);

    // set background color to black
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(Pal);

    // get application settings
    // -------------------------------------------------------------------
    QString configId = settings->value("configurationID", "SAMPLE").toString();

    // prepare the main objects and arbitrator
    // -------------------------------------------------------------------
    // !!!tmp m_plcConnection = new PLCConnection(settings, this);
    // !!!tmp m_configurationManager = new ConfigurationManager(settings, this);
    // !!!tmp m_jobManager = new JobManager(settings, this);

    m_controller = new UIController(
                configId,
                // !!!tmp m_plcConnection,
                // !!!tmp m_configurationManager,
                // !!!tmp m_jobManager,
                this);

    m_uiBackend = new UIBackend(m_controller, settings);

    // !!!tmp connect(m_jobManager, SIGNAL(trainImageAcquired(QPixmap*)), m_uiBackend->getTrainImageProvider(), SLOT(updatePixmap(QPixmap*)));
    // !!!tmp connect(m_jobManager, SIGNAL(newImage(QPixmap*, bool)), m_uiBackend->getBufferImageProvider(), SLOT(appendPixmap(QPixmap*, bool)));
    // -------------------------------------------------------------------

    // setup application close handler
    QObject::connect((QObject *)m_uiBackend->engine(), SIGNAL(quit()), this, SLOT(close()));

    // embbed the QML view
    QWidget *container = QWidget::createWindowContainer(m_uiBackend, this);
    setCentralWidget(container);

    // show the UI
    m_uiBackend->show();
}

/*!
 * \brief MainWindow::~MainWindow the destructor of the application main window
 */
MainWindow::~MainWindow()
{    
    // Uninitialize COM
    CoUninitialize();

    // delete objects (clean-up)
    m_uiBackend->deleteLater();
}
