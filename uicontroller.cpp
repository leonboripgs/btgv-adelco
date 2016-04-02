#include "uicontroller.h"

UIController::UIController(
        QString configId,
// !!!tmp         PLCConnection *plcConnection,
// !!!tmp         ConfigurationManager *manager,
// !!!tmp         JobManager *jobmanager,
        QObject *parent) : QObject(parent)
{
    m_activeConfigurationId = configId;
    m_ready = false;
    m_plcRunning = false;
    m_plcError = false;
    m_plcWarning = false;
    // !!!tmp m_plcConnection = plcConnection;
    // !!!tmp m_configurationManager = manager;
    // !!!tmp m_jobManager = jobmanager;
    m_jobReady = false;

    // !!!tmp m_cyclePulses = m_plcConnection->getCyclePulses();
    // !!!tmp m_triggerOffset = m_plcConnection->getTriggerOffset();

    newMessage("Application started");

    // !!!tmp connect(m_plcConnection, SIGNAL(plcConnected()), this, SIGNAL(plcConnect()));
    // !!!tmp connect(m_plcConnection, SIGNAL(plcDisconnected()), this, SIGNAL(plcDisconnect()));
    // !!!tmp connect(m_plcConnection, SIGNAL(plcStatus(SystemStatus*)), this, SLOT(onPlcStatus(SystemStatus*)));

    // connect message signals from all sources
    // !!!tmp connect(m_plcConnection, SIGNAL(viewMessage(QString)), this, SIGNAL(newMessage(QString)));
    // !!!tmp connect(m_jobManager, SIGNAL(viewMessage(QString)), this, SIGNAL(newMessage(QString)));

    // connect job manager signals
    // !!!tmp connect(m_jobManager, SIGNAL(ready()), this, SLOT(onJobReady()));
    // !!!tmp connect(m_jobManager, SIGNAL(trainReady()), this, SLOT(onTrainReady()));
    // !!!tmp connect(m_jobManager, SIGNAL(stepImageReady()), this, SIGNAL(stepImageReady()));
    // !!!tmp connect(m_jobManager, SIGNAL(imageReady(bool)), this, SIGNAL(imageReady(bool)));
    // !!!tmp connect(m_jobManager, SIGNAL(newResult(bool)), m_plcConnection, SLOT(onNewResult(bool)));
    // !!!tmp connect(m_jobManager, SIGNAL(newStatistics(QString)), this, SIGNAL(newStatistics(QString)));
}

/*!
 * \brief UIController::setEditMode
 */
void UIController::setEditMode() {
// !!!tmp     m_jobManager->editMode();

    emit jobReady(true);
}

/*!
 * \brief UIController::setRunMode
 */
void UIController::setRunMode() {
// !!!tmp     m_jobManager->runMode();

    emit jobReady(true);
}

/*!
 * \brief UIController::acquireImage
 */
void UIController::acquireImage() {
// !!!tmp     m_jobManager->acquireImage();
}

/*!
 * \brief UIController::acquireInputImage
 * \param symName
 */
void UIController::acquireInputImage(QString symName) {
// !!!tmp     m_jobManager->acquireInputImage(symName);
}

/*!
 * \brief UIController::acquireRootImage
 */
void UIController::acquireRootImage() {
// !!!tmp    m_jobManager->acquireRootImage();
}

/*!
 * \brief UIController::cancelAcquireImage
 */
void UIController::cancelAcquireImage() {
// !!!tmp     m_jobManager->cancelAcquireImage();
}

/*!
 * \brief UIController::acquireStatus
 * \return
 */
bool UIController::acquireStatus() {
// !!!tmp     return m_jobManager->acquireStatus();
}

/*!
 * \brief UIController::onJobReady
 */
void UIController::onJobReady() {
    emit jobReady(true);
}

/*!
 * \brief UIController::isJobReady
 * \return
 */
bool UIController::isJobReady() {
    return m_jobReady;
}

/*!
 * \brief UIController::onTrainReady
 */
void UIController::onTrainReady() {

    emit trainImageReady();
    return;
}

/*!
 * \brief UIController::setAvpNumericParam
 * \param code
 * \param value
 */
void UIController::setAvpNumericParam(QString code, int value) {
// !!!tmp     m_jobManager->setAvpNumericParam(code, value);
}

/*!
 * \brief UIController::setDatumValue
 * \param stepPath
 * \param datumJson
 */
void UIController::setDatumValue(QString stepPath, QString datumJson) {

// !!!tmp     StepDatum stepDatum = DatumProperties::datumFromJson(datumJson);
// !!!tmp     m_jobManager->setDatumValue(stepPath, stepDatum);
}

