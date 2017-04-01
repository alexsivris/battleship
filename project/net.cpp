
// by Philipp Weiss

#include "net.h"

using namespace Netzwerk;

Net::Net(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(si_send()), this, SLOT(sl_send()));
    connect(this, SIGNAL(si_wrong_cmd()), this, SLOT(sl_send_error()));
    connect(this, SIGNAL(si_wrong_length()), this, SLOT(sl_send_error()));
    connect(this, SIGNAL(si_buffer_not_empty()), this, SLOT(sl_read()));

   m_pTimer = new QTimer(this);
   m_pTimer->setSingleShot(true);

   connect(m_pTimer, SIGNAL(timeout()), this, SLOT(sl_incomplete_message()));
}

void Net::sl_read()
{
    qDebug() << "received Message";
    m_ReadBuffer.append(m_pMySocket->readAll());
    u_int8_t ui8aCoordinates[10], ui8Cmd, ui8Length;
    // 500ms time for rest of message to arrive
    m_pTimer->start(500);
    if(m_ReadBuffer.length() >= 2)
    {
        ui8Cmd = m_ReadBuffer[0];
        ui8Length = m_ReadBuffer[1];
        qDebug() << "ReadBuffer[0]: " << ui8Cmd << " ReadBuffer[1]: " << ui8Length;

        if(m_ReadBuffer.length() >= (2+ui8Length))
        {
            // stop timer if message arrived complete
            m_pTimer->stop();
            // check if command is assigned else emit signal wrong_cmd
            // special case 0x01: only client should get this command, this case is checked in netconnect.cpp
            // check for each command if Cmd and Length match else emit signal wrong_length
            // if Cmd and Length fit emit matching signals with the parameters
            // last step: remove message from buffer
            switch(ui8Cmd)
            {
            case 0x01:
                if(ui8Length != 6)
                {
                    emit si_wrong_length();
                    break;
                }
                else
                {
                    qDebug() << "Aushandeln der Spielfeldparameter";
                    emit si_neg_param_en(m_ReadBuffer[2], m_ReadBuffer[3], m_ReadBuffer[4], m_ReadBuffer[5], m_ReadBuffer[6], m_ReadBuffer[7]);
                    break;
                }
            case 0x02:
                if(ui8Length != 0)
                {
                    emit si_wrong_length();
                    break;
                }
                else
                {
                    qDebug() << "Anforderung Spielbeginn";
                    emit si_req_start_en();
                    break;
                }
            case 0x03:
                if(ui8Length != 2)
                {
                    emit si_wrong_length();
                    break;
                }
                else
                {
                    qDebug() << "Schuss";
                    emit si_shot_en(m_ReadBuffer[2], m_ReadBuffer[3]);
                    break;
                }
            case 0x10:
                if(ui8Length != 1)
                {
                    emit si_wrong_length();
                    break;
                }
                else
                {
                    qDebug() << "Antwort auf Anfrage";
                    emit si_a_req_en(m_ReadBuffer[2]);
                    break;
                }
            case 0x11:
                if((ui8Length < 1) || (ui8Length > 11))
                {
                    emit si_wrong_length();
                    break;
                }
                else
                {
                    for(int i=0; i<ui8Length-1;i++)
                    {
                        ui8aCoordinates[i] = m_ReadBuffer[3+i];
                    }
                    qDebug() << "Antwort auf Schuss";
                    qDebug() << m_ReadBuffer[2] << " " << " Länge des Arrays:" << ui8Length << " Koordinaten: " << ui8aCoordinates[0] << " " << ui8aCoordinates[1] << " " << ui8aCoordinates[2] << " " << ui8aCoordinates[3] << " " << ui8aCoordinates[4] << " " << ui8aCoordinates[5];
                    emit si_a_shot_en(m_ReadBuffer[2], (ui8Length-1), ui8aCoordinates);
                    break;
                }
            default:
                emit si_wrong_cmd();
                break;
            }
            m_ReadBuffer.remove(0,ui8Length+2);

            if(m_ReadBuffer.length()>0)
            {
                emit si_buffer_not_empty();
            }
        }
    }
}

