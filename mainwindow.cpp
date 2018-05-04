#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "networkdevice.h"

#include <QDebug>
#include <QScrollBar>

/// TOOD: make enableTee a user setting
MainWindow::MainWindow(bool enableTee, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeCore(enableTee);
}

MainWindow::~MainWindow() {
    // close text file if opened
    if (logFile.isOpen()) {
        qDebug() << "Closing log file.";
        logFile.close();
    }

    delete ui;
}

void MainWindow::initializeCore(bool enableTee) {
    // initialize log file if enabled
    if (enableTee) {
        // open text file
        qDebug() << "Logging to file enabled, attempting to open log.";

        /// TODO: make this an option
        logFile.setFileName("log.txt");

        if (logFile.open(QIODevice::WriteOnly))
            qDebug() << "Opened log file successfully.";
        else
            qDebug() << "Couldn't open log file.";
    }

    bool ok = false;

    // make core connections
    ok = connect(ui->lineEditInput, SIGNAL(returnPressed()), this, SLOT(receiveResponse()));
    Q_ASSERT(ok);

    ok = connect(ui->pushButtonEnter, SIGNAL(clicked()), ui->lineEditInput, SIGNAL(returnPressed()));
    Q_ASSERT(ok);

    ok = connect(ui->actionClear, SIGNAL(triggered()), this, SLOT(clearLog()));
    Q_ASSERT(ok);

    ok = connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    Q_ASSERT(ok);

    postText("Initialized connections.");

    // start network configuration
    postText("\nClient or Server? [C/S] ");
}

void MainWindow::initializeNetwork() {
    m_networkDevice = new networkDevice();

    // initialize
    if (!m_networkDevice->initialize(m_networkParameters)) {
        postText("\nCouldn't initialize network.");
        return;
    }

    // setup read/write channels
    bool ok = connect(this, SIGNAL(sendNetworkData(QByteArray)), m_networkDevice, SIGNAL(writeData(QByteArray)));
    Q_ASSERT(ok);

    ok = connect(m_networkDevice, SIGNAL(readData(QByteArray)), this, SLOT(receiveNetworkData(QByteArray)));
    Q_ASSERT(ok);

    ok = connect(m_networkDevice, SIGNAL(openedConnection()), this, SLOT(openedConnection()));
    Q_ASSERT(ok);

    ok = connect(m_networkDevice, SIGNAL(closedConnection()), this, SLOT(closedConnection()));
    Q_ASSERT(ok);

    postText("\nInitialized network.");
}

void MainWindow::moveSignals() {
    bool ok = false;

    ok = disconnect(ui->lineEditInput, SIGNAL(returnPressed()), this, SLOT(receiveResponse()));
    Q_ASSERT(ok);

    ok = connect(ui->lineEditInput, SIGNAL(returnPressed()), this, SLOT(sendNetworkData()));
    Q_ASSERT(ok);
}

void MainWindow::sendNetworkData() {
    emit sendNetworkData(ui->lineEditInput->text().toLatin1());

    // not really necessary for this implementation
//    emit sendNetworkData("\r\n");

    enterPressed("\nLocal: ");
}

void MainWindow::openedConnection() {
    postText("\nConnection established.");
}

void MainWindow::closedConnection() {
    postText("\nConnection closed.");
    ui->lineEditInput->setEnabled(false);
}

void MainWindow::enterPressed(QString heading) {
    postText(QString("%1%2").arg(heading, ui->lineEditInput->text()));
    ui->lineEditInput->clear();
}

void MainWindow::clearLog() {
    ui->textBrowser->clear();
}

void MainWindow::postText(QString s) {
    if (logFile.isOpen()) {
        // write s to text file
        // flush
        logFile.write(s.toLatin1());
        logFile.flush();
    }

    // viewability hack
    QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    bool scrollBarAtBottom = sb->value() == sb->maximum();

    ui->textBrowser->insertPlainText(s);

    if (scrollBarAtBottom)
        sb->setValue(sb->maximum());
}

void MainWindow::receiveResponse() {
    if (m_networkParameters.nType == Null) {
        if (ui->lineEditInput->text().startsWith('C', Qt::CaseInsensitive)) {
            m_networkParameters.nType = ClientNode;
            enterPressed();
            postText("\nEnter host address: ");

        } else if (ui->lineEditInput->text().startsWith('S', Qt::CaseInsensitive)) {
            m_networkParameters.nType = ServerNode;
            enterPressed();
            postText("\nEnter host address: ");

        } else {
            postText("Invalid mode.\n");
            return;
        }

    } else if (m_networkParameters.hostAddress.isNull()) {
        m_networkParameters.hostAddress = QHostAddress(ui->lineEditInput->text());
        enterPressed();
        postText("\nEnter network port: ");

    } else if (m_networkParameters.port == 0) {
        qint16 networkPort = ui->lineEditInput->text().toInt();

        if (networkPort <= 0) {
            postText("Invalid port.\n");

        } else {
            m_networkParameters.port = networkPort;
            enterPressed();
            moveSignals();
            initializeNetwork();
        }

    } else {
        // this shouldn't happen
        postText("Issue processing response.\n");
    }
}

void MainWindow::receiveNetworkData(QByteArray b) {
    postText(QString("%1: %2").arg("\nRemote", QString(b)));
}
