// by Philipp Weiss

#include "net.h"
#include "guiserver.h"
#include <QHostAddress>
#include <QHostInfo>
#include <QDebug>
using namespace Netzwerk;

Server::Server(QObject *parent) : Net(parent)
{
    m_pMySocket = new QTcpSocket(this);
    connect(&m_MyServer, SIGNAL(newConnection()), this,
            SLOT(sl_connection_request()));
}

void Server::sl_connection_request()
{
    // connect always to incoming request if unconnected
    if(m_pMySocket->state() == QTcpSocket::UnconnectedState)
    {
        delete m_pMySocket;
        m_pMySocket = m_MyServer.nextPendingConnection();
        m_pMySocket->setReadBufferSize(100);    // set read buffer size to a maximum
        if(m_pMySocket->waitForConnected(1000)) // waits up to 1000ms  and returns true if connected in this time
        {
            m_MyServer.close();
            QString sMessage = "Connected to: " + m_pMySocket->peerAddress().toString();
            emit si_connected();
            emit si_togui_history(sMessage);

            connect(m_pMySocket, SIGNAL(disconnected()), this, SLOT(sl_disconnected()));
            connect(m_pMySocket, SIGNAL(bytesWritten(qint64)), this, SLOT(sl_check_send(qint64)));
            connect(m_pMySocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sl_socket_error(QAbstractSocket::SocketError)));
            connect(m_pMySocket, SIGNAL(readyRead()), this, SLOT(sl_read()));
        }
    }
}

// when disconnect button is pushed
void Server::sl_disconnect()
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


void Server::sl_disconnected()
{
    if(m_pMySocket->state() == QTcpSocket::UnconnectedState)
    {
       emit si_togui_history("Disconnected");
    }
}

// get parameters from Logic/Gui and send them to enemy
void Server::sl_negotiate(u_int8_t ui8FieldSizeX, u_int8_t ui8FieldSizeY, u_int8_t ui8CountBattleships, u_int8_t ui8CountCruiser, u_int8_t ui8CountDestroyer, u_int8_t ui8CountSubmarine)
{
    const u_int8_t ui8Cmd = 0x01;
    const u_int8_t ui8Length = 0x06;
    //qDebug() << ui8FieldSizeX << " " << ui8FieldSizeY << " " << ui8CountBattleships << " " << ui8CountCruiser << " " << ui8CountDestroyer << " " << ui8CountSubmarine;
    m_SendBuffer.append(ui8Cmd);
    m_SendBuffer.append(ui8Length);
    m_SendBuffer.append(ui8FieldSizeX);
    m_SendBuffer.append(ui8FieldSizeY);
    m_SendBuffer.append(ui8CountBattleships);
    m_SendBuffer.append(ui8CountCruiser);
    m_SendBuffer.append(ui8CountDestroyer);
    m_SendBuffer.append(ui8CountSubmarine);
    emit si_send();
}

// start Server and send IP and port to GUI when requested
void Server::sl_request_ip_port()
{
    m_MyServer.listen(QHostAddress::Any,1234);
    emit si_togui_history("Server gestartet");

    u_int16_t ui16Tmp = m_MyServer.serverPort();
    QString sPort =  QString::number(ui16Tmp, 10);
    QString sIp = get_ip();
    // send IP and port to GUI
    emit si_togui_ip_port(sIp,sPort);
}

// get ip address of current PC
QString Server::get_ip()
{
    QString str;
    QHostInfo info = QHostInfo::fromName( QHostInfo::localHostName() );
    QList<QHostAddress> l= info.addresses();
    for(int i=0; i<l.count(); i++) {
        str = str + l[i].toString();
    }
    return str;
}
