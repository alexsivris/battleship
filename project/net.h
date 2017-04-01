#ifndef NET_H
#define NET_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QTimer>

/**
 * @brief namespace Netzwerk
 * has the purpose of grouping the classes Netzwerk::Client, Netzwerk::Net, and Netzwerk::Server together as the games' network
 */
namespace Netzwerk
{
    class Net;
    class Server;
    class Client;
}

/**
 * @brief The Netzwerk::Net class is the base class for the Server and Client class,
 * has slots which are important for sending and reading messages and occuring errors,
 * all signals which are emitted in this class are connected to slots in Netconnect
 */

class Netzwerk::Net : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Net
     * Constructor connects intern signals and slots which are important for sending and error messages
     * @param parent
     */
    Net(QObject *parent = 0);

public slots:

    /**
     * @brief sl_start_request
     * appends Cmd and Length for request to start to buffer and emits send signal
     */
    void sl_start_request();

    /**
     * @brief sl_shot
     * appends Cmd, Length and shot coordinates by user to buffer and emits send signal
     * @param ui8X
     * @param ui8Y
     */
    void sl_shot(u_int8_t ui8X,  u_int8_t ui8Y);

    /**
     * @brief sl_answer_request
     * appends Cmd, Length and Status for the request answer to buffer and emits send signal
     * @param ui8Status
     */
    void sl_answer_request(u_int8_t ui8Status);

    /**
     * @brief sl_answer_shot
     * answer to the enemy's shot,
     * appends Cmd, Length, Status, Length of the array and an Array with the coordinates of the sunken ship and emits send signal
     * @param ui8Status
     * @param ui8ArrayLength
     * @param ui8aCoords
     */
    void sl_answer_shot( u_int8_t ui8Status,  u_int8_t ui8ArrayLength,  u_int8_t *ui8aCoords);


    // errors
    /**
     * @brief sl_socket_error
     * leads the signal for a socket error further to Netconnect
     * @param err
     */
    void sl_socket_error(QAbstractSocket::SocketError err);

    /**
     * @brief sl_send_error
     * sends a message to the enemy that an error occured
     */
    void sl_send_error();

    /**
     * @brief sl_incomplete_message
     * is called when rest of the message doesn't arrive within the next 500ms,
     * sends a message to the enemy that the received message is incomplete,
     * leads the error signal for incomplete message further to Netconnect
     */
    void sl_incomplete_message();

    // read/write
    /**
     * @brief sl_read
     * started by signal m_pMySocket->readyRead();
     * appends all bytes to m_ReadBuffer,
     * read the buffer and emit the right message/error signal
     */
    void sl_read();

    /**
     * @brief sl_check_send
     * checks if all bytes have been sent,
     * if not delete the number of bytes sent from m_SendBuffer and send the rest again,
     * clear m_SendBuffer if everything has been sent
     * @param iBytesSent
     * number of bytes which have been sent by the socket
     */
    void sl_check_send(qint64 iBytesSent);

    /**
     * @brief sl_send
     * sends the bytes in m_SendBuffer
     */
    void sl_send();


signals:

    // to gui
    /**
     * @brief si_togui_history
     * passes any messages for the GUI's history further to Netconnect
     * @param sMessage
     */
    void si_togui_history(QString sMessage);

    // messages from enemy
    /**
     * @brief si_a_req_en
     * passes the enemy's answer to request to Netconnect
     * @param ui8Status
     */
    void si_a_req_en(u_int8_t ui8Status);

    /**
     * @brief si_a_shot_en
     * passes the enemy's answer to user's own shot further to Netconnect
     * @param ui8Status
     * @param ui8Length
     * @param ui8aCoordinates
     */
    void si_a_shot_en (u_int8_t ui8Status,  u_int8_t ui8Length,  u_int8_t *ui8aCoordinates);

    /**
     * @brief si_shot_en
     * passes the enemy's shot coordinates to Netconnect
     * @param ui8X
     * @param ui8Y
     */
    void si_shot_en(u_int8_t ui8X,  u_int8_t ui8Y);

    /**
     * @brief si_req_start_en
     * passes the enemy's request for start to Netconnect
     */
    void si_req_start_en();

    /**
     * @brief si_neg_param_en
     * passes the game parameters chosen by enemy to Netconnect
     * @param ui8XFieldSize
     * @param ui8YFieldSize
     * @param ui8CountBattleships
     * @param ui8CountCruiser
     * @param ui8CountDestroyer
     * @param ui8CountSubmarines
     */
    void si_neg_param_en(u_int8_t ui8XFieldSize, u_int8_t ui8YFieldSize, u_int8_t ui8CountBattleships, u_int8_t ui8CountCruiser, u_int8_t ui8CountDestroyer, u_int8_t ui8CountSubmarines);


    // send
    /**
     * @brief si_send
     * connected to sl_send()
     */
    void si_send();


     // errors
    /**
     * @brief si_wrong_cmd
     * emitted when an unknown command has been received
     */

    void si_wrong_cmd();
    /**
     * @brief si_socket_error
     * passes a socket error from Server or Client to Netconnect
     * @param err
     * specifies the Error
     */
    void si_socket_error(QAbstractSocket::SocketError err);

    /**
     * @brief si_wrong_length
     * emitted when the length doesn't match with the received command
     */
    void si_wrong_length();

    /**
     * @brief si_incomplete_message
     * emitted when the timer timed out which means that the rest of the message didn't arrive 500ms after the first part arrived
     */
    void si_incomplete_message();

    /**
     * @brief si_buffer_not_empty if buffer is not empty call Net::sl_read()s again
     */
    void si_buffer_not_empty();

