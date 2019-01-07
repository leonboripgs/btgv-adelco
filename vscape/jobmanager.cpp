#include <QDebug>
#include "jobmanager.h"

#include "datumproperties.h"

/**
 * @brief JobManager::JobManager
 * @param path
 * @param avp
 * @param parent
 */
JobManager::JobManager(QSettings *settings, QObject *parent) : QObject(parent)
{
    m_settings = settings;

    // get application settings
    // ------------------------------------------------------------------------------------
    m_settings->beginGroup("Vscape");
    m_avpPath = m_settings->value("avpPath", "c:/btgv-adelco/avp").toString();
    m_cameraName = m_settings->value("camera", "SoftSys1").toString();
    m_settings->endGroup();
    // ------------------------------------------------------------------------------------

    prepareDatums();

    m_avp = "";                         // AVP filename loaded (or to load on startup)
    m_editMode = false;
    m_runMode = false;
    m_loadInProgress = false;
    m_avpLoaded = false;
    m_stepProperties = NULL;

    m_job = new Job(this);
    m_camera = new Camera(m_cameraName, this);
    m_reportConnection = new ReportConnection(m_settings, this);
    m_setupManager = NULL;

    connect(m_camera, SIGNAL(cameraReady()), this, SLOT(onCameraReady()));
    connect(m_camera, SIGNAL(downloadFinished(bool)), this, SLOT(onAvpReady(bool)));

    connect(m_reportConnection, SIGNAL(newImage(QPixmap*, bool)), this, SLOT(onNewImage(QPixmap*, bool)));
    connect(m_reportConnection, SIGNAL(newResult(bool, unsigned char)), this, SLOT(onNewResult(bool, unsigned char)));
    connect(m_reportConnection, SIGNAL(newStatistics(QString)), this, SIGNAL(newStatistics(QString)));
}

/**
 * @brief JobManager::~JobManager
 */
JobManager::~JobManager() {
    cleanUp();

    delete m_setupManager;
    delete m_job;
}

/*!
 * \brief JobManager::getGlobalDatums
 * \return
 */
QList<DatumType> JobManager::getGlobalDatums() {
    updateGlobalDatumValues();
    return m_globalDatums->values();
}

/*!
 * \brief JobManager::prepareGlobalDatums
 */
void JobManager::prepareDatums() {

    // read global datum definitions from settings
    m_settings->beginGroup("Vscape");
    QStringList globalDatumsStr = m_settings->value("globalDatums", "").toStringList();
    QStringList datumDefaultsStr = m_settings->value("datumDefaults", "").toStringList();
    m_settings->endGroup();

    int i;
    QStringList parts;

    // Global Datums
    // ======================================================================================
    m_globalDatums = new QMap<QString, DatumType>();

    for (i = 0; i < globalDatumsStr.length(); i++) {
        parts = globalDatumsStr[i].split("/");
        if (parts.length() < 4) {
            continue;
        }

        QString type = parts[3];

        DatumType datumType;
        datumType.name = parts[0] + ":" + parts[1];
        datumType.label = parts[2];

        if (type.compare("double") == 0) {
            datumType.type = doubleType;
        }
        else if (type.compare("integer") == 0) {
            datumType.type = integerType;
        }

        datumType.scale = 1;
        datumType.minValue = 0;
        datumType.maxValue = INT_MAX;

        QStringList limits;

        switch (datumType.type) {
        case doubleType:
        case integerType:
            if (parts.length() < 5)
                continue;

            limits = parts[4].split("-");
            datumType.scale = QString(limits[0]).toInt();
            datumType.minValue = QString(limits[1]).toInt();
            datumType.maxValue = QString(limits[2]).toInt();
            break;
        default:
            break;
        }

        m_globalDatums->insert(datumType.name, datumType);
    }

    // Custom Datums default values
    // ======================================================================================
    for(i = 0; i < datumDefaultsStr.length(); i++) {
        parts = datumDefaultsStr[i].split("/");
        if (parts.length() < 3) {
            continue;
        }

        QString stepSym = parts[0];
        QString datumSym = parts[1];
        QString datumDefaultValue = parts[2];

        if (g_stepDatums.contains(stepSym)) {
            QList<StepDatum> *datums = &(g_stepDatums[stepSym]);
            for (int di = 0; di < datums->length(); di++) {
                if (QString::compare(datumSym, datums->at(di).symName) == 0) {
                    (*datums)[di].value = QString(datumDefaultValue).toInt();
                    break;
                }
            }
        }
    }
}

