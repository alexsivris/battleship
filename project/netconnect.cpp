#include "netconnect.h"

using namespace Netzwerk;

Netconnect::Netconnect(QObject *parent) : QObject(parent)
{
    m_pClient = NULL;
    m_pServer = NULL;
    m_NetTyp = UNDEFINED;
    connect(this, SIGNAL(si_wrong_cmd()), this, SLOT(sl_wrong_cmd()));
}

void Netconnect::sl_start_client()                          // mit client button verbinden in master
{
    qDebug() << "Starte Client\n";
    if (m_NetTyp == UNDEFINED)
    {
        m_NetTyp = CLIENT;
        m_pClient = new Client(this);
        qDebug() << "Client erstellt";
        connect(this, SIGNAL(si_fromgui_ip_port(QString,QString)), m_pClient, SLOT(sl_connect(QString,QString)));
        connect(this, SIGNAL(si_disconnect()), m_pClient, SLOT(sl_disconnect()));
        connect(m_pClient, SIGNAL(si_togui_history(QString)), this, SLOT(sl_togui_history(QString)));
        connect(m_pClient, SIGNAL(si_socket_error(QAbstractSocket::SocketError)), this, SLOT(sl_socket_error(QAbstractSocket::SocketError)));
        connect(m_pClient, SIGNAL(si_wrong_length()), this, SLOT(sl_wrong_length()));

        // from enemy
        connect(m_pClient, SIGNAL(si_req_start_en()), this, SLOT(sl_req_start_en()));
        connect(m_pClient, SIGNAL(si_shot_en(u_int8_t,u_int8_t)), this, SLOT(sl_shot_en(u_int8_t,u_int8_t)));
        connect(m_pClient, SIGNAL(si_a_shot_en(u_int8_t,u_int8_t, u_int8_t*)), this, SLOT(sl_a_shot_en(u_int8_t,u_int8_t, u_int8_t*)));
        connect(m_pClient, SIGNAL(si_a_req_en(u_int8_t)), this, SLOT(sl_a_req_en(u_int8_t)));
        connect(m_pClient, SIGNAL(si_neg_param_en(u_int8_t, u_int8_t, u_int8_t, u_int8_t, u_int8_t, u_int8_t)), this, SLOT(sl_neg_param_en(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t)));
        connect(m_pClient, SIGNAL(si_wrong_cmd()), this, SLOT(sl_wrong_cmd()));

        // to enemy
        connect(this, SIGNAL(si_req_start_me()), m_pClient, SLOT(sl_start_request()));
        connect(this, SIGNAL(si_shot_me(u_int8_t,u_int8_t)), m_pClient, SLOT(sl_shot(u_int8_t,u_int8_t)));
        connect(this, SIGNAL(si_a_shot_me(u_int8_t,u_int8_t, u_int8_t*)), m_pClient, SLOT(sl_answer_shot(u_int8_t,u_int8_t, u_int8_t*)));
        connect(this, SIGNAL(si_a_req_me(u_int8_t)), m_pClient, SLOT(sl_answer_request(u_int8_t)));

    }
}
void Netconnect::sl_start_server()                                // mit server button verbinden in master
{
    if (m_NetTyp == UNDEFINED)
    {

        m_NetTyp = SERVER;
        m_pServer = new Server(this);
        connect(this, SIGNAL(si_request_ip_port()), m_pServer, SLOT(sl_request_ip_port()));
        connect(m_pServer, SIGNAL(si_togui_ip_port(QString,QString)), this, SLOT(sl_togui_ip_port(QString,QString)));
        connect(this, SIGNAL(si_disconnect()), m_pServer, SLOT(sl_disconnect()));
        connect(m_pServer, SIGNAL(si_togui_history(QString)), this, SLOT(sl_togui_history(QString)));
        connect(m_pServer, SIGNAL(si_socket_error(QAbstractSocket::SocketError)), this, SLOT(sl_socket_error(QAbstractSocket::SocketError)));
        connect(m_pServer, SIGNAL(si_wrong_length()), this, SLOT(sl_wrong_length()));
        connect(m_pServer, SIGNAL(si_connected()), this, SLOT(sl_server_connected()));

        // from enemy
        connect(m_pServer, SIGNAL(si_req_start_en()), this, SLOT(sl_req_start_en()));
        connect(m_pServer, SIGNAL(si_shot_en(u_int8_t,u_int8_t)), this, SLOT(sl_shot_en(u_int8_t,u_int8_t)));
        connect(m_pServer, SIGNAL(si_a_shot_en(u_int8_t,u_int8_t, u_int8_t*)), this, SLOT(sl_a_shot_en(u_int8_t,u_int8_t, u_int8_t*)));
        connect(m_pServer, SIGNAL(si_a_req_en(u_int8_t)), this, SLOT(sl_a_req_en(u_int8_t)));
        connect(m_pServer, SIGNAL(si_wrong_cmd()), this, SLOT(sl_wrong_cmd()));

        // to enemy
        connect(this, SIGNAL(si_req_start_me()), m_pServer, SLOT(sl_start_request()));
        connect(this, SIGNAL(si_shot_me(u_int8_t,u_int8_t)), m_pServer, SLOT(sl_shot(u_int8_t,u_int8_t)));
        connect(this, SIGNAL(si_a_shot_me(u_int8_t,u_int8_t, u_int8_t*)), m_pServer, SLOT(sl_answer_shot(u_int8_t,u_int8_t, u_int8_t*)));
        connect(this, SIGNAL(si_a_req_me(u_int8_t)), m_pServer, SLOT(sl_answer_request(u_int8_t)));
        connect(this, SIGNAL(si_neg_param_me(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t)), m_pServer, SLOT(sl_negotiate(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t)));
    }
}





