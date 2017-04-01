#include "net.h"
#include "QDebug"

using namespace Netzwerk;

Client::Client(QObject *parent) : Net(parent)
{
    m_pMySocket = new QTcpSocket(this);
    connect(m_pMySocket, SIGNAL(disconnected()), this, SLOT(sl_disconnected()));
    connect(m_pMySocket, SIGNAL(bytesWritten(qint64)), this, SLOT(sl_check_send(qint64)));
    connect(m_pMySocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sl_socket_error(QAbstractSocket::SocketError)));
    connect(m_pMySocket, SIGNAL(readyRead()), this, SLOT(sl_read()));
    connect(m_pMySocket, SIGNAL(connected()), this, SLOT(sl_connected()));
}

void Client::sl_connect(QString sIP, QString sPort)
{
    u_int16_t ui16Port = sPort.toShort();
    if(m_pMySocket->state() == QTcpSocket::UnconnectedState)
    {
        m_pMySocket->connectToHost(sIP,ui16Port);
        m_pMySocket->setReadBufferSize(100); // set read buffer size to a maximum
    }
    else
    {
        emit si_togui_history("Already connected or connecting");
    }
}

void Client::sl_connected()
{
    QString sMessage = "Connected to: " + m_pMySocket->peerAddress().toString();
    emit si_togui_history(sMessage);
}

void Client::sl_disconnect()
{
    if(m_pMySocket->state() == QTcpSocket::ConnectedState)
    {
        m_pMySocket->disconnectFromHost();
    }
    else
    {
        emit si_togui_history("Not connected");
    }
}

void Client::sl_disconnected()
{
    if(m_pMySocket->state() == QTcpSocket::UnconnectedState)
    {
       emit si_togui_history("Disconnected");
    }
}