/*!
 * \brief UIController::updateROI
 * \param stepModel
 */
void UIController::updateROI(QString stepModel) {
// !!!tmp     StepProps stepProps = StepProperties::fromJson(stepModel);

// !!!tmp     m_jobManager->updateROI(stepProps);
}

/*!
 * \brief UIController::updateROIs
 * \param stepsModel
 */
void UIController::updateROIs(QString stepsModel) {
// !!!tmp     QList<StepProps> stepsProps = StepProperties::listFromJson(stepsModel);

// !!!tmp     m_jobManager->updateROIs(stepsProps);
}

/*!
 * \brief UIController::getGlobalDatums
 * \return
 */
QString UIController::getGlobalDatums() {
// !!!tmp     QList<DatumType> globalDatums = m_jobManager->getGlobalDatums();

// !!!tmp     QJsonArray datums = {};
// !!!tmp     for (int i = 0; i < globalDatums.length(); i++) {
// !!!tmp         datums.append( DatumProperties::asJson( globalDatums[i]) );
// !!!tmp     }

// !!!tmp     QJsonDocument doc(datums);
    return NULL; // !!!tmp doc.toJson();
}

/*!
 * \brief UIController::getConfigurableSteps
 * \return
 */
QString UIController::getConfigurableSteps(bool withUpdate) {
// !!!tmp     QList<StepProps> *stepProperties = m_jobManager->getConfigurableSteps(withUpdate);

// !!!tmp     QJsonArray steps = {};
// !!!tmp     for (int i = 0; i < stepProperties->length(); i++) {
// !!!tmp         steps.append( StepProperties::asJson( stepProperties->at(i) ));
    // !!!tmp }

    // !!!tmp QJsonDocument doc(steps);
    return ""; // !!!tmp doc.toJson();
}

/*!
 * \brief UIController::getStep
 * \param path
 * \return
 */
QString UIController::getStep(QString stepPath) {
   // !!!tmp StepProps stepProperties = m_jobManager->getStepProps(stepPath);

   // !!!tmp QJsonObject stepJsonObject = StepProperties::asJson(stepProperties);

   // !!!tmp QJsonDocument doc(stepJsonObject);
   return NULL; // !!!tmp doc.toJson();
}

/*!
 * \brief UIController::trainAll
 */
void UIController::trainAll() {
    // !!!tmp m_jobManager->trainAll();
}

/*!
 * \brief UIController::train
 * \param stepPath
 */
void UIController::train(QString stepPath) {
    // !!!tmp m_jobManager->train(stepPath);
}

/*!
 * \brief UIController::trainParent
 * \param stepPath
 */
void UIController::trainParent(QString stepPath) {
    // !!!tmp m_jobManager->trainParent(stepPath);
}

/*!
 * \brief UIController::tryout
 * \param stepPath
 */
void UIController::tryout(QString stepPath) {
    // !!!tmp m_jobManager->tryout(stepPath);
}

/*!
 * \brief UIController::deleteStep
 * \param stepPath
 */
void UIController::deleteStep(QString stepPath) {
    // !!!tmp m_jobManager->deleteStep(stepPath);
}

/*!
 * \brief UIController::setActiveConfigurationId
 * \param id
 */
void UIController::setActiveConfigurationId(QString id) {
    m_activeConfigurationId = id;
}

/*!
 * \brief UIController::getActiveConfigurationId
 * \return
 */
QString UIController::getActiveConfigurationId() {
   return m_activeConfigurationId;
}

/*!
 * \brief UIController::initialize
 */
void UIController::initialize() {
    // !!!tmp m_plcConnection->plcConnect();
    m_ready = true;

    // !!!tmp QString avp = m_configurationManager->getAvpById(m_activeConfigurationId);
    // !!!tmp m_jobManager->setAvp(avp);

    // !!!tmp m_jobManager->start();
}

/*!
 * \brief UIController::requestPLCStatus
 */
void UIController::requestPLCStatus() {
    // !!!tmp m_plcConnection->getPLCStatus();
}

/*!
 * \brief UIController::requestPLCStart
 */
void UIController::requestPLCStart() {
    // !!!tmp m_plcConnection->start();
}

/*!
 * \brief UIController::requestPLCStop
 */
void UIController::requestPLCStop() {
    // !!!tmp m_plcConnection->stop();
}

/*!
 * \brief UIController::resetCounters
 * \param group
 */