// send message to enemy that message was incomplete
void Net::sl_incomplete_message()
{
    qDebug() << "incomplete message";
    m_ReadBuffer.clear();
    u_int8_t ui8Cmd = 0x10;
    u_int8_t ui8Length = 0x01;
    u_int8_t ui8Status = 0x03;
    m_SendBuffer.append(ui8Cmd);
    m_SendBuffer.append(ui8Length);
    m_SendBuffer.append(ui8Status);

    emit si_incomplete_message();
    emit si_send();
}

// to check if whole message has been sent
// if not remove the number of bytes sent from buffer and send the rest again
void Net::sl_check_send(qint64 iBytesSent)
{
    qint64 iDifference = m_SendBuffer.length() - iBytesSent;

    if(iDifference > 0)          // not full message sent
    {
        m_SendBuffer.remove(0,(m_SendBuffer.length() - iDifference));
        m_pMySocket->write(m_SendBuffer);
    }
    else if(iDifference == 0)    // if full message sent clear buffer
    {
        qDebug() << "full message sent";
        m_SendBuffer.clear();
    }
}

// socket error
void Net::sl_socket_error(QAbstractSocket::SocketError err)
{
    qDebug() << err;
    emit si_socket_error(err);
    qDebug() << m_pMySocket->errorString();
    emit si_togui_history(m_pMySocket->errorString());
}


void Net::sl_send_error()                                           // TODO connect with errrors
{
    u_int8_t ui8Cmd = 0x10;
    u_int8_t ui8Length = 0x01;
    u_int8_t ui8Status = 0x20; // error
    m_SendBuffer.append(ui8Cmd);
    m_SendBuffer.append(ui8Length);
    m_SendBuffer.append(ui8Status);

    emit si_send();
}

// append the messages to send to buffer
// slots get the parameters by signals emmited from netconnect.cpp
void Net::sl_shot(u_int8_t ui8X,u_int8_t ui8Y)
{
    u_int8_t ui8Cmd = 0x03;
    u_int8_t ui8Length = 0x02;
    m_SendBuffer.append(ui8Cmd);
    m_SendBuffer.append(ui8Length);
    m_SendBuffer.append(ui8X);
    m_SendBuffer.append(ui8Y);
    emit si_send();
}

void Net::sl_start_request()
{
    u_int8_t ui8Cmd = 0x02;
    u_int8_t ui8Length = 0x00;
    m_SendBuffer.append(ui8Cmd);
    m_SendBuffer.append(ui8Length);
    emit si_send();
}

void Net::sl_answer_request(u_int8_t ui8Status)
{
    qDebug() << "send answer request with status " << ui8Status;
    u_int8_t ui8Cmd = 0x10;
    u_int8_t ui8Length = 0x01;
    m_SendBuffer.append(ui8Cmd);
    m_SendBuffer.append(ui8Length);
    m_SendBuffer.append(ui8Status);
    emit si_send();
}

void Net::sl_answer_shot(u_int8_t ui8Status,u_int8_t ui8ArrayLength,u_int8_t *ui8aCoords)
{
    u_int8_t ui8Cmd = 0x11;
    u_int8_t ui8Length = 0x01;
    if((ui8Status == 0x03) || (ui8Status == 0x02))
    {
        ui8Length+= 2*ui8ArrayLength;
    }
    m_SendBuffer.append(ui8Cmd);
    m_SendBuffer.append(ui8Length);
    m_SendBuffer.append(ui8Status);

    if((ui8Status == 0x03) || (ui8Status == 0x02))
    {
        for(int i=0; i<2*ui8ArrayLength; i++)
        {
            m_SendBuffer.append(ui8aCoords[i]);
        }
    }
    qDebug() << "sl_answer_shot Länge der Nachricht: " << ui8Length << " Koordinaten: " << ui8aCoords[0] <<  " " << ui8aCoords[1] <<  " " << ui8aCoords[2] <<  " " << ui8aCoords[3] <<  " " << ui8aCoords[4] <<  " " << ui8aCoords[5];
    emit si_send();
}

// send the message
void Net::sl_send()
{
    if(m_pMySocket->isValid() == true)
    {
        m_pMySocket->write(m_SendBuffer);
    }
}
