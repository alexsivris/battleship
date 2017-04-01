#ifndef NETCONNECT_H
#define NETCONNECT_H
#include "net.h"

namespace Netzwerk
{
    class Netconnect;
    enum NetTyp{UNDEFINED, SERVER, CLIENT};
}

/**
 * @brief The Netzwerk::Netconnect class creates a Server or a Client by selecting Server or Client button at the beginning.
 * Depending on whether it's a Client or Server it passes the signals from the Gui and Logic to the Server/Client and the other way.
 * Logic and Gui don't need to care about whether a Server or a Client is used.
 */

class Netzwerk::Netconnect : public QObject
{
    Q_OBJECT
public:
    Netconnect(QObject *parent = 0);

public slots:
    // start
    /**
     * @brief sl_start_server is executed when a new game is started and the Server button is clicked
     */
    void sl_start_server();
    /**
     * @brief sl_start_client is executed when a new game is started and the Client button is clicked
     */
    void sl_start_client();

    // server
    /**
     * @brief sl_request_ip_port is executed to get the Servers IP and port
     */
    void sl_request_ip_port();
    /**
     * @brief sl_togui_ip_port is executed to send the Servers IP and port to the GUI
     * @param sIP
     * @param sPort
     */
    void sl_togui_ip_port(QString sIP, QString sPort);

    // client
    /**
     * @brief sl_fromgui_ip_port is executed to send the IP and port from the GUI to the Client
     * @param sIP
     * @param sPort
     */
    void sl_fromgui_ip_port(QString sIP, QString sPort);

    // from enemy
    /**
     * @brief sl_req_start_en is executed when the enemy (opponent) starts a request
     */
    void sl_req_start_en();
    /**
     * @brief sl_a_shot_en  is executed when the enemy (opponent) answers to a shot from the player
     * @param ui8Status
     * @param ui8ArrayLength
     * @param ui8aCoordinates
     */
    void sl_a_shot_en(u_int8_t ui8Status,  u_int8_t ui8ArrayLength,  u_int8_t *ui8aCoordinates);
    /**
    /**
     * @brief sl_shot_en is executed when the enemy (opponent) fires a shot
     * @param ui8X
     * @param ui8Y
     */
    void sl_shot_en(u_int8_t ui8X,  u_int8_t ui8Y);
    /**
     * @brief sl_a_req_en is executed when the enemy (opponent) answers to a request from the player
     * @param ui8Status
     */
    void sl_a_req_en(u_int8_t ui8Status);
    /**
     * @brief sl_neg_param_en is executed when the enemy (opponent) is negotiating the game parameters
     * checks whether a Server or Client is used, if it's a Server si_err_wrong_cmd is emitted
     * @param ui8FieldSizeX
     * @param ui8FieldSizeY
     * @param ui8CountBattleships
     * @param ui8CountCruiser
     * @param ui8CountDestroyer
     * @param ui8CountSubmarine
     */
    void sl_neg_param_en(u_int8_t ui8FieldSizeX, u_int8_t ui8FieldSizeY, u_int8_t ui8CountBattleships, u_int8_t ui8CountCruiser, u_int8_t ui8CountDestroyer, u_int8_t ui8CountSubmarine);

    // to enemy
    /**
     * @brief sl_req_start_me is executed when the player starts a request
     */
    void sl_req_start_me();
    /**
     * @brief sl_shot_me is executed when the player fires a shot
     * @param ui8X
     * @param ui8Y
     */
    void sl_shot_me(u_int8_t ui8X, u_int8_t ui8Y);
    /**
     * @brief sl_a_shot_me is executed when the player answers to a shot from the opponent
     * @param ui8Status
     * @param ui8ArrayLength
     * @param ui8aCoordinates
     */
    void sl_a_shot_me(u_int8_t ui8Status, u_int8_t ui8ArrayLength, u_int8_t* ui8aCoordinates);
    /**
     * @brief sl_a_req_me is executed when the player answers to a request from the opponent
     * @param ui8Status
     */
    void sl_a_req_me(u_int8_t ui8Status);
    /**
     * @brief sl_neg_param_me
     * @param ui8FieldSizeX
     * @param ui8FieldSizeY
     * @param ui8CountBattleships
     * @param ui8CountCruiser
     * @param ui8CountDestroyer
     * @param ui8CountSubmarine
     */
    void sl_neg_param_me(u_int8_t ui8FieldSizeX, u_int8_t ui8FieldSizeY, u_int8_t ui8CountBattleships, u_int8_t ui8CountCruiser, u_int8_t ui8CountDestroyer, u_int8_t ui8CountSubmarines);

    // server/client
    /**
     * @brief sl_togui_history is executed to send the connection status to the GUI
     * @param sMessage
     */
    void sl_togui_history(QString sMessage);
    /**
     * @brief sl_disconnect is executed to disconnect from the host
     */
    void sl_disconnect();

    // errors
    /**
     * @brief sl_socket_error is executed when a socket error
     * @param err
     */
    void sl_socket_error(QAbstractSocket::SocketError err);
    /**
     * @brief sl_wrong_cmd gives out an error when a wrong command is entered
     */
    void sl_wrong_cmd();
    /**
     * @brief sl_wrong_length gives out an error message when the command is of the wrong length
     */
    void sl_wrong_length();