void Netconnect::sl_server_connected()
{
    emit si_server_connected();
}

//                                                                      TODO connect with GUI in master
void Netconnect::sl_request_ip_port()
{
    emit si_request_ip_port();
}

void Netconnect::sl_togui_ip_port(QString sIp, QString sPort)
{
    emit si_togui_ip_port(sIp, sPort);
}

void Netconnect::sl_fromgui_ip_port(QString sIp, QString sPort)
{
    emit si_fromgui_ip_port(sIp, sPort);
}

void Netconnect::sl_togui_history(QString sMessage)
{
    emit si_togui_history(sMessage);
}

void Netconnect::sl_disconnect()
{
    emit si_disconnect();
}

// errors                                                            TODO connect in master
void Netconnect::sl_socket_error(QAbstractSocket::SocketError err)
{
    emit si_socket_error(err);
}

void Netconnect::sl_wrong_cmd()
{
    qDebug() << "MESSAGE ERROR: Wrong Command received";
    emit si_err_cmd();
    emit si_togui_history("MESSAGE ERROR: Wrong Command received");
}

void Netconnect::sl_wrong_length()
{
    qDebug() << "MESSAGE ERROR: Wrong Length";
    emit si_err_length();
    emit si_togui_history("MESSAGE ERROR: Wrong Length");
}

void Netconnect::sl_incomplete_message()
{
    qDebug() << "MESSAGE ERROR: Incomplete message";
    emit si_err_incomplete_message();
    emit si_togui_history("MESSAGE ERROR: Incomplete message");
}

//                                                              TODO connect with Logic in master

// from enemy
void Netconnect::sl_req_start_en()
{
    emit si_req_start_en();
}

void Netconnect::sl_shot_en( u_int8_t ui8X,  u_int8_t ui8Y)
{
    qDebug() << "X: " << ui8X << " Y: " << ui8Y;
    emit si_shot_en(ui8X, ui8Y);
}

void Netconnect::sl_a_req_en( u_int8_t ui8Status)
{
    qDebug() << "Status " << ui8Status;
    emit si_a_req_en(ui8Status);
}

void Netconnect::sl_a_shot_en( u_int8_t ui8Status,  u_int8_t ui8Length,  u_int8_t *ui8aCoords)
{
    qDebug() << "Status " << ui8Status << " Länge der Nachricht: " << ui8Length << " Koordinaten: " << ui8aCoords[0] << ui8aCoords[1] << ui8aCoords[2] << ui8aCoords[3] << ui8aCoords[4] << ui8aCoords[5] << ui8aCoords[6] << ui8aCoords[7] << ui8aCoords[8] << ui8aCoords[9];
    emit si_a_shot_en(ui8Status, ui8Length, ui8aCoords);
}

void Netconnect::sl_neg_param_en(u_int8_t ui8FieldSizeX, u_int8_t ui8FieldSizeY, u_int8_t ui8CountBattleships, u_int8_t ui8CountCruiser, u_int8_t ui8CountDestroyer, u_int8_t ui8CountSubmarine)
{
    if(m_NetTyp == SERVER)
    {
        emit si_wrong_cmd();
    }
    else if(m_NetTyp == CLIENT)
    {
        qDebug() << ui8FieldSizeX << " " << ui8FieldSizeY << " " << ui8CountBattleships << " " << ui8CountCruiser << " " << ui8CountDestroyer << " " << ui8CountSubmarine;
        emit si_neg_param_en(ui8FieldSizeX, ui8FieldSizeY, ui8CountBattleships, ui8CountCruiser, ui8CountDestroyer, ui8CountSubmarine);
    }
}

// to enemy
void Netconnect::sl_req_start_me()
{
    emit si_req_start_me();
}

void Netconnect::sl_shot_me(u_int8_t ui8X, u_int8_t ui8Y)
{
    emit si_shot_me(ui8X, ui8Y);
}

void Netconnect::sl_a_shot_me(u_int8_t ui8Status,u_int8_t ui8Length,u_int8_t* ui8aCoords)
{
    qDebug()<< "sl_a_shot_me " << ui8aCoords[0] << " " << ui8aCoords[1] << " "<< ui8aCoords[2] << " " << ui8aCoords[3];
    emit si_a_shot_me(ui8Status, ui8Length, ui8aCoords);
}

void Netconnect::sl_a_req_me(u_int8_t ui8Status)
{
    emit si_a_req_me(ui8Status);
}

void Netconnect::sl_neg_param_me(u_int8_t ui8FieldSizeX, u_int8_t ui8FieldSizeY, u_int8_t ui8CountBattleships, const u_int8_t ui8CountCruiser, const u_int8_t ui8CountDestroyer, const u_int8_t ui8CountSubmarine)
{
    qDebug() << "neg_param_me in netconnect";
    emit si_neg_param_me(ui8FieldSizeX, ui8FieldSizeY, ui8CountBattleships, ui8CountCruiser, ui8CountDestroyer, ui8CountSubmarine);
}


