#include "server.h"

#include <QStringList>
#include <QTcpSocket>

Server::Server() : QTcpServer() {}

Server::~Server() {}

bool Server::initialize(networkParameters networkParams) {
    bool ok = false;

    qDebug() << "Making connections.";
    ok = connect(this, SIGNAL(newConnection()), SLOT(initializeConnection()));

    if (ok) {
        qDebug() << "Opening server on" << networkParams.hostAddress << networkParams.port;
        ok = listen(networkParams.hostAddress, networkParams.port);
    }

    return ok;
}

bool Server::isConnected() {
    if (m_activeSocket != NULL)
        return m_activeSocket->isOpen();

    return false;
}

void Server::readFromSocket() {
    emit readFromSocket(m_activeSocket->readAll());
}

void Server::writeToSocket(const QByteArray &bytes) {
    qint64 bytesWritten = m_activeSocket->write(bytes);

    qDebug() << "Wrote" << bytesWritten << "to socket.";
}

void Server::initializeConnection() {
    m_activeSocket = nextPendingConnection();

    // make connections
    bool ok = connect(m_activeSocket, SIGNAL(readyRead()), this, SLOT(readFromSocket()));
    Q_ASSERT(ok);

    ok = connect(m_activeSocket, SIGNAL(disconnected()), this, SIGNAL(closedConnection()));
    Q_ASSERT(ok);

    if (ok)
        emit openedConnection();

    // We are only setting up for a single connection, stop listening
    // after we establish the first connection.
    close();
}
