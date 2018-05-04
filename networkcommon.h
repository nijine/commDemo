#ifndef NETWORKCOMMON_H
#define NETWORKCOMMON_H

#include <QObject>
#include <QHostAddress>

enum nodeType {
    Null = -1,
    ClientNode,
    ServerNode
};

struct networkParameters {
    nodeType nType = Null;
    QHostAddress hostAddress;
    qint16 port = 0;
};

#endif // NETWORKCOMMON_H