    /**
     * @brief sl_incomplete_message
     */
    void sl_incomplete_message();

    /**
     * @brief sl_server_connected
     */
    void sl_server_connected();



signals:
    // to server
    /**
     * @brief si_request_ip_port - requests the Servers IP and port
     */
    void si_request_ip_port();
    // from server
    /**
     * @brief si_togui_ip_port - sends the IP and port from the Server to the GUI
     * @param sIP
     * @param sPort
     */
    void si_togui_ip_port(QString sIP, QString sPort);
    void si_server_connected();
    // to client
    /**
     * @brief si_fromgui_ip_port - sends IP and port from the GUI to the Client
     * @param sIP
     * @param sPort
     */
    void si_fromgui_ip_port(QString sIP, QString sPort);

    // from enemy
    /**
     * @brief si_req_start_en - sends start request from the opponent (enemy) to the player
     */
    void si_req_start_en();
    /**
     * @brief si_shot_en - sends the X and Y coordinates (a "shot") clicked by the opponent to the player
     * @param ui8X
     * @param ui8Y
     */
    void si_shot_en(u_int8_t ui8X,  u_int8_t ui8Y);
    /**
     * @brief si_a_shot_en - sends an opponents answer to a shot by the player back to the player (Hit, no Hit, Hit & Sunk, ...)
     * @param ui8Status
     * @param ui8ArrayLength
     * @param ui8aCoordinates
     */
    void si_a_shot_en(u_int8_t ui8Status, u_int8_t ui8ArrayLength, u_int8_t *ui8aCoordinates);
    /**
     * @brief si_a_req_en - sends an opponents answer to a players request back to the player (ok, denied, not ready, ...)
     * @param ui8Status
     */
    void si_a_req_en(u_int8_t ui8Status);
    /**
     * @brief si_neg_param_en  - sends an opponents negotiated game parameters to the player (field size, number of ships, ...)
     * @param ui8FieldSizeX
     * @param ui8FieldSizeY
     * @param ui8CountBattleships
     * @param ui8CountCruiser
     * @param ui8CountDestroyer
     * @param ui8CountSubmarines
     */
    void si_neg_param_en(u_int8_t ui8FieldSizeX, u_int8_t ui8FieldSizeY, u_int8_t ui8CountBattleships, u_int8_t ui8CountCruiser, u_int8_t ui8CountDestroyer, u_int8_t ui8CountSubmarines);

    // to enemy
    /**
     * @brief si_req_start_me - sends start request from the player to the opponent
     */
    void si_req_start_me();
    /**
     * @brief si_shot_me - sends the X and Y coordinates (a "shot") clicked by the player to the opponent
     * @param ui8X
     * @param ui8Y
     */
    void si_shot_me(u_int8_t ui8X,  u_int8_t ui8Y);
    /**
     * @brief si_a_shot_me - sends the players answer to a shot by the opponent back to the opponent (Hit, no Hit, Hit & Sunk, ...)
     * @param ui8Status
     * @param ui8ArrayLength
     * @param ui8Coordinates
     */
    void si_a_shot_me(u_int8_t ui8Status,  u_int8_t ui8ArrayLength,  u_int8_t *ui8Coordinates);
    /**
     * @brief si_a_req_me - sends the players answer to an opponents request back to the opponent (ok, denied, not ready, ...)
     * @param ui8Status
     */
    void si_a_req_me(u_int8_t ui8Status);

    /**
     * @brief si_neg_param_me - sends the players parameters to the opponent
     * @param ui8FieldSizeX
     * @param ui8FieldSizeY
     * @param ui8CountBattleships
     * @param ui8CountCruiser
     * @param ui8CountDestroyer
     * @param ui8CountSubmarines
     */
    void si_neg_param_me(u_int8_t ui8FieldSizeX, u_int8_t ui8FieldSizeY, u_int8_t ui8CountBattleships, u_int8_t ui8CountCruiser, u_int8_t ui8CountDestroyer, u_int8_t ui8CountSubmarines);


    // from server/client
    /**
     * @brief si_togui_history - sends the connection status of the server/client to the GUI
     * @param sMessage
     */
    void si_togui_history(QString sMessage);
    // to server/client
    /**
     * @brief si_disconnect - sends the command to disconnect to the server/client
     */
    void si_disconnect();

    // errors
    /**
     * @brief si_socket_error - sent when a socket error occurs
     * @param err
     */
    void si_socket_error(QAbstractSocket::SocketError err);
    /**
     * @brief si_err_cmd - sent when an unknown command is entered
     */
    void si_err_cmd();
    /**
     * @brief si_wrong_cmd - sent when a wrong command is entered
     */
    void si_wrong_cmd();

    /**
     * @brief si_err_length - sent if the command is of the wrong length
     */
    void si_err_length();

    /**
     * @brief si_err_incomplete_message emitted when the server is connected to a client
     */
    void si_err_incomplete_message();

private:
    /**
     * @brief m_pServer - pointer to the Server
     */
    Netzwerk::Server *m_pServer;
    /**
     * @brief m_pClient - pointer to the Client
     */
    Netzwerk::Client *m_pClient;
    /**
     * @brief m_NetTyp - either Server or Client
     */
    NetTyp m_NetTyp;

};

#endif // NETCONNECT_H
