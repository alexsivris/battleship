/** \brief The graphical user interface of the server-configuration
  * \author Alexandros Sivris
  * \date March 2015
  */
#ifndef GUISERVER_H
#define GUISERVER_H

#include <QWidget>
#include "net.h"
#include "battlefield.h"
#include "ui_guiserver.h"
#include <QDebug>
#include <QMessageBox>

namespace Gui {
class GuiServer;
}
namespace Ui {
class GuiServer;
}

/**
 * @brief The Gui::GuiServer class
 */
class Gui::GuiServer : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief si_request_ip_port - signal to request the Servers IP and port
     */
    void si_request_ip_port();
    /**
     * @brief si_start_server - signal to start a new Server window
     */
    void si_start_server();
    /**
     * @brief si_server_disconnected - signal to disconnect the Server from the Client
     */
    void si_server_disconnected();
    /**
     * @brief si_close_app - sent to close the game
     */
    void si_close_app();
    /**
     * @brief si_neg_param_gui - signal that sends the negotiated parameters
     */
    void si_neg_param_gui(u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t,u_int8_t);
public:
    /**
     * @brief GuiServer - The constructor
     * @param parent
     */
    explicit GuiServer(QWidget *parent = 0);
    /**
     * @brief ~GuiServer - The destructor
     */
    ~GuiServer();
    /**
     * @brief setBfPointer
     * @param ptr
     */
    void setBfPointer(Battlefield * ptr);
    /**
     * @brief setIpPort
     * @param ip
     * @param port
     */
    void setIpPort(QString ip,QString port);
public slots:
    /**
     * @brief sl_insert_ip - executed to show the Server IP in the address window of the Server
     */
    void sl_insert_ip(QString);
    /**
     * @brief sl_insert_history
     */
    void sl_insert_history(QString);
    /**
     * @brief sl_insert_port - executed to show the Server port in the port window of the Server
     */
    void sl_insert_port(QString);
    /**
     * @brief sl_start_server - executed to start the Server window
     */
    void sl_start_server();
    /**
     * @brief sl_server_disconnect - executed to disconnect the Server from the Client
     */
    void sl_server_disconnect();
    /**
     * @brief sl_close_app - executed to close the game
     */
    void sl_close_app();
private:
    /**
     * @brief m_gsUi pointer to the GuiServer class
     */
    Ui::GuiServer *m_gsUi;

    /**
     * @brief m_pBf pointer to the Battlefield class
     */
    Gui::Battlefield *m_pBf;
};

#endif // GUISERVER_H