/*!
 * \brief JobManager::updateGlobalDatumValues
 */
void JobManager::updateGlobalDatumValues() {

    QMap<QString, DatumType>::iterator i;

    for (i = m_globalDatums->begin(); i != m_globalDatums->end(); ++i) {

        QStringList parts = i.value().name.split(":");

        if (parts.length() < 2) {
            qCritical() << "Invalid code in global datums: " + i.value().name;
            return;
        }

        switch (i.value().type) {
        case doubleType:
            i.value().doubleValue = m_job->getAvpNumericParameter(parts[0], parts[1]).toDouble();
            break;
        case integerType:
            i.value().intValue = m_job->getAvpNumericParameter(parts[0], parts[1]).toInt();
            break;
        default:
            break;
        }
    }
}

/*!
 * \brief JobManager::trainAll
 */
void JobManager::trainAll() {
    m_job->walkJob(m_stepProperties, trainSteps);
}

/*!
 * \brief JobManager::tryOutSteps
 */
void JobManager::tryOutSteps(QString stepPath) {

    QAxObject *system = m_job->getSystemStep();
    QAxObject *step = system->querySubObject(QString("Find(\"" + stepPath + "\", 2)").toLatin1().data());
    if (step != NULL && ! step->isNull()) {

        m_setupManager->tryOut(step);

        delete step;
    }
    delete system;
}

/*!
 * \brief JobManager::train
 * \param stepPath
 */
void JobManager::train(QString stepPath) {
    m_job->train(stepPath);
}

/*!
 * \brief JobManager::trainParent
 * \param stepPath
 */
void JobManager::trainParent(QString stepPath) {
    m_job->trainParent(stepPath);
}

/*!
 * \brief JobManager::tryout
 * \param stepPath
 */
void JobManager::tryout(QString stepPath) {

    QAxObject *system = m_job->getSystemStep();
    QAxObject *step = system->querySubObject(QString("Find(\"" + stepPath + "\", 2)").toLatin1().data());
    if (step != NULL && ! step->isNull()) {

        m_setupManager->tryoutStep(step);

        delete step;
    }
    delete system;
}

/*!
 * \brief JobManager::deleteStep
 * \param stepPath
 */
void JobManager::deleteStep(QString stepPath) {

    QAxObject *system = m_job->getSystemStep();
    QAxObject *step = system->querySubObject(QString("Find(\"" + stepPath + "\", 2)").toLatin1().data());
    if (step != NULL && ! step->isNull()) {

        m_setupManager->disconnectJob();
        m_job->deleteStep(step);
        m_setupManager->connectJob(m_job->getSystemStep());

        delete step;
    }
    delete system;
}

/*!
 * \brief JobManager::getConfigurableSteps
 * \return
 */
QList<StepProps> *JobManager::getConfigurableSteps(bool withUpdate) {

    // check if step properties list is already populated
    if (m_stepProperties != NULL && ! withUpdate) {
        return m_stepProperties;
    }

    if (withUpdate && m_stepProperties != NULL) {
        m_stepProperties->clear();
        delete m_stepProperties;
        m_stepProperties = NULL;
    }

    if (m_stepProperties == NULL) {
        m_stepProperties = new QList<StepProps>();
    }

    // walk Job and execute command
    m_job->walkJob(m_stepProperties, scanForSteps);

    return m_stepProperties;
}

/*!
 * \brief JobManager::getStep
 * \param stepPath
 * \return
 */
