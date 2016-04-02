#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QObject>
// #include <plcconnection.h>  // !!!tmp
// #include <configurationmanager.h>  // !!!tmp
// #include <trainimageprovider.h> // !!!tmp
// #include <vscape/jobmanager.h> // !!!tmp

class UIController : public QObject
{
    Q_OBJECT

public:
    // !!!tmp Q_PROPERTY(SystemConfiguration *systemConfiguration READ getSystemConfiguration)
    Q_INVOKABLE void requestPLCStatus();
    Q_INVOKABLE void requestPLCStart();
    Q_INVOKABLE void requestPLCStop();
    Q_INVOKABLE void resetCounters(int);

    Q_INVOKABLE void startTest(int);
    Q_INVOKABLE void stopTest(int);
    Q_INVOKABLE void setManualMode(bool);
    Q_INVOKABLE void resetErrors();

    // !!!tmp Q_INVOKABLE void saveSystemConfiguration(SystemConfiguration *);

    Q_INVOKABLE QStringList getConfigurationNames();
    Q_INVOKABLE QStringList getReadOnlyConfigurations();
    Q_INVOKABLE QString getActiveConfigurationId();
    // !!!tmp Q_INVOKABLE BatchConfiguration *getConfigurationById(QString);
    // !!!tmp Q_INVOKABLE void useBatchConfiguration(BatchConfiguration*, bool loadAvp = false);
    // !!!tmp Q_INVOKABLE void updateConfiguration(BatchConfiguration*);
    // !!!tmp Q_INVOKABLE void saveConfiguration(BatchConfiguration*);
    Q_INVOKABLE bool configurationNameExists(QString);
    Q_INVOKABLE void copyAvp(QString, QString);
    Q_INVOKABLE void deleteConfiguration(QString);

    Q_INVOKABLE bool isJobReady();
    Q_INVOKABLE void loadAvp(QString code, bool editMode = true);
    Q_INVOKABLE void saveAvp(QString code);
    Q_INVOKABLE void setAvpNumericParam(QString code, int value);
    Q_INVOKABLE QString getGlobalDatums();
    Q_INVOKABLE QString getConfigurableSteps(bool withUpdate = false);
    Q_INVOKABLE QString getStep(QString path);
    Q_INVOKABLE void setDatumValue(QString stepPath, QString datumJson);
    Q_INVOKABLE void setEditMode();
    Q_INVOKABLE void setRunMode();

    Q_INVOKABLE void acquireImage();
    Q_INVOKABLE void acquireInputImage(QString symName);    // TODO: check compliance with step path - change uses
    Q_INVOKABLE void acquireRootImage();
    Q_INVOKABLE void cancelAcquireImage();
    Q_INVOKABLE bool acquireStatus();
    Q_INVOKABLE void updateROI(QString stepModel);
    Q_INVOKABLE void updateROIs(QString stepsModel);
    Q_INVOKABLE void trainAll();
    Q_INVOKABLE void train(QString stepPath);
    Q_INVOKABLE void trainParent(QString stepPath);
    Q_INVOKABLE void tryout(QString stepPath);
    Q_INVOKABLE void deleteStep(QString stepPath);

    explicit UIController(
            QString configId,
// !!!tmp             PLCConnection*,
// !!!tmp             ConfigurationManager*,
// !!!tmp             JobManager*,
            QObject *parent = 0
            );

    void initialize();

// !!!tmp     SystemConfiguration *getSystemConfiguration();
    void setActiveConfigurationId(QString);

signals:
    void plcConnect();
    void plcDisconnect();
// !!!tmp     void plcStatus(SystemStatus*);
    void plcStarted();
    void plcStopped();
    void plcErrorStatus(bool);
    void plcWarningStatus(bool);

    void jobReady(bool);
    void trainImageReady();
    void stepImageReady();
    void imageReady(bool);
    void newStatistics(QString statsJson);

    void newMessage(QString);
    void clearFailedImages();

public slots:

private slots:
    void onJobReady();
    void onTrainReady();
// !!!tmp     void onPlcStatus(SystemStatus*);

private:
    bool m_ready;
    bool m_plcRunning;
    bool m_plcError;
    bool m_plcWarning;
    bool m_jobReady;

    int m_cyclePulses;
    int m_triggerOffset;

    QString m_activeConfigurationId;
// !!!tmp     BatchConfiguration m_config;

// !!!tmp     PLCConnection           *m_plcConnection;
// !!!tmp     ConfigurationManager    *m_configurationManager;
// !!!tmp     JobManager              *m_jobManager;

// !!!tmp     void convertDegressToPulses(BatchConfiguration *);
};

#endif // UICONTROLLER_H
