#include "networkdevice.h"
#include "client.h"
#include "server.h"

#include <QPointer>
#include <QSslCertificate>
#include <QSslKey>

networkDevice::networkDevice(QObject *parent) : QObject(parent) {}

networkDevice::~networkDevice() {
    if (!m_devicePtr.isNull())
        delete m_devicePtr;
}

bool networkDevice::initialize(networkParameters networkParams) {
    if (!m_devicePtr.isNull()) {
        qDebug() << "Cannot initialize network device more than once.";
        return false;
    }

    bool ok = false;
    m_networkParams = networkParams;

    switch(networkParams.nType) {
    case ClientNode:
        qDebug() << "Initializing client.";

        m_devicePtr = new Client();
        ok = static_cast<Client*>(m_devicePtr.data())->initialize(networkParams);

        if (ok) {
            qDebug() << "Client object initialized.";
        } else {
            qDebug() << "Couldn't initialize client.";
        }

        break;

    case ServerNode:
        qDebug() << "Initializing server.";

        m_devicePtr = new Server();
        ok = static_cast<Server*>(m_devicePtr.data())->initialize(networkParams);

        if (ok)
            qDebug() << "Server object initialized.";
        else
            qDebug() << "Couldn't initialize server.";

        break;

    default:
        qDebug() << "Invalid node type.";
        break;
    }

    if (ok) {
        qDebug() << "Connecting read signal.";
        ok = connect(m_devicePtr, SIGNAL(readFromSocket(QByteArray)), this, SIGNAL(readData(QByteArray)));
    }

    if (ok) {
        qDebug() << "Connecting write signal.";
        ok = connect(this, SIGNAL(writeData(QByteArray)), m_devicePtr, SLOT(writeToSocket(QByteArray)));
    }

    if (ok) {
        qDebug() << "Connecting closed signal.";
        ok = connect(m_devicePtr, SIGNAL(closedConnection()), this, SIGNAL(closedConnection()));
    }

    if (ok) {
        qDebug() << "Connecting opened signal.";
        ok = connect(m_devicePtr, SIGNAL(openedConnection()), this, SIGNAL(openedConnection()));
    }

    return ok;
}