StepProps JobManager::getStepProps(QString stepPath) {

    QString cmd;
    StepProps stepProps, parentProps;
    QAxObject *system;
    QAxObject *step;

    cmd = "Find(\"" + stepPath + "\", 2)";

    // check if the object is the root (single symbol name in the path)
    if (stepPath.indexOf(".") < 0) {

        system = m_job->getSystemStep();
        step = system->querySubObject(cmd.toLatin1().data());

        parentProps.level = 1;
        parentProps.path = step->property("NameSym").toString();
        stepProps = StepProperties::fromStepObject(step, parentProps);

        delete step;
        delete system;

        return stepProps;
    }

    stepProps = StepProps { "", "", 0, "", "", 0, false, false, false, false, false, false, false, false, false, false, false, false, false, {0, 0, 0, 0}, false, 0, 0, 0, {} };

    system = m_job->getSystemStep();
    step = system->querySubObject(cmd.toLatin1().data());

    if (step != NULL && ! step->isNull()) {

        QString parentPath = stepPath.left(stepPath.lastIndexOf('.'));
        parentProps = getStepProps(parentPath);

        stepProps = StepProperties::fromStepObject(step, parentProps);
        delete step;
    }
    else {
        qCritical() << "Error trying to get step from path: " << stepPath;
    }

    delete system;

    return stepProps;
}

/*!
 * \brief JobManager::setAvp
 * \param avp
 */
void JobManager::setAvp(QString avp) {
    m_avp = avp;
}

/**
 * @brief JobManager::start
 */
void JobManager::start() {
    m_camera->start();
}

/*!
 * \brief JobManager::saveAVP
 * \param fileName
 */
void JobManager::saveAVP(QString fileName) {

    QAxObject *jobStep = m_job->getJobStep();

    QString avpFilePath = m_avpPath + "/" + fileName;
    jobStep->dynamicCall(QString("SaveAll(\""+ avpFilePath +"\")").toLatin1().data());
    qDebug() << "Saved " + avpFilePath;
}

/**
 * @brief JobManager::loadAVP
 * @param fileName
 */
void JobManager::loadAVP(QString fileName) {
    qDebug() << "*******" << "JobManager::loadAVP";       //created for tracking steps

    m_avpInProgress = fileName;
    QString fullAVPPath = m_avpPath + "/" + fileName;

    if (m_loadInProgress) {
        qDebug() << "! Loading in progress - cannot load " + fullAVPPath;
        return;
    }

    if (m_avpLoaded && m_avp.compare(fileName, Qt::CaseInsensitive) == 0) {
        qDebug() << "! Already loaded - no action necessary: " + fileName;
        emit ready();
        return;
    }

    if (! m_camera->isReady() || m_job == NULL || fileName.isEmpty() || m_avpPath.isEmpty()) {
        qCritical() << "Cannot load AVP file " + fullAVPPath;
        return;
    }

    qDebug() << "> Start loading " + fullAVPPath;
    emit viewMessage("Loading Job: " + fullAVPPath);

    m_loadInProgress = true;

    cleanUp();
    m_job->load(fullAVPPath);

    QAxObject *systemStep = m_job->getSystemStep();
    if (systemStep != NULL && ! systemStep->isNull()) {

        m_camera->download(systemStep);
        delete systemStep;
    }
}

/**
 * @brief JobManager::cleanUp
 */
void JobManager::cleanUp() {

    // clear edit and run Mode so that when requested again to perform the necessary actions
    m_editMode = false;
    m_runMode = false;

    // remove all entries from configurable step list
    if (m_stepProperties != NULL) {
        m_stepProperties->clear();
        delete m_stepProperties;
        m_stepProperties = NULL;
    }

    // disconnect setup manager
    if (m_setupManager != NULL) {
        m_setupManager->cancelAcquireImage();
        m_setupManager->disconnectJob();
    }

    // disconnect reports
    m_reportConnection->disconnectReport();

    //stop all inspections
    if (m_camera != NULL)
        m_camera->stopAllInspections();

    // clear the job from memory
    if (m_job != NULL)
        m_job->removeAllJobs();

    m_avpLoaded = false;
}

/**
 * @brief JobManager::onAvpReady
 * @param success
 */
