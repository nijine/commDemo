#ifndef CLIENT_H
#define CLIENT_H

#include "networkcommon.h"

#include <QHostAddress>

class QTcpSocket;
class Client : public QObject
{
    Q_OBJECT
    QTcpSocket *m_tcpSocket;

public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    bool initialize(networkParameters networkParams);
    bool isConnected();

signals:
    void readFromSocket(QByteArray);
    void openedConnection();
    void closedConnection();

public slots:
    void readFromSocket();
    void writeToSocket(const QByteArray &bytes);
};

#endif // CLIENT_H
