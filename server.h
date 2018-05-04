#ifndef SSLSERVER_H
#define SSLSERVER_H

#include "networkcommon.h"

#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT
    QTcpSocket *m_activeSocket;

public:
    Server();
    ~Server();

    bool initialize(networkParameters networkParams);
    bool isConnected();

signals:
    void readFromSocket(QByteArray);
    void openedConnection();
    void closedConnection();

public slots:
    void readFromSocket();
    void writeToSocket(const QByteArray &bytes);

private slots:
    void initializeConnection();
};

#endif // SSLSERVER_H