void JobManager::onAvpReady(bool success) {
    qDebug() << ((success ? "< Download finished ": "Error while downloading job to camera ") + m_avpInProgress);
    qDebug() << "*******" << "JobManager::onAvpReady";       //created for tracking steps

    if (success) {
        m_avpLoaded = true;
        m_avp = m_avpInProgress;
    }
    m_loadInProgress = false;

//    if (m_setupManager == NULL) {
//        m_setupManager = new SetupManager(this);
//        connect(m_setupManager, SIGNAL(trainImageAcquired(QPixmap*)), this, SLOT(onTrainImageAcquired(QPixmap*)));
//    }

    emit ready();
    emit viewMessage("Job loaded: " + m_avp);
}

/**
 * @brief JobManager::setPath
 * @param path
 */
void JobManager::setPath(QString path) {
    m_avpPath = path;
}

/**
 * @brief JobManager::getPath
 * @return
 */
QString JobManager::getPath() {
    return m_avpPath;
}

/**
 * @brief JobManager::onCameraReady
 */
void JobManager::onCameraReady() {
    loadAVP(m_avp);
}

/**
 * @brief JobManager::walkSteps
 */
void JobManager::walkSteps() {
    // m_job->walkJob();
}

/**
 * @brief JobManager::editMode
 */
void JobManager::editMode() {

    if (m_editMode) {
        qInfo() << "Already in Edit Mode - no action required";
        return;
    }
    else {
        qInfo() << "Entering AVP Edit Mode";
    }

    m_reportConnection->disconnectReport();
    m_camera->stopAllInspections();

    if (m_setupManager == NULL) {
        m_setupManager = new SetupManager(this);
        connect(m_setupManager, SIGNAL(trainImageAcquired(QPixmap*)), this, SLOT(onTrainImageAcquired(QPixmap*)));
    }

    m_setupManager->connectJob(m_job->getSystemStep());
    m_editMode = true;
    m_runMode = false;
}

/*!
 * \brief JobManager::runMode
 */
void JobManager::runMode() {

    if (m_runMode) {
        qInfo() << "Already in Run Mode - no action required";
        return;
    }
    else {
        qInfo() << "Entering AVP Run Mode";
    }

    // disconnect setup manager
    if (m_setupManager != NULL) {
        m_setupManager->cancelAcquireImage();
        m_setupManager->disconnectJob();
    }

    // download the job again to the device
    m_camera->download(m_job->getSystemStep());

    // connect reports
    m_reportConnection->connectReport(
                m_camera->getDevice(),
                m_job->getSystemStep(),
                1,
                m_batchConfiguration->getNoOfProducts()
                );

    m_runMode = true;
    m_editMode = false;
}

/*!
 * \brief JobManager::acquireImage
 */
void JobManager::acquireImage() {
    m_setupManager->acquireImage();
}

/*!
 * \brief JobManager::acquireInputImage
 * \param symName
 */
void JobManager::acquireStepImage(QString path, bool useOutput, bool showGraphics) {

    QAxObject *system = m_job->getSystemStep();
    QAxObject *step = system->querySubObject(QString("Find(\"" + path + "\", 2)").toLatin1().data());
    if (step != NULL && ! step->isNull()) {

        m_setupManager->selectStep(step);
        m_setupManager->acquireStepImage(step, useOutput, showGraphics);

        delete step;
    }
    delete system;
}

/*!
 * \brief JobManager::acquireRootImage
 */
void JobManager::acquireRootImage(bool showGraphics) {
    m_setupManager->getImage(showGraphics);
}

/*!
 * \brief JobManager::getImageHSI
 * \param x
 * \param y
 * \return
 */
int* JobManager::getImageHSI(int x, int y) {
    return m_setupManager->getImageHSI(x, y);
}

/*!
 * \brief JobManager::cancelAcquireImage
 */
void JobManager::cancelAcquireImage() {
    m_setupManager->cancelAcquireImage();
}

/*!
 * \brief JobManager::acquireStatus
 */
bool JobManager::acquireStatus() {
    return m_setupManager->acquireStatus();
}

/**
 * @brief JobManager::onTrainImageAcquired
 */