void UIController::resetCounters(int group) {
   // !!!tmp m_plcConnection->resetCounters(group);
}

/*!
 * \brief UIController::resetErrors
 */
void UIController::resetErrors() {
   // !!!tmp m_plcConnection->resetErrors();
}

/*!
 * \brief UIController::startTest
 * \param code
 */
void UIController::startTest(int code) {
    // !!!tmp m_plcConnection->startTest(code);
}

/*!
 * \brief UIController::stopTest
 * \param code
 */
void UIController::stopTest(int code) {
    // !!!tmp m_plcConnection->stopTest(code);
}

/*!
 * \brief UIController::setManualMode
 * \param manualMode
 */
void UIController::setManualMode(bool manualMode) {
    // !!!tmp m_plcConnection->setManualMode(manualMode);
}

/*!
 * \brief UIController::saveSystemConfiguration
 * \param config
 */
// !!!tmp void UIController::saveSystemConfiguration(SystemConfiguration *config) {
// !!!tmp     m_plcConnection->sendSystemConfiguration(config);
// !!!tmp }

// !!!tmp SystemConfiguration *UIController::getSystemConfiguration() {
// !!!tmp     return m_plcConnection->getSystemConfiguration();
// !!!tmp }

/*!
 * \brief UIController::onPlcStatus
 * \param systemStatus
 */
// !!!tmp void UIController::onPlcStatus(SystemStatus *systemStatus) {
// !!!tmp     emit plcStatus(systemStatus);

// !!!tmp     quint8 flags = systemStatus->getStatusFlags();

    // handle Running status changes
    // -------------------------------------------------------------
// !!!tmp     bool newRunningStatus = flags & SYSTEM_STARTED_FLAG;
// !!!tmp
/*
    if (newRunningStatus != m_plcRunning) {
        m_plcRunning = newRunningStatus;

        if (m_plcRunning) {
            emit plcStarted();
            emit newMessage("PLC started");
        }
        else {
            emit plcStopped();
            emit newMessage("PLC stopped");
        }
    }

    // handle Error status changes
    // -------------------------------------------------------------
    bool newErrorStatus = flags & SYSTEM_ERROR_FLAG;

    if (newErrorStatus != m_plcError) {
        m_plcError = newErrorStatus;
        emit plcErrorStatus(m_plcError);
    }

    // handle Warning status changes
    // -------------------------------------------------------------
    bool newWarningStatus = flags & SYSTEM_WARNING_FLAG;

    if (newWarningStatus != m_plcWarning) {
        m_plcWarning = newWarningStatus;
        emit plcWarningStatus(m_plcWarning);
    }
}
   */

// ------------------------------------------------------------------
// Configuration Management functions
// ------------------------------------------------------------------
/*!
 * \brief UIController::getConfigurationNames
 * \return
 */
QStringList UIController::getConfigurationNames() {
    return {}; // !!!tmp m_configurationManager->getConfigurationNames();
}

/*!
 * \brief UIController::getReadOnlyConfigurations
 * \return
 */
QStringList UIController::getReadOnlyConfigurations() {
    return {}; // !!!tmp m_configurationManager->getReadOnlyConfigurations();
}

/*!
 * \brief UIController::getConfigurationById
 * \param id
 * \param config
 */
// !!!tmp
/*
BatchConfiguration *UIController::getConfigurationById(QString id) {

    BatchConfiguration *config = new BatchConfiguration();

    // convert pulses to degress (x100)
    // -------------------------------------------------------------
    m_configurationManager->getConfigurationById(id, config);
    config->setCameraTrigger ( (quint32)( ((m_cyclePulses + config->getCameraTrigger()  - m_triggerOffset) % m_cyclePulses) * 36000.0 / m_cyclePulses + 0.5) );
    config->setLightStrobeOff( (quint32)( ((m_cyclePulses + config->getLightStrobeOff() - m_triggerOffset) % m_cyclePulses) * 36000.0 / m_cyclePulses + 0.5) );
    config->setRejectValveOn ( (quint32)( ((m_cyclePulses + config->getRejectValveOn()  - m_triggerOffset) % m_cyclePulses) * 36000.0 / m_cyclePulses + 0.5) );
    config->setRejectValveOff( (quint32)( ((m_cyclePulses + config->getRejectValveOff() - m_triggerOffset) % m_cyclePulses) * 36000.0 / m_cyclePulses + 0.5) );

    return config;
}
*/

/*!
 * \brief UIController::useConfiguration
 * \param config
 * \param loadAvp
 */
