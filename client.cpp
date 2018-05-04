#include "client.h"

#include <QTcpSocket>
#include <QDebug>

Client::Client(QObject *parent) : QObject(parent) {}

Client::~Client() {
    if (m_tcpSocket != NULL) {
        m_tcpSocket->close();
        delete m_tcpSocket;
    }
}

// initialize with host and port
bool Client::initialize(networkParameters networkParams) {
    m_tcpSocket = new QTcpSocket();
    m_tcpSocket->connectToHost(networkParams.hostAddress, networkParams.port);

    if (!m_tcpSocket->waitForConnected()) {
        qDebug() << "Couldn't connect to host.";
        return false;
    }

    bool ok = connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readFromSocket()));
    Q_ASSERT(ok);

    ok = connect(m_tcpSocket, SIGNAL(disconnected()), this, SIGNAL(closedConnection()));
    Q_ASSERT(ok);

    emit openedConnection();
    qDebug() << "Connected.";

    return true;
}

bool Client::isConnected() {
    if (m_tcpSocket == NULL)
        return false;

    return m_tcpSocket->isOpen();
}

void Client::readFromSocket() {
    emit readFromSocket(m_tcpSocket->readAll());
}

void Client::writeToSocket(const QByteArray &bytes) {
    qint64 bytesWritten = m_tcpSocket->write(bytes);

    qDebug() << "Wrote" << bytesWritten << "to socket.";
}