void JobManager::onTrainImageAcquired(QPixmap *image) {
    emit trainImageAcquired(image);
    emit trainReady();
}

/*!
 * \brief JobManager::onNewImage
 * \param image
 * \param result
 */
void JobManager::onNewImage(QPixmap *image, bool result) {
    emit newImage(image, result);
    emit imageReady(result);
}

/**
 * @brief JobManager::getImage
 */
void JobManager::getImage() {
    m_setupManager->getImage();
}

/*!
 * \brief JobManager::updateROI
 * \param stepProps
 */
void JobManager::updateROI(StepProps stepProps) {
    m_job->updateROI(stepProps);
}

/*!
 * \brief JobManager::updateROIs
 * \param steps
 */
void JobManager::updateROIs(QList<StepProps> steps) {
    for (int i = 0; i < steps.size(); i++) {
        m_job->updateROI(steps[i]);
    }
}

/*!
 * \brief JobManager::onNewResult
 * \param result
 */
void JobManager::onNewResult(bool result, unsigned char resultMask) {
    emit newResult(result, resultMask);
}

/*!
 * \brief JobManager::setAvpNumericParam
 * \param code
 * \param value
 */
void JobManager::setAvpNumericParam(QString code, int value) {

    QVariant targetValue;
    double doubleValue;
    int intValue;

    // get the symbolic names of step and datum from code
    QStringList parts;
    parts = code.split(":");

    if (parts.length() < 2) {
        qCritical() << "Invalid datum code " + code;
        return;
    }

    if (m_globalDatums->contains(code)) {

        DatumType dt = m_globalDatums->value(code);

        switch (dt.type) {
        case doubleType:
            doubleValue = value * 1.0 / dt.scale;
            targetValue = QVariant::fromValue(doubleValue);
            break;
        case integerType:
            intValue = value / dt.scale;
            targetValue = QVariant::fromValue(intValue);
        default:
            break;
        }
    }

    qDebug() << "> Setting value to " + code + " " + targetValue.toString();

    m_job->setAvpNumericParameter(parts[0], parts[1], targetValue);
}

/*!
 * \brief JobManager::setDatumValue
 * \param stepPath
 * \param stepDatum
 */
void JobManager::setDatumValue(QString stepPath, StepDatum stepDatum) {
    m_job->setDatumValue(stepPath, stepDatum);

    // TODO: update m_stepProperties
}

/*!
 * \brief JobManager::regenerate
 */
void JobManager::regenerate() {
    m_setupManager->regenerate();
    // m_job->regenerate();
}

void JobManager::refresh() {
    m_setupManager->refresh();
}

/*!
 * \brief JobManager::getAvpNumericParam
 * \param code
 * \return
 */
QVariant JobManager::getAvpNumericParam(QString code) {

    QVariant sourceValue, targetValue;
    double doubleValue;
    int intValue;

    targetValue = QVariant::fromValue(0);

    // get the symbolic names of step and datum from code
    QStringList parts;
    parts = code.split(":");

    if (parts.length() < 2) {
        qCritical() << "Invalid datum code " + code;
        return QVariant::fromValue(NULL);
    }

    sourceValue = m_job->getAvpNumericParameter(parts[0], parts[1]);

    if (m_globalDatums->contains(code)) {

        DatumType dt = m_globalDatums->value(code);

        switch (dt.type) {
        case doubleType:
            doubleValue = sourceValue.toDouble() * dt.scale;
            targetValue = QVariant::fromValue(doubleValue);
            break;
        case integerType:
            intValue = sourceValue.toInt() * dt.scale;
            targetValue = QVariant::fromValue(intValue);
        default:
            break;
        }
    }
    return targetValue;
}

/*!
 * \brief JobManager::setBatchConfiguration
 *        Use this function to set the effectin Batch Configuration. The supplied
 *        configuration will be used for all subsequent inspection process executions.

 * \param conf the current batch configuration data.
 */
void JobManager::setBatchConfiguration(BatchConfiguration *conf) {

    qDebug() << "Change batch configuration";

    m_batchConfiguration = new BatchConfiguration(conf, this);
}