// !!!tmp
/*void UIController::useBatchConfiguration(BatchConfiguration *config, bool loadAvp) {

    // convert degress (x100) to pulses
    // -------------------------------------------------------------
    BatchConfiguration *newConfig = new BatchConfiguration(config, config);
    newConfig->setCameraTrigger ( (quint32)(config->getCameraTrigger()  * m_cyclePulses / 36000.0 + m_triggerOffset + 0.5) % m_cyclePulses  );
    newConfig->setLightStrobeOff( (quint32)(config->getLightStrobeOff() * m_cyclePulses / 36000.0 + m_triggerOffset + 0.5) % m_cyclePulses  );
    newConfig->setRejectValveOn ( (quint32)(config->getRejectValveOn()  * m_cyclePulses / 36000.0 + m_triggerOffset + 0.5) % m_cyclePulses  );
    newConfig->setRejectValveOff( (quint32)(config->getRejectValveOff() * m_cyclePulses / 36000.0 + m_triggerOffset + 0.5) % m_cyclePulses  );
    m_plcConnection->sendBatchConfiguration(newConfig);
    delete newConfig;

    if (! loadAvp) {
        return;
    }

    emit jobReady(false);

    m_activeConfigurationId = config->getName();
    m_configurationManager->storeConfigurationId(m_activeConfigurationId);

    QString fileName = m_activeConfigurationId + ".avp";
    m_jobManager->loadAVP(fileName);
}
*/

/*!
 * \brief UIController::loadAvp
 * \param code
 */
void UIController::loadAvp(QString code, bool editMode) {
    // TODO: check the necessity of the editMode parameter
    Q_UNUSED(editMode);

    emit jobReady(false);

    QString fileName = code + ".avp";
    // !!!tmp m_jobManager->loadAVP(fileName);
}

/*!
 * \brief UIController::saveAvp
 * \param code
 */
void UIController::saveAvp(QString code) {

    QString fileName = code + ".avp";
    // !!!tmp m_jobManager->saveAVP(fileName);
}

/*!
 * \brief UIController::updateConfiguration
 * \param config
 */
// !!!tmp
/*
void UIController::updateConfiguration(BatchConfiguration *config) {
    qDebug() << "Updating the Batch-Configuration database entry for " + config->getName();

    BatchConfiguration *newConfig = new BatchConfiguration(config, config);
    convertDegressToPulses(newConfig);
    m_configurationManager->updateConfiguration(newConfig);
    delete newConfig;
}
*/

/*!
 * \brief UIController::saveConfiguration
 * \param config
 */
// !!!tmp
/*
void UIController::saveConfiguration(BatchConfiguration *config) {
    qDebug() << "Saving the Batch-Configuration database entry for " + config->getName();

    BatchConfiguration *newConfig = new BatchConfiguration(config, config);
    convertDegressToPulses(newConfig);
    m_configurationManager->saveConfiguration(newConfig);
    delete newConfig;
}
*/

/*!
 * \brief UIController::convertDegressToPulses
 * \param config
 */
// !!!tmp
/*
void UIController::convertDegressToPulses(BatchConfiguration *config) {
    // convert degress (x100) to pulses
    // -------------------------------------------------------------
    config->setCameraTrigger ( (quint32)(config->getCameraTrigger()  * m_cyclePulses / 36000.0 + m_triggerOffset + 0.5) % m_cyclePulses  );
    config->setLightStrobeOff( (quint32)(config->getLightStrobeOff() * m_cyclePulses / 36000.0 + m_triggerOffset + 0.5) % m_cyclePulses  );
    config->setRejectValveOn ( (quint32)(config->getRejectValveOn()  * m_cyclePulses / 36000.0 + m_triggerOffset + 0.5) % m_cyclePulses  );
    config->setRejectValveOff( (quint32)(config->getRejectValveOff() * m_cyclePulses / 36000.0 + m_triggerOffset + 0.5) % m_cyclePulses  );
}
*/

/*!
 * \brief UIController::configurationNameExists
 * \param name
 */
bool UIController::configurationNameExists(QString name) {
    return NULL; // !!!tmp m_configurationManager->nameExists(name);
}

/*!
 * \brief UIController::copyAvp
 * \param name1
 * \param name2
 */
void UIController::copyAvp(QString name1, QString name2) {
    // !!!tmp m_configurationManager->copyAvp(name1, name2);
}

/*!
 * \brief UIController::deleteConfiguration
 * \param name
 */
void UIController::deleteConfiguration(QString name) {
    // !!!tmp m_configurationManager->deleteConfiguration(name);
}