protected:

    /**
     * @brief m_pMySocket
     * Socket to communicate between server and client
     */
    QTcpSocket *m_pMySocket;

    /**
     * @brief m_ReadBuffer
     * read messages are appended to this buffer,
     * buffer is cleared when full message has been passed to Netconnect
     */
    QByteArray m_ReadBuffer;

    /**
     * @brief m_SendBuffer
     * messages which should be sent are appended to this buffer,
     * sent in sl_send(),
     * cleared in check_send() when complete message has been sent
     */
    QByteArray m_SendBuffer;

    /**
     * @brief m_timer
     * set to single shot,
     * fires a shot 500ms after a message has been received,
     * is stopped only when a full message has been received
     */
    QTimer *m_pTimer;

};



/**
 * @brief The Netzwerk::Server class
 * inherits from Netzwerk::Net,
 * server is started when start server button is clicked,
 * disconnects the slot when disconnect button is clicked,
 * sends messages accordingly to the GUI's history,
 * sends the game parameters,
 * only one connection to the slot at once possible
 */

class Netzwerk::Server: public Net
{
    Q_OBJECT
public:
    /**
     * @brief Server
     * new Socket is constructed
     * @param parent
     */
    Server(QObject *parent = 0);
public slots:
    /**
     * @brief sl_negotiate
     * gets game parameters from Logic passed over Netconnect, appends them to m_SendBuffer and emits the send signal
     * @param ui8FieldSizeX
     * @param ui8FieldSizeY
     * @param ui8CountBattleships
     * @param ui8CountCruiser
     * @param ui8CountDestroyer
     * @param ui8CountSubmarine
     */
    void sl_negotiate(u_int8_t ui8FieldSizeX, u_int8_t ui8FieldSizeY, u_int8_t ui8CountBattleships, u_int8_t ui8CountCruiser, u_int8_t ui8CountDestroyer, u_int8_t ui8CountSubmarine);

    /**
     * @brief sl_connection_request
     * is called when a connection is available,
     * connects the socket if unconnected to the
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
next pending connection,
     * emits a signal that the socket is connected,
     * connects socket signals with some slots
     */
    void sl_connection_request();

    /**
     * @brief sl_disconnect
     * called when button disconnect is pushed,
     * disconnects the socket,
     * else emits message: not connected
     */
    void sl_disconnect();

    /**
     * @brief sl_disconnected
     * called when socket disconnected,
     * emits message: Disconnected
     */
    void sl_disconnected();

    /**
     * @brief sl_request_ip_port
     * called when start server button is pushed,
     * set the server to listen state,
     * gets the server's ip and port,
     * emits si_togui_ip_port
     */
    void sl_request_ip_port();

    //void sl_connected();

signals:

    /**
     * @brief si_togui_ip_port
     * passes the ip and port to the GUI
     * @param sIP
     * @param sPort
     */
    void si_togui_ip_port(QString sIP, QString sPort);

    /**
     * @brief si_connected
     * emitted when socket is connected,
     * passed to Logic
     */
    void si_connected();

    //void si_next_connection();

private:
    /**
     * @brief m_MyServer
     */
    QTcpServer m_MyServer;

public:
    /**
     * @brief get_ip
     * finds the IP of the user
     * @return
     * returns the IP as string
     */
    QString get_ip();
};

/**
 * @brief The Netzwerk::Client class inherits from Netzwerk::Net,
 * connects to server when connect button is clicked,
 * disconnects when disconnect button is clicked,
 * sends messages accordingly to the GUI's history
 */
class Netzwerk::Client: public Net
{
    Q_OBJECT
public:
    /**
     * @brief Client
     * new socket is constructed,
     * socket signals are connected to slots
     * @param parent
     */
    Client(QObject *parent = 0);
public slots:
    /**
     * @brief sl_connect
     * called when connect button is pushed,
     * connects to the host if unconnected,
     * else sends message to history: Already connected or connecting
     * @param sIP
     * @param sPort
     */
    void sl_connect(QString sIP, QString sPort);

    /**
     * @brief sl_connected
     * called when connected to server,
     * sends message to history with the peer's IP
     */
    void sl_connected();

    /**
     * @brief sl_disconnect
     * called when button disconnect is pushed,
     * disconnects the socket,
     * else emits message: not connected
     */
    void sl_disconnect();
    /**
     * @brief sl_disconnected
     * called when start server button is pushed,
     * set the server to listen state,
     * gets the server's ip and port,
     * emits si_togui_ip_port
     */
    void sl_disconnected();

};

#endif //NET_H
