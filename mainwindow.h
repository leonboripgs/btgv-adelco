#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include <uibackend.h>
#include <uicontroller.h>
// #include <plcconnection.h>  // !!!tmp
// #include <configurationmanager.h>  // !!!tmp
// #include <vscape/jobmanager.h>  // !!!tmp

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QSettings*, QWidget *parent = 0);
    ~MainWindow();

private:
    UIController            *m_controller;
    UIBackend               *m_uiBackend;
    // !!!tmp PLCConnection           *m_plcConnection;
    // !!!tmp ConfigurationManager    *m_configurationManager;
    // !!!tmp JobManager              *m_jobManager;
};

#endif // MAINWINDOW_H
