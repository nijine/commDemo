#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "networkcommon.h"

#include <QMainWindow>
#include <QFile>

namespace Ui {
class MainWindow;
}

class networkDevice;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    QFile logFile;
    networkDevice *m_networkDevice;
    networkParameters m_networkParameters;

    void initializeCore(bool enableTee);
    void initializeNetwork();
    void moveSignals();
    void enterPressed(QString heading = QString());

public:
    explicit MainWindow(bool enableTee = true, QWidget *parent = 0);
    ~MainWindow();

signals:
    void sendNetworkData(QByteArray);

public slots:
    void clearLog();
    void postText(QString s);
    void receiveResponse();
    void receiveNetworkData(QByteArray b);
    void sendNetworkData();
    void openedConnection();
    void closedConnection();
};

#endif // MAINWINDOW_H
