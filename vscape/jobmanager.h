#ifndef JOBMANAGER_H
#define JOBMANAGER_H

#include <QObject>
#include <QSettings>
#include <vscape/job.h>
#include <vscape/camera.h>
#include <vscape/reportconnection.h>
#include <vscape/setupmanager.h>
#include <vscape/datumproperties.h>
#include <vscape/stepproperties.h>
#include <batchconfiguration.h>

class JobManager : public QObject
{
    Q_OBJECT

public:
    explicit JobManager(QSettings *settings, QObject *parent = 0);
    ~JobManager();

    void start();
    void loadAVP(QString fileName);
    void saveAVP(QString fileName);
    void cleanUp();

    void editMode();
    void runMode();
    void acquireImage();
    void acquireStepImage(QString symName, bool useOutput, bool showGraphics);
    void acquireRootImage(bool showGraphics);
    int* getImageHSI(int x, int y);
    void cancelAcquireImage();
    bool acquireStatus();
    void getImage();

    void walkSteps();  // for testing
    void trainAll();
    void train(QString stepPath);
    void trainParent(QString stepPath);
    void tryout(QString stepPath);
    void tryOutSteps(QString stepPath);
    void deleteStep(QString stepPath);

    void setPath(QString path);
    QString getPath();
    void setAvp(QString avp);
    void setBatchConfiguration(BatchConfiguration *conf);

    void setAvpNumericParam(QString code, int value);
    QVariant getAvpNumericParam(QString code);
    QList<DatumType> getGlobalDatums();
    QList<StepProps> *getConfigurableSteps(bool withUpdate = false);
    StepProps getStepProps(QString stepPath);
    void updateROI(StepProps step);
    void updateROIs(QList<StepProps> steps);
    void setDatumValue(QString stepPath, StepDatum stepDatum);
    void regenerate();
    void refresh();

signals:
    void ready();
    void trainReady();
    void stepImageReady();
    void trainImageAcquired(QPixmap*);
    void newImage(QPixmap*, bool);
    void viewMessage(QString msg);
    void imageReady(bool);
    void newResult(bool, unsigned char);
    void newStatistics(QString statsJson);

public slots:
    void onTrainImageAcquired(QPixmap*);
    void onNewImage(QPixmap*,bool);
    void onNewResult(bool, unsigned char);

private slots:
    void onCameraReady();
    void onAvpReady(bool);

private:
    QSettings *m_settings;
    QString m_avpPath;
    QString m_avp;
    QString m_avpInProgress;
    QString m_cameraName;

    bool m_avpLoaded;
    bool m_loadInProgress;
    bool m_editMode;
    bool m_runMode;

    QMap<QString, DatumType> *m_globalDatums;
    void prepareDatums();
    void updateGlobalDatumValues();

    QList<StepProps> *m_stepProperties;

    Job*                m_job;
    Camera*             m_camera;
    SetupManager*       m_setupManager;
    ReportConnection*   m_reportConnection;
    BatchConfiguration* m_batchConfiguration;
};

#endif // JOBMANAGER_H
