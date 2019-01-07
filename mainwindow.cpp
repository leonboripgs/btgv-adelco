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
    setMinimumWidth(1024);
    setMinimumHeight(768);

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
    m_plcConnection = new PLCConnection(settings, this);
    m_configurationManager = new ConfigurationManager(settings, this);

    BatchConfiguration* batchConfig = new BatchConfiguration();
    m_configurationManager->getConfigurationById(configId, batchConfig);

    m_jobManager = new JobManager(settings, this);
    m_jobManager->setBatchConfiguration(batchConfig);

    m_bufferImageProvider = new BufferImageProvider(this);
    m_trainImageProvider = new TrainImageProvider(this);

    m_controller = new UIController(
                configId,
                m_plcConnection,
                m_configurationManager,
                m_jobManager,
                m_trainImageProvider,
                this);

    m_uiBackend = new UIBackend(m_controller, settings, m_trainImageProvider, m_bufferImageProvider);

    connect(m_jobManager, SIGNAL(trainImageAcquired(QPixmap*)), m_uiBackend->getTrainImageProvider(), SLOT(updatePixmap(QPixmap*)));
    connect(m_jobManager, SIGNAL(newImage(QPixmap*, bool)), m_uiBackend->getBufferImageProvider(), SLOT(appendPixmap(QPixmap*, bool)));
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
    m_bufferImageProvider->deleteLater();
    m_trainImageProvider->deleteLater();

    // m_uiBackend->deleteLater();
    m_uiBackend->close();
    delete m_uiBackend;

    delete m_controller; // ->deleteLater();
}
