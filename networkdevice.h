#ifndef NETWORKDEVICE_H
#define NETWORKDEVICE_H

#include <QObject>
#include <QPointer>
#include <QFile>

#include "networkcommon.h"

class networkDevice : public QObject
{
    Q_OBJECT
    QPointer<QObject> m_devicePtr;
    networkParameters m_networkParams;

public:
    explicit networkDevice(QObject *parent = nullptr);
    ~networkDevice();
    bool initialize(networkParameters networkParams);

signals:
    void readData(QByteArray b);
    void writeData(QByteArray b);
    void openedConnection();
    void closedConnection();
};

#endif // NETWORKDEVICE_H
